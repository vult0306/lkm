**Build Yocto image for i.MX6UL device and write kernel module**

Product: https://variwiki.com/index.php?title=VAR-SOM-6UL

![image](https://github.com/vult0306/lkm/assets/19576509/7860341c-39a5-48b7-a9f7-bd58d82cde13)

**1. Setup Docker**
**Clone Variscite's Docker container repository:**

```$ git clone https://github.com/varigit/var-host-docker-containers.git ~/var-host-docker-containers```

**Create folder in host(ubuntu) machine**

```$ mkdir ~/var-fslc-yocto```

**Starting a container**

```
$ cd ~/var-host-docker-containers
$ ./run.sh -p -u 20.04 -w ~/docker-yocto-build -v ~/var-fslc-yocto

vari@abe078d0f67c:/workdir$ grep DESCRIPTION /etc/lsb-release
DISTRIB_DESCRIPTION="Ubuntu 20.04.2 LTS"
```

**2. Build Yocto image and generate SDK**

**Download the latest revision**

```
$ repo init -u https://github.com/varigit/variscite-bsp-platform.git -b kirkstone -m kirkstone-5.15.71-2.2.0.xml
$ repo sync -j$(nproc)
```

**Build X11 GUI demo image**

```
$ MACHINE=imx6ul-var-dart DISTRO=fslc-x11 . setup-environment build_x11
```

**The above command is only mandatory for the very first build setup: whenever restarting a newer build session (from a different terminal or in a different time), you can skip the full setup and just run**

```
$ source setup-environment build_x11
```

**Use systemd instead of SysV init**

**Append the following to the conf/local.conf file in your Yocto build directory, to use systemd instead of SysV init in your images:**

```
DISTRO_FEATURES:append = " virtualization"
DISTRO_FEATURES:append = " systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED:append = " sysvinit"
VIRTUAL-RUNTIME_init_manager = "systemd"
VIRTUAL-RUNTIME_initscripts = ""
IMX_DEFAULT_DISTRO_FEATURES:append = " systemd"
```

**Append the following to the conf/local.conf file in your Yocto build directory, to add Eclipse debug support to your images:**

```
EXTRA_IMAGE_FEATURES = " \
    eclipse-debug \
    ssh-server-openssh \
    "
```

**Append the following to the conf/local.conf file in your Yocto build directory, to generate kernel header for kernel module compile:**

```
TOOLCHAIN_TARGET_TASK:append = " kernel-devsrc"
```

**Execute bitbake command to generate the Yocto image including the SDK (take couple hours)**

```
$  bitbake -c populate_sdk fsl-image-gui
```

**When the build step is done**

```
image:
~/var-fslc-yocto/build_x11/tmp/deploy/images/imx6ul-var-dart/fsl-image-gui-imx6ul-var-dart-20240505161420.rootfs.wic.gz
sdk:
~/var-fslc-yocto/build_x11/tmp/deploy/sdk/fslc-x11-glibc-x86_64-fsl-image-gui-cortexa7t2hf-neon-imx6ul-var-dart-toolchain-4.0.sh
```

**3. Flash image to SD-card**

```
$ zcat tmp/deploy/images/imx6ul-var-dart/fsl-image-gui-imx6ul-var-dart.wic.gz | sudo dd of=/dev/sdX bs=1M && sync
```

**Bootup device and connect via USB OTG Etherne**

```
$ modprob g_ether
$ ifconfig usb0 192.168.6.1
```

![image](https://github.com/vult0306/lkm/assets/19576509/e3bd1055-d7f2-4e76-b733-1fa3c5d1e96c)

**Config your host(ubuntu) ip to 192.168.6.100**

![image](https://github.com/vult0306/lkm/assets/19576509/cee1d467-ffd0-4b10-bed2-63a658bfba1a)

**4. Install SDK to host(ubuntu) machine**

**Execute command to install SDK:**

```
~/var-fslc-yocto/build_x11/tmp/deploy/sdk/fslc-x11-glibc-x86_64-fsl-image-gui-cortexa7t2hf-neon-imx6ul-var-dart-toolchain-4.0.sh
```

![image](https://github.com/vult0306/lkm/assets/19576509/f000710f-51f7-4f2e-b447-b94a06fa2593)

**The script will install the SDK to:**

```
/opt/fslc-x11/4.0
```

**Build the Kernel header for kernel module development**

```
$ cd /opt/fslc-x11/4.0/sysroots/cortexa7t2hf-neon-fslc-linux-gnueabi/lib/modules/5.15.71-imx6ul+g5ebe23fd1585/build
```

**Copy the config folder from this repo for pre-config of kernel**

```
https://github.com/nxp-imx/linux-imx/tree/git.kernel.org/linux-stable/linux-5.15.y/arch/arm/configs
$ sudo cp -r configs /opt/test-fslc-x11/4.0/sysroots/cortexa7t2hf-neon-fslc-linux-gnueabi/lib/modules/5.15.71-imx6ul+g5ebe23fd1585/build/arch/arm/.
```

**Build the kernel:**

```
$ pwd
/opt/fslc-x11/4.0/sysroots/cortexa7t2hf-neon-fslc-linux-gnueabi/lib/modules/5.15.71-imx6ul+g5ebe23fd1585
$ sudo make ARCH=arm CROSS_COMPILE=/opt/fslc-x11/4.0/sysroots/x86_64-fslcsdk-linux/usr/bin/arm-fslc-linux-gnueabi/arm-fslc-linux-gnueabi- imx_v7_defconfig
```
![image](https://github.com/vult0306/lkm/assets/19576509/129df9d4-9a7c-4d45-b849-33f5024ca310)

```
$ sudo make ARCH=arm CROSS_COMPILE=/opt/fslc-x11/4.0/sysroots/x86_64-fslcsdk-linux/usr/bin/arm-fslc-linux-gnueabi/arm-fslc-linux-gnueabi- scripts
```

![image](https://github.com/vult0306/lkm/assets/19576509/fab4b569-52a8-43f5-a79c-d82df637577d)

```
$ sudo make ARCH=arm CROSS_COMPILE=/opt/fslc-x11/4.0/sysroots/x86_64-fslcsdk-linux/usr/bin/arm-fslc-linux-gnueabi/arm-fslc-linux-gnueabi- prepare
```
![image](https://github.com/vult0306/lkm/assets/19576509/5a89d368-2f12-4200-88b6-26383209a0d1)


**5. Build Kernel Module**

```
$ cd hello
$ make
```

**6. copy to target (imx)**

```
$ scp hello1.ko hello2.ko hello5.ko root@192.168.6.1:/home/root/.
```

**7. insert the moodule**

```
$ insmod hello1.ko
```

**check the module:**

```
$ dmesg
```

![image](https://github.com/vult0306/lkm/assets/19576509/718805e4-e925-4c30-ab3f-72ecc039e121)

