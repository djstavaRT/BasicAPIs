########################################################
#                                                      #
#       Filename: Makefile                             #
#       Description:                                   #
#       Author: djstava , djstava@gmail.com            #
#       Company: www.sany-tech.com                     #
#                                                      #
########################################################

CORSS_COMPILE = 
                          
CC = $(CORSS_COMPILE)gcc
CPP = $(CORSS_COMPILE)g++
AR = $(CORSS_COMPILE)ar
STRIP = $(CORSS_COMPILE)strip
 
PRODIR = .
PROSRCDIR = $(PRODIR)/src
PROINCDIR = $(PRODIR)/include
PROBUILDDIR = $(PRODIR)/build
PROLIBDIR = $(PROBUILDDIR)/lib
PROOBJDIR = $(PROBUILDDIR)/obj

EXTRALIBDIR = $(PRODIR)/lib/linux

#CFLAGS = -I$(PROINCDIR) -Wall -O2 `pkg-config --cflags glib-2.0`
CFLAGS = -I$(PROINCDIR) -Wall -O2 -w 
LIBCFLAGS = $(CFLAGS) -D_REENTRANT -fPIC
#CFLAGS += -L$(EXTRALIBDIR) -lcurl 
LDFLAGS = -L$(EXTRALIBDIR) -lcurl 
LIBSRCS := $(wildcard src/*.c)
LIBOBJS := $(patsubst %.c,%.o,$(LIBSRCS))
LIBRARY = $(PROLIBDIR)/libdjstava.so

all:$(LIBRARY)

$(LIBRARY):$(LIBOBJS)
	$(CC) -shared $(LDFLAGS) $(LIBOBJS) -o $@
$(LIBOBJS):%.o:%.c
	$(CC) $(LIBCFLAGS) -c $< -o $@

.PHONY:clean distclean
clean:
	@echo "Start clean ..."
	-rm -f $(LIBOBJS) $(LIBRARY)

distclean:
	@echo "Start distclean ..."
	-rm -f $(LIBOBJS) $(LIBRARY)
