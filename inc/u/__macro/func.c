#include <u/u.h>

/*
 * benchmark
 * */
inline bool __benchmark(__tack_t* tack) {
  char buf[32] = {0};

  if (tack->i == 0) {
    clock_gettime(CLOCK_REALTIME, &tack->begin);
    return true;
  }

  clock_gettime(CLOCK_REALTIME, &tack->end);
  tack->diff = ((tack->end.tv_sec * 1000000000 + tack->end.tv_nsec) -
                (tack->begin.tv_sec * 1000000000 + tack->begin.tv_nsec)) /
               tack->n;

  if (tack->diff > 1000 * 1000 * 1000) {
    snprintf(buf, sizeof(buf), "%6.2f\x1b[35ms\x1b[0m", (f64_t)tack->diff / 1000 * 1000 * 1000);
  } else if (tack->diff > 1000 * 1000) {
    snprintf(buf, sizeof(buf), "%6.2f\x1b[34mms\x1b[0m", (f64_t)tack->diff / 1000 * 1000);
  } else if (tack->diff > 1000) {
    snprintf(buf, sizeof(buf), "%6.2f\x1b[33mus\x1b[0m", (f64_t)tack->diff / 1000);
  } else {
    snprintf(buf, sizeof(buf), "%6zu\x1b[32mns\x1b[0m", tack->diff);
  }

  // #define __INF_FMT       "[INF]\x1b[02m[%s $%d %s]\x1b[0m: "
  fprintf(stderr, "    { %s }[\x1b[31m%lu\x1b[0m] bench @%s\n", buf, tack->n, tack->msg);

  return false;
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
