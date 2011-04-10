#include <stdio.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define ADDR_POT1   0x2C
#define ADDR_POT2   0x2E
#define CMD_WRITE   0x00

int main() {
    int file;
    char *filename = "/dev/i2c-2";

    uint8_t buf[2] = {0};
    uint8_t temp = 0;

    if((file = open(filename, O_RDWR)) < 0) {
        perror("Could not open i2c-2");
        exit(1);
    }

        while(1) {
        int t;
        printf("Enter a byte (0-255) to send to both pots: ");
        scanf("%d", &t);
        temp = t;
        buf[1] = temp;
        buf[0] = CMD_WRITE;
        if(ioctl(file, I2C_SLAVE, ADDR_POT1) < 0) {

            perror("Failed to talk to slave pot1\n");
            exit(1);
        }

        if(write(file,buf,2) != 2) {
            printf("Failed to write to the i2c bus: ");
            printf(strerror(errno));
            printf("\n");
        }
        if(ioctl(file, I2C_SLAVE, ADDR_POT2) < 0) {
            perror("Failed to talk to slave pot1\n");
            exit(1);
        }

        if(write(file,buf,2) != 2) {
            printf("Failed to write to the i2c bus: ");
            printf(strerror(errno));
            printf("\n");
        }
    }
}
