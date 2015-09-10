OPTIMIZATION?=-O0
DEBUG?=-g -ggdb -rdynamic
OBJS=unicorn.o mount.o util.o cgroup.o
CC=gcc

all: unicorn
	sudo mkdir -p /tmp/rootfs && sudo cp -r rootfs_base/* /tmp/rootfs

clean:
	rm -f *.o unicorn

install: all
	cp unicorn /usr/local/bin/

unicorn: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -Wall $(OPTIMIZATION) $(DEBUG) $(CFLAGS) $<

