/** jack.c
 *
 * Author: Jon Evans <jon@craftyjon.com>
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jack/jack.h>

#include "jack.h"

jack_port_t *input_port;
jack_port_t *output_port_signal;
jack_client_t *client;

extern sample_t rolling_buffer[BUF_SIZE];

/* Main JACK callback - processes nframes as passthrough, storing up to BUF_SIZE of them into the buffer */
int process (jack_nframes_t nframes, void *arg) {
    jack_default_audio_sample_t *in, *out;

    in = jack_port_get_buffer (input_port, nframes);
    out = jack_port_get_buffer (output_port_signal, nframes);

    memcpy (out, in, sizeof (sample_t) * nframes);

    if(nframes < BUF_SIZE)
    {
        memcpy (rolling_buffer, in, sizeof(sample_t)*nframes);
    }
    else
    {
        memcpy (rolling_buffer, in, sizeof(sample_t)*BUF_SIZE);
    }

    return 0;
}

/* If JACK has to die, it will use this callback to make us die as well */
void jack_shutdown (void *arg) {
    exit (1);
}

void jack_close(void) {
    jack_client_close(client);
}

/* Hook in our program to the JACK server for realtime audio I/O */
void init_jack(void)
{
    const char **ports;
    const char *client_name = "classh";
    const char *server_name = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

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

    input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    output_port_signal = jack_port_register (client, "output_signal", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if ((input_port == NULL) || (output_port_signal == NULL))
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

    free (ports);
}

