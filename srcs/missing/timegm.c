/* $Id: timegm.c,v 1.1 2006/11/20 13:38:01 tho Exp $ */

#include <time.h>
#include <u/missing/timegm.h>

#ifndef HAVE_TIMEGM

#  include <stdio.h>
#  include <stdlib.h>

time_t timegm(struct tm* tm) {
  time_t ret;
  char* tz;

  /* save current timezone and set UTC */
  tz = getenv("TZ");
  putenv("TZ=UTC"); /* use Coordinated Universal Time (i.e. zero offset) */
  tzset();

  ret = mktime(tm);
  if (tz) {
    char buf[256];
    snprintf(buf, sizeof(buf), "TZ=%s", tz);
    putenv(buf);
  } else {
    putenv("TZ=");
  }
  tzset();

  return ret;
}

#else
time_t timegm(struct tm*);
#endif
