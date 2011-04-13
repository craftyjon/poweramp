#!/bin/sh

jackd -m --no-realtime -d alsa -p 512 -n 8 -P hw:0 -C hw:0 -S -r 44100
