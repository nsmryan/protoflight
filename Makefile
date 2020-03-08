# don't create a new shell for each command
.ONESHELL:
# remove files if they do not create successfully
.DELETE_ON_ERROR:
# do not allow undefined variables to be used
MAKEFLAGS += --warn-undefined-variables
# do not use build in rules- we will defined the rules we need
MAKEFLAGS += --no-builtin-rules


# default OS option is posix
OS ?= posix

CFLAGS+=-Ios/$(OS)/ -Ifsw/inc/ -Ios/inc/ -Wall -Werror -O0 -g3
ifeq ($(OS), posix)
		LDFLAGS=-lrt -pthread -L/usr/lib/x86_64-linux-gnu/
endif

OS_WSL ?= 0
ifeq ($(OS_WSL), 1)
	CFLAGS+=-DOS_WSL
endif

unit_test: test/test.c os/os_test.c $(shell find test/unity os/${OS}/src/ -type f -name *.c)
	$(CC) -DFSW_UNIT_TEST -g -o $@ $^ -Itest/unity/ -DUNITY_FIXTURE_NO_EXTRAS $(CFLAGS) ${LDFLAGS} 

clean:
	rm -rf unit_test

