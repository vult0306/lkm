PWD := $(shell pwd)

ifndef ARCH
	KERNELDIR ?= /lib/modules/$(shell uname -r)/
else
	ARCH ?= arm
	CROSS_COMPILE ?= /opt/fslc-x11/4.0/sysroots/x86_64-fslcsdk-linux/usr/bin/arm-fslc-linux-gnueabi/arm-fslc-linux-gnueabi-
	KERNELDIR ?= /opt/fslc-x11/4.0/sysroots/cortexa7t2hf-neon-fslc-linux-gnueabi/lib/modules/5.15.71-imx6ul+g5ebe23fd1585/
endif

all:
ifndef ARCH
	make -C $(KERNELDIR)/build M=$(PWD) modules
else
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR)/build M=$(PWD) modules
endif

clean:
ifndef ARCH
	make -C $(KERNELDIR)/build  M=$(PWD) clean
else
	$(MAKE) -C $(KERNELDIR)/build M=$(PWD) clean
endif

obj-m += hello1.o hello2.o hello5.o
obj-m += hello6.o
hello6-objs := hello6_start.o hello6_stop.o