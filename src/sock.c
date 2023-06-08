#include "sock.h"

#include <stdlib.h>

struct sock_url {
  int family;
  int type;
  int protocol;

  str_t addr;

  in_port_t port;
};

static void sock_pack_url(c_str url, struct sock_url* url_res);
static bool sock_set_opt(sock_conf_t* conf, int fd);
static int sock_set_nonblock(sock_conf_t* conf);
static int sock_open_ip(sock_conf_t* conf, struct sock_url* url);
static int sock_open_unix(sock_conf_t* conf, struct sock_url* url);

static void sock_pack_url(c_str url, struct sock_url* url_res) {
  c_str tmp = nullptr;
  if (strncmp(url, "tcp", 3) == 0) {
    url_res->type     = SOCK_STREAM;
    url_res->protocol = IPPROTO_TCP;
  } else if (strncmp(url, "udp", 3) == 0) {
    url_res->type     = SOCK_DGRAM;
    url_res->protocol = IPPROTO_UDP;
  }

  url += 3;
  if (url[0] == '4') {
    url_res->family = AF_INET;
    url += 3;
  } else if (url[0] == '6') {
    url_res->family = AF_INET6;
    url += 3;
  } else if (url[0] == '/' && url[2] == '/') {
    url_res->family = AF_LOCAL;
    url += 2;
  }

  if (url_res->family == AF_INET || url_res->family == AF_INET6) {
    tmp = strrchr(url, '#');

    if (tmp == nullptr) {
      url_res->addr = str_new(url);
    } else {
      url_res->port = atoi(tmp + 1);
      url_res->addr = str_new(url, tmp - url);
    }
  } else {
    url_res->addr = str_new(url);
  }
}

static bool sock_set_opt(sock_conf_t* conf, int fd) {
  int ret = 0;

  for (size_t i = 0; i < conf->opts_len; i++) {
    auto opt = &conf->opts[i];

    ret = setsockopt(fd, SOL_SOCKET, opt->opt_id, opt->value, opt->value_len);
    u_goto_if(ret < 0);
  }
  return true;

err:
  return false;
}

static int sock_set_nonblock(sock_conf_t* conf) {
  int ret   = 0;
  int flags = 0;

  ret = fcntl(conf->fd, F_GETFL, 0);
  u_goto_if(ret < 0);

  ret = fcntl(conf->fd, F_SETFL, flags | O_NONBLOCK);
  u_goto_if(ret < 0);

  return 0;

err:
  return ret;
}

static int sock_open_ip(sock_conf_t* conf, struct sock_url* url) {
  int fd     = -1;
  int ret    = 0;
  str_t port = nullptr;
  struct addrinfo hints;
  struct addrinfo* res = nullptr;

  hints.ai_family   = url->family;
  hints.ai_socktype = url->type;
  hints.ai_protocol = url->protocol;
  hints.ai_flags    = AI_CANONNAME;

  if (conf->listen > 0) {
    hints.ai_flags |= AI_PASSIVE;
  }

  port = str_new_f("%u", url->port);

  ret = getaddrinfo(url->addr->c_str, port->c_str, &hints, &res);
  u_goto_if(ret != 0, err, "getaddrinfo() -> %d(%s)", ret, gai_strerror(ret));

  for (auto ai = res; ai != nullptr; ai = ai->ai_next) {
    fd = socket(url->family, url->type, url->protocol);
    if (fd == -1) {
      err("socket() -> %d", fd);
      continue;
    }

    ret = sock_set_opt(conf, fd);
    u_goto_if(!ret);

    /* server */
    if (conf->listen > 0) {
      ret = bind(fd, res->ai_addr, res->ai_addrlen);
      u_goto_if(ret == -1);

      if (url->type == SOCK_STREAM) {
        ret = listen(fd, conf->listen);
        u_goto_if(ret != 0);
      }
    } else { /* client */
      ret = connect(fd, ai->ai_addr, ai->ai_addrlen);
      if (ret != 0) {
        close(fd);
        fd = -1;
      }
    }
  }

  str_cleanup(&port);
  freeaddrinfo(res);

  conf->fd = fd;
  return 0;

err:
  str_cleanup(&port);
  freeaddrinfo(res);

  return ret;
}

static int sock_open_unix(sock_conf_t* conf, struct sock_url* url) {
  int fd  = 0;
  int ret = 0;

  struct sockaddr_un addr;
  socklen_t addr_len = sizeof(addr);

  fd = socket(url->family, url->type, url->protocol);
  u_goto_if(fd < 0);

  ret = sock_set_opt(conf, fd);
  u_goto_if(!ret);

  addr.sun_family = AF_LOCAL;
  strncpy(addr.sun_path, url->addr->c_str, url->addr->len);

  if (conf->listen > 0) {
    ret = bind(fd, (struct sockaddr*)&addr, addr_len);
    u_goto_if(ret != 0);

    if (url->type == SOCK_STREAM) {
      ret = listen(fd, conf->listen);
      u_goto_if(ret != 0);
    }
  } else {
    ret = connect(fd, (struct sockaddr*)&addr, addr_len);
    u_goto_if(ret != 0);
  }

  conf->fd = fd;

  return 0;

err:
  u_close_if(fd);

  return ret;
}

int sock_open(sock_conf_t* conf) {
  int ret             = 0;
  struct sock_url url = {0};

  sock_pack_url(conf->url, &url);

  if (url.family == AF_LOCAL) {
    inf("domain(AF_LOCAL), protocol(%d), addr(%s)", url.protocol, url.addr->c_str);

    ret = sock_open_unix(conf, &url);
  } else {
    inf("domain(%d), protocol(%d), addr(%s), port(%d)",
        url.family,
        url.protocol,
        url.addr->c_str,
        url.port);

    ret = sock_open_ip(conf, &url);
  }

  str_cleanup(&url.addr);

  if (ret == 0 && conf->nonblock) {
    ret = sock_set_nonblock(conf);
  }

  return ret;
}
