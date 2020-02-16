# don't create a new shell for each command
.ONESHELL:
# remove files if they do not create successfully
.DELETE_ON_ERROR:
# do not allow undefined variables to be used
MAKEFLAGS += --warn-undefined-variables
# do not use build in rules- we will defined the rules we need
MAKEFLAGS += --no-builtin-rules


CFLAGS+=-Ios/$(OS)/inc/ -Ifsw/inc/ -Wall -Werror -O0 -g3
LDFLAGS=-lrt -pthread -L/usr/lib/x86_64-linux-gnu/
BUILDDIR=build

# options are posix
OS ?= posix

OS_PTHREAD ?= 0
ifeq ($(OS_PTHREAD), 1)
	CFLAGS+=-DOS_QUEUE_PTHREAD
endif

build/test: $(shell find os/$(OS)/src test -type f -name *.c)
	mkdir -p $(BUILDDIR)
	$(CC) -static -g -o $@ $^ -Itest/unity/ -DUNITY_FIXTURE_NO_EXTRAS -DFSW_UNIT_TEST $(CFLAGS) ${LDFLAGS} 

clean:
	rm -rf $(BUILDDIR)

