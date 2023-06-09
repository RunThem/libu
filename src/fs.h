#pragma once

#include "str.h"
#include "u.h"

/*
 * 返回文件大小, 失败返回-1, 必须是文件, 而不能是目录
 * */
off_t fs_size(c_str filename);

/*
 * 判断文件是否存在
 * */
bool fs_exists(c_str filename);

/*
 * 读出整个文件, 需要自行释放内存
 * */
str_t fs_read(c_str filename);

/*
 * 向一个文件中写入, 若不存在则创建, 反之追加文件
 * */
off_t fs_write(c_str filename, c_str buf, size_t len);

/*
 * 删除一个文件, 必须是文件, 而不能是目录
 * */
bool fs_remove(c_str filename);
