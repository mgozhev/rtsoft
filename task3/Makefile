BOARD_KDIR =
HOST_KDIR = /lib/modules/$(shell uname -r)/build

BOARD_PARAM = ARCH=arm CROSS_COMPILE=arm-linux-gueabihf-
HOST_PARAM = 


KDIR := $(HOST_KDIR)
PARAM := $(HOST_PARAM)

obj-m = blackboard.o

modules:
	make $(PARAM) -C $(KDIR) M=$(PWD) modules

clean:
	make $(PARAM) -C $(KDIR) M=$(PWD) clean

CC = gcc

cli: clients
clients: read write

read: read.c
	$(CC) -o $@ $^

write: write.c
	$(CC) -o $@ $^

