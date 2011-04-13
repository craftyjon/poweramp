/*
 * Class H Power Amp
 * Control Logic
 * Target: BeagleBoard (OMAP3)
 *
 * Jon Evans <jon@craftyjon.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <curses.h>

#include "jack.h"
#include "classh.h"
#include "i2c.h"

int buf_ptr;
extern sample_t rolling_buffer[BUF_SIZE];

uint8_t pot_neg;
uint8_t pot_pos;

/* Input: audio sample (sample_t)
 * Output: none (updates globals)
 * Does the "class H" math on an audio sample
 */
void sample_to_potvals(sample_t sample) {
    float out_voltage = 0;

    out_voltage = (AMP_SLOPE * sample) + AMP_OFFSET;

    pot_pos = (POT_POS_SLOPE * ((out_voltage - PSU_POS_OFFSET) / PSU_POS_SLOPE)) + POT_POS_OFFSET;
    pot_neg = (POT_NEG_SLOPE * ((out_voltage - PSU_NEG_OFFSET) / PSU_NEG_SLOPE)) + POT_NEG_OFFSET;

    mvprintw(4,1,"%1.3f\t%1.3f\t%3d\t%3d\n",sample,out_voltage,pot_pos,pot_neg);
    refresh();
}


void sigint_handler(int sig) {
    fprintf(stderr,"Caught signal, exiting...\n");
    endwin();
    jack_close();
    exit(1);
}

void init_buffer() {
    buf_ptr = 0;
    memset(rolling_buffer, 0, sizeof(sample_t)*BUF_SIZE);
}

void update_buffer() {
    sample_t sample_total, sample_max;
    int i;

    sample_total = 0;
    sample_max = 0;
    for(i=0; i<BUF_SIZE; i++) {
        if(rolling_buffer[i] > sample_max)
            sample_max = rolling_buffer[i];
        sample_total += fabs(rolling_buffer[i]);
    }
    sample_total /= BUF_SIZE;

    sample_to_potvals(sample_total);
}

void update_pots() {
    pot_write(POT_POS, pot_pos);
    pot_write(POT_NEG, pot_neg);
}

void init_curses() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    mvprintw(1,1,"Class H Power Amp - Control Status");
    mvprintw(2,1,"----------------------------------");

    mvprintw(3,1,"Input\tOutput\tpot_pos\tpot_neg");
}

int main() {
#ifdef DEBUG
    uint8_t temp = 0;
    int t;
#endif
    (void)signal(SIGINT,sigint_handler);

    /* Startup with psu's at maximum */
    pot_neg = 255;
    pot_pos = 255;

    init_i2c();
    init_jack();
    init_buffer();
    init_curses();

    while(1) {
#ifdef DEBUG
        printf("Enter a byte (0-255) to send to both pots: ");
        scanf("%d", &t);
        temp = t;

        pot_write(POT_POS, temp);
        pot_write(POT_NEG, temp);
#else
        update_buffer();
        update_pots();
#endif
    }

    jack_close();
}
