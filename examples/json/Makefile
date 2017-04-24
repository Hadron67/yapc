

OBJS = $(SRCS:%.c=%.o)
SRCS = matcher.c parser.c main.c json.c json_match.c
HDRS = parser.h json.h matcher.h json_match.h

CFLAGS = -g -ggdb -fvisibility=hidden -Wall
LDFLAGS = -lm
all:main

main:$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) 

%.o: %.c $(HDRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

parser.c: parser.y
	../../yapc $<
	
matcher.c: matcher.y
	../../yapc $<

.PHONY: clean rebuild

clean:
	$(RM) *.o main parser.c parser.h parser.output matcher.c matcher.h matcher.output
