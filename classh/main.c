/*
 * Class H Power Amp
 * Control Logic
 * Target: BeagleBoard (OMAP3)
 *
 * Jon Evans <jon@craftyjon.com>
 */

#include <stdio.h>
#include <errno.h>

#include "classh.h"
#include "i2c.h"

int main() {
    uint8_t temp = 0;

    init_i2c();

    while(1) {
        int t;
        printf("Enter a byte (0-255) to send to both pots: ");
        scanf("%d", &t);
        temp = t;

        pot_write(POT_POS, temp);
        pot_write(POT_NEG, temp);

    }
}
