

SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)
OBJS = $(SRCS:%.c=%.o)

CFLAGS = -g -ggdb -fvisibility=hidden -Wall
LDFLAGS = -lm
all:main

main:$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) 

%.o: %.c $(HDRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

parser.c: parser.y
	../../yapc $<

.PHONY: clean rebuild

clean:
	$(RM) *.o main parser.c parser.h parser.output