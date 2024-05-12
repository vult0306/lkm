/***************************************************************************//**
*  \file       test_app.c
*
*  \details    Userspace application to test the Device driver
*
*  \author     EmbeTronicX
*
*  \Tested with Linux raspberrypi 5.10.27-v7l-embetronicx-custom+
*
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int main()
{
        int fd;
        int32_t value, number;
        printf("*********************************\n");
        printf("*******WWW.EmbeTronicX.com*******\n");

        printf("\n Openning Driver\n");
        fd = open("/dev/etx_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }

        printf("Enter the value to send to driver :");
        scanf("%d", (int32_t*)&number);
        printf("Data Writing ...");
        ioctl(fd, WR_VALUE, (int32_t*)&number);
        printf("Data Reading ...");
        ioctl(fd, RD_VALUE, (int32_t*)&value);
        printf("Done!\n\n");
        printf("value = %d\n\n", value);
        printf("Closing Driver\n");
        close(fd);
}