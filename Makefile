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

WORKDIR = $(realpath .)

all: $(OUT)

run: $(OUT)
	./$(OUT) $(WORKDIR)/examples/pig.txt

test: clean $(TESTS)
	$(foreach t,$(TESTS),$(WORKDIR)/$t)

$(TESTS): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@.o -c $@.c
	$(CC) $(OBJS) $@.o -o $@

$(OUT): $(OBJS) $(MAINOBJ)
	$(CC) $(OBJS) $(MAINOBJ) -o $(OUT)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@$(RM) $(OBJS)
	@$(RM) $(MAINOBJ)
	@$(RM) $(OUT)
	@$(RM) $(TESTS)
	@$(RM) tests/*.o
