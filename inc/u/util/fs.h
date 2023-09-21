#pragma once

#include <u/core/str.h>

off_t u_fs_size(u_str_t file);

bool u_fs_exist(u_str_t file);

u_str_t u_fs_read(u_str_t file);

off_t u_fs_write(u_str_t file, u_str_t buf, size_t len);

bool u_fs_remove(u_str_t file);
