/*
 * jack.h
 *
 * Jon Evans <jon@craftyjon.com>
 *
 */

#ifndef CLASSH_JACK_H
#define CLASSH_JACK_H

#include <jack/jack.h>

#define BUF_SIZE 16

typedef jack_default_audio_sample_t sample_t;

int process(jack_nframes_t, void*);
void jack_close(void);
void jack_shutdown(void*);
void init_jack(void);

sample_t rolling_buffer[BUF_SIZE];

#endif
