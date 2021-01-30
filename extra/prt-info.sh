#!/bin/sh
# prt info command replacement
FMT() { echo "                  $@"; }
path=$(prt printf "%p/%n" --filter=$1)

prt printf --filter=$1 "
name:             %n
path:             %p
version:          %v
release:          %r
description:      %d
url:              %u
maintainer:       %M
dependencies:     %e
content:
$(if [ -x /usr/bin/treex ]; then
	tree -av --noreport -C $path | while read f; do FMT $f; done
else
	FMT $path
	ls -A --color=always -v $path | while read f; do FMT $f; done
fi)"
