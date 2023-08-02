#pragma once

#include "str.h"

off_t fs_size(c_str filename);

bool fs_exists(c_str filename);

str_t fs_read(c_str filename);

off_t fs_write(c_str filename, c_str buf, size_t len);

bool fs_remove(c_str filename);

