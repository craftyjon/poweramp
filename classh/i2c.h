/*
 * Class H Power Amp
 * Control Logic
 * Target: Beagleboard (OMAP3)
 *
 * Jon Evans <jon@craftyjon.com>
 */

#ifndef ORANGE_I2C_H
#define ORANGE_I2C_H

#define POT_POS     0x2C
#define POT_NEG     0x2E
#define CMD_WRITE   0x00

void init_i2c(void);
void pot_write(uint8_t, uint8_t);

#endif
