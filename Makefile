CC = gcc
CFLAGS = -Wall -Wextra

CSRCS = main.c
CSRCS += $(wildcard machines/*.c)
CSRCS += $(wildcard common/*.c)

INCLUDES = -I common

OBJS = $(patsubst %.c,%.o,$(CSRCS))

OUT = tmac

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@rm $(OBJS)
	@rm $(OUT)
