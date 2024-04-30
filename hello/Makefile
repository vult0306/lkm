obj-m += hello1.o hello2.o hello5.o
obj-m += hello6.o
hello6-objs := hello6_start.o hello6_stop.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean