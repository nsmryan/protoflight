# don't create a new shell for each command
.ONESHELL:
# remove files if they do not create successfully
.DELETE_ON_ERROR:
# do not allow undefined variables to be used
MAKEFLAGS += --warn-undefined-variables
# do not use build in rules- we will defined the rules we need
MAKEFLAGS += --no-builtin-rules


CC=gcc
CFLAGS=-Iinc
LDFLAGS=-lrt -lpthreads
BUILDDIR=build

build/test: $(shell find src test -type f -name *.c)
	mkdir -p $(BUILDDIR)
	$(CC) -o $@ $(CFLAGS) -Itest/unity/ $^ test/unity/*.c -DUNITY_FIXTURE_NO_EXTRAS

clean:
	rm -rf $(BUILDDIR)

