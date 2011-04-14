/*
 * Class H Power Amp
 * Control Logic
 * Target: Beagleboard (OMAP3)
 *
 * Jon Evans <jon@craftyjon.com>
 */

#ifndef ORANGE_CLASSH_H
#define ORANGE_CLASSH_H


// Linearization of potentiometer behavior
#define POT_POS_SLOPE   0.33
#define POT_POS_OFFSET  1.98
#define POT_NEG_SLOPE   0.33
#define POT_NEG_OFFSET  2.25

// Power supply behavior
#define PSU_POS_SLOPE   1.3
#define PSU_POS_OFFSET  1
#define PSU_NEG_SLOPE   1.3
#define PSU_NEG_OFFSET  1

// Note: Power supplies are non-linear with new resistor values.  We are now using a lookup table
uint8_t voltage_to_potval(uint8_t, float);

// Amplifier behavior
#define AMP_SLOPE   15
#define AMP_OFFSET  1
#define AMP_CEILING 16
#define AMP_FLOOR   3

/*
 * out_voltage = (AMP_SLOPE)sample + AMP_OFFSET
 * out_voltage = (PSU_SLOPE)R + PSU_OFFSET
 * pot_byte = (POT_SLOPE)R + POT_OFFSET
 *
 * R = (out_voltage - PSU_OFFSET) / PSU_SLOPE
 * pot_byte = (POT_SLOPE * (out_voltage - PSU_OFFSET) / PSU_SLOPE) + POT_OFFSET
 *
 */

#endif
