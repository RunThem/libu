#include <cstddef>

template <auto> struct include { };
struct T {
  struct std::nullptr_t h;
} inline constexpr stdio{nullptr};

include<stdio.h>
