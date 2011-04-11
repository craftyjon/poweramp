/*
 * Class H Power Amp
 * Control Logic
 * Target: Beagleboard (OMAP3)
 *
 * Jon Evans <jon@craftyjon.com>
 */

#ifndef ORANGE_I2C_H
#define ORANGE_I2C_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>

#define POT_POS     0x2C
#define POT_NEG     0x2E
#define CMD_WRITE   0x00

void i2c_init(void);
void pot_write(uint8_t, uint8_t);

#endif