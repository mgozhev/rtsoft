BOARD_KDIR =
HOST_KDIR = /lib/modules/$(shell uname -r)/build

BOARD_PARAM = ARCH=arm CROSS_COMPILE=arm-linux-gueabihf-
HOST_PARAM = 


KDIR := $(HOST_KDIR)
PARAM := $(HOST_PARAM)

obj-m = blackboard.o

all: modules clients

modules:
	make $(PARAM) -C $(KDIR) M=$(PWD) modules

clean:
	make $(PARAM) -C $(KDIR) M=$(PWD) clean
	rm -f read write

CC = gcc

cli: clients
clients: read write

read: read.c
	$(CC) -o $@ $^

write: write.c
	$(CC) -o $@ $^

