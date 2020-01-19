#!/bin/sh
gcc dispatcher.c common.c -o dispatcher
gcc charger.c common.c -o charger
./dispatcher
./charger