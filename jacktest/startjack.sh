#!/bin/sh

jackd -m --no-realtime -d alsa -p 512 -n 4 -P hw:0 -C hw:0 -S -r 48000
