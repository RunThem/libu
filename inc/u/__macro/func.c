#include <u/u.h>

/*
 * format time string
 * */
thread_local char time_fmt_buf[12] = {0};

#define time_fmt_h()  ((u64_t)60 * 60 * 1000 * 1000 * 1000)
#define time_fmt_m()  ((u64_t)60 * 1000 * 1000 * 1000)
#define time_fmt_s()  ((u64_t)1000 * 1000 * 1000)
#define time_fmt_ms() ((u64_t)1000 * 1000)
#define time_fmt_us() ((u64_t)1000)

char* time_fmt(u64_t t) {
  if (t > time_fmt_h()) {
    snprintf(time_fmt_buf, sizeof(time_fmt_buf), "%6.2f h", (f64_t)t / time_fmt_h());
  } else if (t > time_fmt_m()) {
    snprintf(time_fmt_buf, sizeof(time_fmt_buf), "%6.2f m", (f64_t)t / time_fmt_m());
  } else if (t > time_fmt_s()) {
    snprintf(time_fmt_buf, sizeof(time_fmt_buf), "%6.2f s", (f64_t)t / time_fmt_s());
  } else if (t > time_fmt_ms()) {
    snprintf(time_fmt_buf, sizeof(time_fmt_buf), "%6.2f ms", (f64_t)t / time_fmt_ms());
  } else if (t > time_fmt_us()) {
    snprintf(time_fmt_buf, sizeof(time_fmt_buf), "%6.2f us", (f64_t)t / time_fmt_us());
  } else {
    snprintf(time_fmt_buf, sizeof(time_fmt_buf), "%6zu ns", t);
  }

  return time_fmt_buf;
}

/*
 * format print
 * */
void __inf_hex(const u8_t* arr, size_t size) {
  char buf[17] = {0};
  size_t i     = 0;
  size_t pos   = 0;
  size_t idx   = 0;

  for (; pos < size; pos++) {
    if ((pos % 16) == 0) {
      print("   %08lx: ", pos);
    }

    print("%02x %s", arr[pos] & 0xff, (pos % 16) == 7 ? " " : "");
    if ((pos % 16) == 15) {
      for (i = 0; i < arr_len(buf); i++) {
        idx    = pos - 16 + i;
        buf[i] = isprint(arr[idx]) ? arr[idx] : '.';
      }

      buf[i - 1] = '\0';
      println("%s", buf);
    }
  }

  if ((pos % 16) != 0) {
    for (i = 0; i < pos % 16 + 2; i++) {
      idx    = pos - (pos % 16) + i - 1;
      buf[i] = isprint(arr[idx]) ? arr[idx] : '.';
    }

    buf[i - 1] = '\0';
    println("%*s", 47 - 3 * ((int)pos % 16) + (int)i, buf);
  }
}

void __inf_bit(const u8_t* buf, size_t size) {
  u8_t byte = 0;

  for (ssize_t i = as(size, ssize_t) - 1; i >= 0; i--) {
    byte = buf[i];

    print("    %u%u%u%u%u%u%u%u",
          bit(byte, 7),
          bit(byte, 6),
          bit(byte, 7),
          bit(byte, 4),
          bit(byte, 3),
          bit(byte, 2),
          bit(byte, 1),
          bit(byte, 0));

    if (i != 0 && (i + 1) % 2) {
      print("\n");
    } else {
      print(" ");
    }
  }

  println();
}
