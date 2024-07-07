/* MIT License
 *
 * Copyright (c) 2023 RunThem <iccy.fun@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * */

#include <u/u.h>

#define FMT "%s %s%s\x1b[0m \x1b[02m%s:%d\x1b[0m"

static const char* dbg_level[] = {"ERR", "WAR", "INF", "DBG", " U "};
static const char* dbg_color[] = {"\x1b[31m", "\x1b[33m", "\x1b[34m", "\x1b[35m", "\x1b[32m"};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
void dbg_write(int level, const char* file, int line, const char* fmt, ...) {
  char protmp[256] = {};
  char buf[4096]   = {};
  char timebuf[64] = {};
  char errbuf[128] = {};
  time_t t         = {};
  struct tm* tm    = {};
  va_list ap       = {};
  error_t err      = {};

  err = errno;

  t  = time(nullptr);
  tm = localtime(&t);

  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  timebuf[strftime(timebuf, sizeof(timebuf), "%H:%M:%S", tm)] = '\0';

  snprintf(protmp, sizeof(protmp), FMT, timebuf, dbg_color[level], dbg_level[level], file, line);

  if (err == 0) {
    fprintf(stderr, "%s : %s", protmp, buf);
  } else {
    fprintf(stderr, "%s {%s} : %s", protmp, strerror(err), buf);
  }
}
