#pragma once

#include "map.c"
#include "u.h"

/*
 * map 结构布局, next 是指向下一个节点的指针, 但无法递归的描述该节点类型, 所以使用万能指针
 * */
#define map(K, V)                                                                                  \
  struct {                                                                                         \
    map_hash_t hash_func;                                                                          \
    size_t len;                                                                                    \
    size_t _cap;                                                                                   \
    struct {                                                                                       \
      hash_t hash;                                                                                 \
      K key;                                                                                       \
      V value;                                                                                     \
                                                                                                   \
      void* next;                                                                                  \
    } nodes[0];                                                                                    \
  }*

#undef _
#define _(m) (*(m))

/*
 * map 节点类型
 * */
#define map_T(m) typeof(_(m)->nodes[0])

/*
 * map 健类型
 * */
#define map_K(m) typeof(_(m)->nodes[0].key)

/*
 * map 值类型
 * */
#define map_V(m) typeof(_(m)->nodes[0].value)

typedef uint64_t hash_t;
typedef hash_t (*map_hash_t)(const uint8_t*, size_t);

/* fnv 64 bit hash function */
hash_t map_mem_hash(const uint8_t* ptr, size_t len) {
  hash_t hash = 14695981039346656037U;
  for (size_t i = 0; i < len; ++i) {
    hash *= 1099511628211U;
    hash ^= (uint64_t)ptr[i];
  }

  return hash;
}

/*
 * 整型数据hash函数
 * */
hash_t map_int_hash(const uint8_t* ptr, size_t len) {
  hash_t hash = 0;

  if (len == 1) {
    hash = (hash_t)(*(uint8_t*)ptr);
  } else if (len == 2) {
    hash = (hash_t)(*(uint16_t*)ptr);
  } else if (len == 4) {
    hash = (hash_t)(*(uint32_t*)ptr);
  } else if (len == 8) {
    hash = (hash_t)(*(uint64_t*)ptr);
  } else {
    hash = (hash_t)ptr;
  }

  hash = (hash ^ (hash >> 31) ^ (hash >> 62)) * UINT64_C(0x319642b2d24d8ec3);
  hash = (hash ^ (hash >> 27) ^ (hash >> 54)) * UINT64_C(0x96de1b173f119089);
  hash = hash ^ (hash >> 30) ^ (hash >> 60);

  return hash;
}

/*
 * map 节点大小
 * */
#define map_itsize(map) ({ sizeof(map_T(map)); })

/*
 * 初始化map, 并设置桶个数
 * */
#define map_init(map, arg...) __map_init(map, arg)

/*
 * 清除map, 释放所有节点
 * */
#define map_clear(map) __map_clear(map)

/*
 * 释放map, 释放所有节点
 * */
#define map_cleanup(map) __map_cleanup(map)

/*
 * 添加元素
 *
 * code:
 *    map(int, char*) m;
 *
 *    map_init(&m);
 *
 *    map_push(1, "one");
 *    map_push(2, "two");
 *
 *    // m -> (2) { { 1: "one" }, { 2: "two" } }
 * */
#define map_push(map, _key, _value) __map_push(map, _key, _value)

/*
 * 删除元素
 *
 * code:
 *    map(int, c_str) m;
 *
 *    map_init(&m);
 *
 *    map_push(&m, 1, "one");
 *    map_push(&m, 2, "two");
 *
 *    auto v = map_pop(&m, 1);
 *
 *    assert(1 == v.key);
 *    assert(!strncmp("one", v.value, 3));
 *
 *    // m -> (2) { { 2: "two" } }
 * */
#define map_pop(map, _key) __map_pop(map, _key)
