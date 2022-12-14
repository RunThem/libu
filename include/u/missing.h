/*
 * Copyright (c) 2005-2012 by KoanLogic s.r.l. - All rights reserved.
 */

#ifndef _LIBU_MISSING_H_
#define _LIBU_MISSING_H_

#include <u/libu_conf.h>
#include <u/missing/daemon.h>
#include <u/missing/fnmatch.h>
#include <u/missing/strlcat.h>
#include <u/missing/strlcpy.h>
#include <u/missing/strsep.h>
#include <u/missing/strtok_r.h>
#include <u/missing/timegm.h>
#include <u/missing/va.h>

#ifndef HAVE_SSIZE_T
typedef int ssize_t;
#endif

/* on VxWorks/DCC there's not extern declaration (even if the var is available
   into library files) */
extern char* optarg;
extern int optind;

#endif /* !_LIBU_MISSING_H_ */
