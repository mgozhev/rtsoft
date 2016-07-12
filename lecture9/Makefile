#KDIR := /home/user/ti-processor-sdk-linux-am335x-evm-02.00.02.11/board-support/linux-4.1.18+gitAUTOINC+bbe8cfc1da-gbbe8cfc
#CROSS := ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

KDIR := /lib/modules/$(shell uname -r)/build

obj-m := saw.o

all:
	make $(CROSS) -C $(KDIR) M=$(PWD) modules

clean:
	make $(CROSS) -C $(KDIR) M=$(PWD) clean
