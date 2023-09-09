#pragma once

#include "str.h"

off_t fs_size(c_str file);

bool fs_exists(c_str file);

str_t fs_read(c_str file);

off_t fs_write(c_str file, c_str buf, size_t len);

bool fs_remove(c_str file);
