#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* libs */
#include "fs.h"

off_t fs_size(c_str filename) {
  int ret        = 0;
  struct stat st = {0};

  u_ret_if(filename == nullptr, -1);

  ret = stat(filename, &st);
  u_ret_if(ret != 0, -1, "stat('%s') failed", filename);

  return st.st_size;
}

bool fs_exists(c_str filename) {
  return -1 != fs_size(filename);
}

str_t fs_read(c_str filename) {
  ret_t code  = 0;
  int fd      = 0;
  off_t size  = 0;
  ssize_t len = 0;
  str_t str   = {0};

  u_ret_if(filename == nullptr, str);

  size = fs_size(filename);
  u_err_if(size == -1);

  code = str_init(&str, size);
  u_err_if(code != 0);

  fd = open(filename, O_RDONLY);
  u_err_if(fd < 0);

  len = read(fd, str.c_str, size);
  u_err_if(len != size);

  close(fd);

  str.len = len;

  return str;

err:
  if (str_isinit(&str)) {
    str_cleanup(&str);
  }

  u_close_if(fd);

  return str;
}

off_t fs_write(c_str filename, c_str buf, size_t len) {
  int fd       = 0;
  ssize_t size = 0;

  u_ret_if(filename == nullptr, -1);
  u_ret_if(buf == nullptr, -1);

  fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  u_err_if(fd < 0);

  size = write(fd, buf, len);
  u_err_if(size != len);

  close(fd);

  return size;

err:
  u_close_if(fd);

  return -1;
}

bool fs_remove(c_str filename) {
  int ret = 0;

  u_ret_if(filename == nullptr, false);

  ret = unlink(filename);
  u_err_if(ret < 0);

  return true;

err:
  return false;
}
