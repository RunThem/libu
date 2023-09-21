#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

/* libs */
#include <u/core/str.h>

#define SOCK_MAX_OPTS 4

typedef struct {
  int opt_id;
  void* value;
  socklen_t value_len;
} u_sock_opt;

/*
 * socket 配置文件
 *
 * url -> "tcp4//127.0.0.1#8080"
 * url -> "tcp4//127.0.0.1#8080"
 *
 * 若listen > 0, 则表示为服务器模式
 * */
typedef struct {
  int fd;

  u_str_t url;
  int listen;

  bool nonblock;

  u_sock_opt opts[SOCK_MAX_OPTS];
  size_t opts_len;
} u_sock_conf_t;

typedef struct {
  socklen_t alen;
  union {
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_un un;
  } addr;
} sock_addr_t;

/*
 * 创建一个socket, .fd就是描述符
 * */
int u_sock_open(u_sock_conf_t* conf);
