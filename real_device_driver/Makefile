ARCH ?= arm
CROSS_COMPILE ?= /opt/fslc-x11/4.0/sysroots/x86_64-fslcsdk-linux/usr/bin/arm-fslc-linux-gnueabi/arm-fslc-linux-gnueabi-
KERNELDIR ?= /opt/fslc-x11/4.0/sysroots/cortexa7t2hf-neon-fslc-linux-gnueabi/lib/modules/5.15.71-imx6ul+g5ebe23fd1585/

PWD := $(shell pwd)

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR)/build M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR)/build M=$(PWD) clean

obj-m += real_device_driver.o