BOARD_KDIR =
HOST_KDIR = /lib/modules/$(shell uname -r)/build

BOARD_PARAM = ARCH=arm CROSS_COMPILE=arm-linux-gueabihf-
HOST_PARAM = 

obj-m = blackboard.o

all: module_host


clean: module_host_clean


module_host:
	make $(HOST_PARAM) -C $(HOST_KDIR) M=$(PWD) modules

module_host_clean:
	make $(HOST_PARAM) -C $(HOST_KDIR) M=$(PWD) clean

module_board:
	make $(BOARD_PARAM) -C $(BOARD_KDIR) M=$(PWD) modules

module_board_clean:
	make $(BOARD_PARAM) -C $(BOARD_KDIR) M=$(PWD) clean

CC = gcc

cli: clients
clients: read write

read: read.c
	$(CC) -o $@ $^

write: write.c
	$(CC) -o $@ $^

