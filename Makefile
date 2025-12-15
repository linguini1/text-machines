CC = gcc
CFLAGS = -Wall -Wextra

INCLUDES = -I common

OUT = tmac

CMAIN = main.c
CSRCS = $(wildcard machines/*.c)
CSRCS += $(wildcard common/*.c)

OBJS = $(patsubst %.c,%.o,$(CSRCS))
MAINOBJ = $(patsubst %.c,%.o,$(CMAIN))

TESTSRCS = $(wildcard tests/*.c)
TESTS = $(patsubst %.c,%,$(TESTSRCS))
TESTCASES = $(patsubst %,%.test,$(TESTS))

WORKDIR = $(realpath .)

# Recipe for making the output binary
all: $(OUT)

# Recipe for running all test cases
check: clean $(TESTCASES)

# Recipe for running a test binary
%.test: %
	@$(WORKDIR)/$<

# Recipe for creating a single test binary
$(TESTS): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@.o -c $@.c
	@$(CC) $(OBJS) $@.o -o $@

# Create the main output binary for quick testing
$(OUT): $(OBJS) $(MAINOBJ)
	$(CC) $(OBJS) $(MAINOBJ) -o $(OUT)

# Compile a C file
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@$(RM) $(OBJS)
	@$(RM) $(MAINOBJ)
	@$(RM) $(OUT)
	@$(RM) $(TESTS)
	@$(RM) tests/*.o
