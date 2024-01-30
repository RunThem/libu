#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* libs */
#include <u/util.h>

off_t u_fs_size(u_str_t file) {
  struct stat st = {0};
  ret_t code     = 0;

  u_check_ret(file == nullptr, -1);

  code = stat(file, &st);
  u_err_if(code != 0, "fs call stat('%s') failed.", file);

  infln("fs size(file('%s'), size = %zu)", file, st.st_size);

  return st.st_size;

err:
  return -1;
}

bool u_fs_exist(u_str_t file) {
  ret_t code = 0;
  u_check_ret(file == nullptr, false);

  code = access(file, F_OK);

  infln("fs exist(file(%s), exist = %s)", file, code == -1 ? "false" : "true");

  return code == 0;
}

u_str_t u_fs_read(u_str_t file) {
  ret_t code  = 0;
  int fd      = 0;
  off_t size  = 0;
  ssize_t len = 0;
  u_str_t buf = {0};

  u_check_ret(file == nullptr, nullptr);

  size = u_fs_size(file);
  u_err_if(size == -1, "fs read('%s') get size failed.", file);

  fd = open(file, O_RDONLY);
  u_err_if(fd < 0, "fs read('%s') call open() failed.", file);

  buf = u_str_new(size);
  u_err_if(buf == nullptr, "fs read('%s') create buffer failed., size(%zu)", file, size);

  len = read(fd, buf, size);
  u_err_if(len != size, "fs read() read %zu bytes, file size(%zu)", len, size);

  close(fd);

  u_str_slen(&buf, len);

  return buf;

err:
  u_close_if(fd);

  if (buf != nullptr) {
    u_str_cleanup(&buf);
  }

  return nullptr;
}

bool u_fs_remove(u_str_t file) {
  ret_t code = 0;

  u_check_ret(file == nullptr, false);

  code = unlink(file);
  u_err_if(code < 0, "fs remove('%s') call unlink() failed.", file);

  return true;

err:
  return false;
}
