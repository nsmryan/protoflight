# don't create a new shell for each command
.ONESHELL:
# remove files if they do not create successfully
.DELETE_ON_ERROR:
# do not allow undefined variables to be used
MAKEFLAGS += --warn-undefined-variables
# do not use build in rules- we will defined the rules we need
MAKEFLAGS += --no-builtin-rules

# build directory to place output files
BUILD ?= build

# OS options:
# posix - general POSIX, using librt unless on WSL.
# tinypthread - C11 implemented through the tinypthread library, for testing only.
OS ?= posix

CFLAGS += -O0 -g3
CFLAGS += -Ios/$(OS)/ -Ifsw/inc/ -Ios/inc/ -Wall -Werror -Wextra

TEST_CFLAGS += $(CFLAGS)
TEST_CFLAGS += -Itest/unity -DFSW_UNIT_TEST -DUNITY_FIXTURE_NO_EXTRAS 

ifeq ($(OS), posix)
	LDFLAGS=-lrt -pthread -L/usr/lib/x86_64-linux-gnu/
	# NOTE is this needed for MSYS2?
	#LDFLAGS += -L/usr/lib/x86_64-linux-gnu/
endif

ifeq ($(OS), tinycthread)
	CFLAGS+=lib/tinycthread/tinycthread.c -Ilib/tinycthread/
endif

LDLIBS += -lrt

# OS source files
OS_SRC := os_mutex.c os_sem.c os_task.c os_time.c os_timer.c

# WSL detection:
# This relies on the fact that ?= will define the variable if is does not
# exist, and ifndef will trigger if the variable doesn't exist *or* it
# has an empty value. WSL will define this empty, triggering this path.
WSLENV ?= 0
ifndef WSLENV
	# if using WSL, use the portable queue as librt is not supported.
	OS_SRC += os_queue_portable.c
	# Also add the OS_WSL flag to build.
	CFLAGS+=-DOS_WSL=1
else
	OS_SRC += os_queue.c
endif


FSW_SRC := em.c fsw.c mb.c msg.c tlm.c tm.c
SRC := $(OS_SRC) $(FSW_SRC) protoflight.c

TEST_SRC := $(OS_SRC) $(FSW_SRC) os_test.c mb_test.c msg_test.c em_test.c unity.c unity_fixture.c test.c

OBJS := $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(notdir $(SRC)))))
TEST_OBJS := $(addprefix $(BUILD)/, $(addsuffix .to, $(basename $(notdir $(TEST_SRC)))))

VPATH := fsw/src/em fsw/src/fsw fsw/src/mb fsw/src/msg fsw/src/tlm fsw/src/tm os/$(OS)/src os/$(OS) os test test/unity

.PHONY: all protoflight test sloc run tags

all: $(BUILD)/protoflight $(BUILD)/unit_test

protoflight: $(BUILD)/protoflight

test: $(BUILD)/unit_test
	$(BUILD)/unit_test

sloc: $(SRC)
	cloc $^ --by-file

tags:
	@ctags -R --fields=+iaS --extra=+q

run: protoflight
	@$(BUILD)/protoflight

$(BUILD)/protoflight: $(OBJS) | $(BUILD)
	$(CC) $(CFLAGS) ${LDFLAGS} -o $@ $^ $(LDLIBS)

$(BUILD)/unit_test: $(TEST_OBJS) | $(BUILD)
	$(CC) ${LDFLAGS} $(TEST_CFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) ${LDFLAGS} -c -o $@ $^ $(LDLIBS)

$(BUILD)/%.to: %.c | $(BUILD)
	$(CC) $(TEST_CFLAGS) ${LDFLAGS} -c -o $@ $^ $(LDLIBS)

$(BUILD):
	-@mkdir -p build

clean:
	rm -rf build

