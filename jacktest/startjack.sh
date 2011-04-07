#!/bin/sh

jackd -m -d alsa -p 256 -n 4 -P hw:0 -C hw:0 -S -r 48000
