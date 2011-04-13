#include "i2c.h"

int i2c_driver;
const char *i2c_filename = "/dev/i2c-2";

void init_i2c() {
    if((i2c_driver = open(i2c_filename, O_RDWR)) < 0) {
        perror("Could not open i2c-2");
        exit(1);
    }
}

void pot_write(uint8_t pot, uint8_t val) {
    char buf[2];
    buf[0] = 0x00;
    buf[1] = val;

    if(ioctl(i2c_driver, I2C_SLAVE, pot) < 0) {
        fprintf(stderr,"Could not open pot %02x",pot);
    }

    if(write(i2c_driver,buf,2) != 2) {
        fprintf(stderr,"Failed to write to the i2c bus: ");
        fprintf(stderr,strerror(errno));
        fprintf(stderr,"\n");
    }
}


