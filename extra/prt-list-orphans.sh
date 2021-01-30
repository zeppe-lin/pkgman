#!/bin/sh
DEPLIST=""
pkginfo -i | while read pkg ver; do
  DEPLIST="$DEPLIST $(prt dep $pkg | xargs)"
done
echo $DEPLIST

#pkginfo -i | while read pkg ver; do
#  if [ 
 
# vim:ft=sh:sw=2:ts=2:sts=2:et:cc=72
# End of file
