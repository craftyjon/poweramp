/** playground.c
 *
 * Author: Jonathan Evans <jonathan.evans@tufts.edu>
 * Revision: 2010-11-18
 *
 * Description: DSP Playground using JACK.  Sets up a JACK client, maps input to output, and captures statistics about the input.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <jack/jack.h>

#include <curses.h>

#define BUF_SIZE 16

jack_port_t *input_port;
jack_port_t *output_port_signal;
jack_port_t *output_port_power;
jack_client_t *client;

typedef jack_default_audio_sample_t sample_t;

sample_t rolling_buffer[BUF_SIZE];
sample_t power_output;
int buf_ptr;

float pg_psu_voltage(sample_t);

/* Main JACK callback - processes nframes as passthrough, storing up to BUF_SIZE of them into the buffer */
int process (jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *in, *out, *out_power;

    in = jack_port_get_buffer (input_port, nframes);
    out = jack_port_get_buffer (output_port_signal, nframes);
    out_power = (sample_t *)jack_port_get_buffer(output_port_power, nframes);

    memcpy (out, in, sizeof (sample_t) * nframes);

    jack_nframes_t i;
    for(i=0; i<nframes; i++)
    {
        out_power[i] = (sample_t)(pg_psu_voltage(in[i])/15);
    }

    //memcpy (out_power, power_output, sizeof(sample_t));

    if(nframes < BUF_SIZE)
    {
        mvprintw(0,0,"Warning: Capturing less than %d frames into the buffer in process()!",BUF_SIZE);
        memcpy (rolling_buffer, in, sizeof(sample_t)*nframes);
    }
    else
    {
        memcpy (rolling_buffer, in, sizeof(sample_t)*BUF_SIZE);
    }

    return 0;
}

/* If JACK has to die, it will use this callback to make us die as well */
void jack_shutdown (void *arg)
{
    exit (1);
}

/* Set up ncurses for pretty printing */
void pg_init_curses(void)
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
}

/* Hook in our program to the JACK server for realtime audio I/O */
void pg_init_jack(void)
{
    const char **ports;
    const char *client_name = "jack-playground";
    const char *server_name = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    power_output = 0;

    /* open a client connection to the JACK server */

    client = jack_client_open (client_name, options, &status, server_name);
    if (client == NULL)
    {
        fprintf (stderr, "jack_client_open() failed, "
                 "status = 0x%2.0x\n", status);
        if (status & JackServerFailed)
        {
            fprintf (stderr, "Unable to connect to JACK server\n");
        }
        exit (1);
    }
    if (status & JackServerStarted)
    {
        fprintf (stderr, "JACK server started\n");
    }
    if (status & JackNameNotUnique)
    {
        client_name = jack_get_client_name(client);
        fprintf (stderr, "unique name `%s' assigned\n", client_name);
    }

    jack_set_process_callback (client, process, 0);
    jack_on_shutdown (client, jack_shutdown, 0);

    /* create one input and two output ports */
    input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    output_port_signal = jack_port_register (client, "output_signal", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    output_port_power = jack_port_register (client, "output_power", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if ((input_port == NULL) || (output_port_signal == NULL) || (output_port_power == NULL))
    {
        fprintf(stderr, "no more JACK ports available\n");
        exit (1);
    }

    if (jack_activate (client))
    {
        fprintf (stderr, "cannot activate client");
        exit (1);
    }

    ports = jack_get_ports (client, NULL, NULL,
                            JackPortIsPhysical|JackPortIsOutput);
    if (ports == NULL)
    {
        fprintf(stderr, "no physical capture ports\n");
        exit (1);
    }

    if (jack_connect (client, ports[0], jack_port_name (input_port)))
    {
        fprintf (stderr, "cannot connect input ports\n");
    }

    free (ports);

    ports = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (ports == NULL)
    {
        fprintf(stderr, "no physical playback ports\n");
        exit (1);
    }

    if (jack_connect (client, jack_port_name (output_port_signal), ports[0]))
    {
        fprintf (stderr, "cannot connect output ports\n");
    }

    if (jack_connect (client, jack_port_name (output_port_power), ports[1]));

    free (ports);
}

/* Calculate the output voltage for a given sample - this formula is fudged for now.
 */
float pg_psu_voltage(sample_t sample)
{
    /* amplifier parameters */
    const float vmax = 15;      // Limit on output rail
    const float vss = 1;        // Quiescent state of output rail
    const float vleeway = 1;  // Leeway voltage (rail must stay this much above output)

    /* scale sample to Vmax */
    float v_sample = (sample * (vmax - vleeway))+vleeway;

    /* compare to vss */
    power_output = (v_sample <= vss) ? vss : v_sample;

    return power_output;
}

int main (int argc, char *argv[])
{
    /* initialize audio buffer */
    buf_ptr = 0;
    memset(rolling_buffer, 0, sizeof(sample_t)*BUF_SIZE);

    /* initialize ncurses */
    pg_init_curses();

    /* initialize jack */
    pg_init_jack();

    sample_t sample_total, sample_max;
    int i, row, col;
    getmaxyx(stdscr,row,col);
    float output_voltage = 0;

    mvprintw(1,1,"==== dsp-playground ====");
    mvprintw(2,1,"Connected at %" PRIu32" kHz",jack_get_sample_rate (client));
    mvprintw(3,1,"Buffer size: %d", BUF_SIZE);

    while(1) {
        sample_total = 0;
        sample_max = 0;
        for(i=0; i<BUF_SIZE; i++) {
            if(rolling_buffer[i] > sample_max)
                sample_max = rolling_buffer[i];
            sample_total += fabs(rolling_buffer[i]);
        }
        sample_total /= BUF_SIZE;

        output_voltage = pg_psu_voltage(sample_total);

        mvprintw(row/2,3,"Current average sample level: %f",sample_total);
        mvprintw((row/2)+1,3,"Current max sample level: %f",sample_max);
        mvprintw((row/2)+2,3,"Power supply output voltage based on average level: %f", output_voltage);
        refresh();

        //usleep(100000);
    }

    jack_client_close (client);
    exit (0);
}
