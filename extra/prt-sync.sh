#!/bin/sh
if [ "`id -u`" != 0 ]; then
	echo "$0: only root can update ports"
	exit 1
fi

for driver in /etc/ports/drivers/*; do
  suffix=${driver##*/}

  for collection in /etc/ports/*.$suffix; do
    [ $collection = "/etc/ports/*.$suffix" ] && continue
    $driver $collection
  done
done

# vim:ft=sh:sw=2:ts=2:sts=2:et:cc=72
# End of file
