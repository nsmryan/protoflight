# don't create a new shell for each command
.ONESHELL:
# remove files if they do not create successfully
.DELETE_ON_ERROR:
# do not allow undefined variables to be used
MAKEFLAGS += --warn-undefined-variables
# do not use build in rules- we will defined the rules we need
MAKEFLAGS += --no-builtin-rules


CFLAGS=-Iinc
LDFLAGS=-lrt -pthread -L/usr/lib/x86_64-linux-gnu/
BUILDDIR=build


build/test: $(shell find src test -type f -name *.c)
	mkdir -p $(BUILDDIR)
	$(CC) -static -o $@ $^ -Itest/unity/ -DUNITY_FIXTURE_NO_EXTRAS -DFSW_UNIT_TEST $(CFLAGS) ${LDFLAGS} 

clean:
	rm -rf $(BUILDDIR)

