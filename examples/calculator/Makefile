

OBJS = $(SRCS:%.c=%.o)
SRCS = parser.c main.c 
HDRS = parser.h

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
