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
PROINCDIR = $(PRODIR)/../../include
PROBUILDDIR = $(PRODIR)/../../build
PROLIBDIR = $(PROBUILDDIR)/lib

EXTRALIB = $(PRODIR)/../../lib/linux

APPNAME = sample_curl
LDFLAGS = -L$(PROLIBDIR) -ldjstava -L$(EXTRALIB) -lcurl 
CFLAGS = -I$(PROINCDIR) -Wall -O2
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all:$(APPNAME)
$(APPNAME):$(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(APPNAME)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	@echo "Start clean ..."
	-rm -f $(OBJS) $(APPNAME)
