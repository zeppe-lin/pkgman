#!/bin/sh
prt printf "%l\t%n\n" | grep ^yes | cut -f2
