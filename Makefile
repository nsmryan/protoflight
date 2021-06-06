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

CFLAGS += -O0 -g3
CFLAGS += -Ios/$(OS)/ -Ifsw/inc/ -Ios/inc/ -Wall -Werror -Wextra

TEST_CFLAGS += $(CFLAGS)
TEST_CFLAGS += -Itest/unity -DFSW_UNIT_TEST -DUNITY_FIXTURE_NO_EXTRAS 

ifeq ($(OS), posix)
	LDFLAGS=-lrt -pthread -L/usr/lib/x86_64-linux-gnu/
endif

ifeq ($(OS), tinycthread)
	CFLAGS+=lib/tinycthread/tinycthread.c -Ilib/tinycthread/
endif

OS_WSL ?= 0
ifeq ($(OS_WSL), 1)
	CFLAGS+=-DOS_WSL
endif

LDLIBS += -lrt

# OS source files
OS_SRC := os/$(OS)/src/os_mutex.c os/$(OS)/src/os_queue_portable.c os/$(OS)/src/os_sem.c os/$(OS)/src/os_task.c os/$(OS)/src/os_time.c os/$(OS)/src/os_timer.c

# if using WSL, use the portable queue as librt is not supported
ifeq ($(OS_WSL), 1)
OS_SRC += os/$(OS)/src/os_queue_portable.c
else
OS_SRC += os/$(OS)/src/os_queue.c
endif


SRC := $(OS_SRC)

TEST_SRC := $(SRC) test/test.c os/os_test.c test/unity/unity.c test/unity/unity_fixture.c

protoflight: $(shell find fsw/ os/$(OS)/src -type f -name *.c)
	$(CC) $(CFLAGS) ${LDFLAGS} -o $@ $^ $(LDLIBS)

unit_test: $(TEST_SRC) $(shell find test/unity os/${OS}/src/ -type f -name *.c)
	$(CC) ${LDFLAGS} $(TEST_CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf unit_test

