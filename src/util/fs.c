#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* libs */
#include <u/util/fs.h>

off_t u_fs_size(u_str_t file) {
  struct stat st = {0};
  ret_t code     = 0;

  u_assert(file == nullptr);

  code = stat(file, &st);
  u_ret_if(code != 0, -1, "stat('%s') failed", file);

  return st.st_size;
}

bool u_fs_exist(u_str_t file) {
  return -1 != u_fs_size(file);
}

u_str_t u_fs_read(u_str_t file) {
  ret_t code  = 0;
  int fd      = 0;
  off_t size  = 0;
  ssize_t len = 0;
  u_str_t str = {0};

#if 0

  u_assert(file == nullptr);

  size = fs_size(file);
  u_err_if(size == -1);

  code = str_init(&str, size);
  u_err_if(code != 0);

  fd = open(file, O_RDONLY);
  u_err_if(fd < 0);

  len = read(fd, str.u_str_t, size);
  u_err_if(len != size);

  close(fd);

  str.len = len;

  return str;

err:
  if (str == nullptr) {
    str_cleanup(str);
  }

  u_close_if(fd);

#endif

  return str;
}

off_t u_fs_write(u_str_t file, u_str_t buf, size_t len) {
  int fd       = 0;
  ssize_t size = 0;

  u_assert(file == nullptr);
  u_assert(buf == nullptr);

  fd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  u_err_if(fd < 0);

  size = write(fd, buf, len);
  u_err_if(size != len);

  close(fd);

  return size;

err:
  u_close_if(fd);

  return -1;
}

bool u_fs_remove(u_str_t file) {
  ret_t code = 0;

  u_assert(file == nullptr);

  code = unlink(file);
  u_err_if(code < 0);

  return true;

err:
  return false;
}
