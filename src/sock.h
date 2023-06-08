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
#include "str.h"
#include "u.h"

#define SOCK_MAX_OPTS 4

typedef struct {
  int opt_id;
  void* value;
  socklen_t value_len;
} sock_opt;

typedef struct {
  int fd;

  c_str url;
  int listen;

  bool nonblock;

  sock_opt opts[SOCK_MAX_OPTS];
  size_t opts_len;
} sock_conf_t;

typedef struct {
  socklen_t alen;
  union {
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_un un;
  } addr;
} sock_addr_t;

int sock_open(sock_conf_t* conf);
