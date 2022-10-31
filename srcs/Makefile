# $Id: Makefile,v 1.22 2010/05/27 08:35:12 tho Exp $

include common.mk
include ../Makefile.conf

LIB = u

SRCS = $(wildcard toolbox/*.c)
SRCS += $(wildcard missing/*.c)

include lib.mk

# MacOSX ld(1) bug workaround
ifdef OS_DARWIN
install-hook-post:
	chmod u+w $(LIBDIR)/libu.a && $(RANLIB) $(LIBDIR)/libu.a
endif
