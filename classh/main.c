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
#include <time.h>

#include "jack.h"
#include "classh.h"
#include "i2c.h"

int buf_ptr;
extern sample_t rolling_buffer[BUF_SIZE];

uint8_t pot_neg;
uint8_t pot_pos;

uint8_t pot_pos_change, pot_neg_change;


/* Input: audio sample (sample_t)
 * Output: none (updates globals)
 * Does the "class H" math on an audio sample
 */
void sample_to_potvals(sample_t sample) {
    float out_voltage = 0;
    uint8_t temp = 0;

    out_voltage = fminf(fmaxf((AMP_SLOPE * sample) + AMP_OFFSET, AMP_FLOOR),AMP_CEILING);

//  pot_pos = (POT_POS_SLOPE * ((out_voltage - PSU_POS_OFFSET) / PSU_POS_SLOPE)) + POT_POS_OFFSET;
//  pot_neg = (POT_NEG_SLOPE * ((out_voltage - PSU_NEG_OFFSET) / PSU_NEG_SLOPE)) + POT_NEG_OFFSET;

    temp = voltage_to_potval(POT_POS, out_voltage);
    if(temp != pot_pos) {
        pot_pos_change = (temp > pot_pos) ? 1 : -1;
        pot_pos = temp;
    } else {
        pot_pos_change = 0;
    }
    
    temp = voltage_to_potval(POT_NEG, out_voltage);
    if(temp != pot_neg) {
        pot_neg_change = (temp > pot_neg) ? 1 : -1;
        pot_neg = temp;
    } else {
        pot_neg_change = 0;
    }


    mvprintw(4,1,"%1.3f\t%1.3f\t%3d\t%3d\n",sample,out_voltage,pot_pos,pot_neg);
    refresh();
}

// Massive kluge for Friday
uint8_t voltage_to_potval(uint8_t pot, float voltage) {
    uint8_t potval = 0;
    switch(pot) {
        case POT_POS:
            potval = 0;
            if(voltage <= 11.3)
                potval = 1;
            if(voltage <= 8.8)
                potval = 2;
            if(voltage <= 7.3)
                potval = 3;
            if(voltage <= 6.3)
                potval = 4;
            if(voltage <= 5.6)
                potval = 5;
            if(voltage <= 5.1)
                potval = 6;
            if(voltage <= 4.75)
                potval = 7;
            if(voltage <= 4.5)
                potval = 8;
            if(voltage <= 4.2)
                potval = 9;
            if(voltage <= 3.9)
                potval = 10;
            break;

        case POT_NEG:
            potval = 0;
            break;
    }
    return potval;
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

    int ret = 0;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 700000;

    if(pot_pos_change > 0)  // lowering positive voltage
    {
        fprintf(stderr,"updating lower");
        if((ret = pot_write(POT_POS, pot_pos)) != 0) {
            mvprintw(7,1,"I2C Error!");
            mvprintw(8,1,strerror(ret));
            refresh();
        } else {
            mvprintw(7,1,"           ");
            refresh();
        }
        nanosleep(&ts,NULL);
    }
    if(pot_pos_change < 0) // raising positive voltage, high priority
    {
        fprintf(stderr,"updating higher");
        ts.tv_nsec = 900000;
        if((ret = pot_write(POT_POS, pot_pos)) != 0) {
            mvprintw(7,1,"I2C Error!");
            mvprintw(8,1,strerror(ret));
            refresh();
        } else {
            mvprintw(7,1,"           ");
            refresh();
        }
        nanosleep(&ts,NULL);
    }

    if(!pot_neg_change)
        return;

  //  pot_write(POT_NEG, pot_neg);
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
    pot_neg = 0;
    pot_pos = 0;

    pot_pos_change = false;
    pot_neg_change = false;

    init_i2c();
    init_jack();
    init_buffer();
    init_curses();

    /*struct timespec {
        time_t tv_sec;
        long tv_nsec;
    } ts;*/
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 400000;

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
        nanosleep(&ts,NULL);
#endif
    }

    jack_close();
}
