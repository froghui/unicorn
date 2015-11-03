OPTIMIZATION?=-O0
DEBUG?=-g -ggdb -rdynamic
OBJS=unicorn.o mount.o util.o cgroup.o net.o
CC=gcc

all: unicorn

init: 
	bash rootfs_base.sh && bash bridge.sh

clean:
	rm -f *.o unicorn

install: all
	cp unicorn /usr/local/bin/

unicorn: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -Wall $(OPTIMIZATION) $(DEBUG) $(CFLAGS) $<

