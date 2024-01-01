#!/bin/sh
cache=/dev/shm/wallpe
frametime=0.08
setwlp="xwallpaper --tile"
rm -rf $cache/*
mkdir -p $cache
ffmpeg -i "$1" "$cache/%01d.png"
while true; do
	for frame in $(ls $cache | sort -n); do
		$setwlp "$cache/$frame"  &
		sleep $frametime

	done
done
