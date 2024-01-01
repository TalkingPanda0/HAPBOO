#!/bin/sh
input="$1"
while read -r voice text; do
	echo $voice | grep -q "^#" && continue 
	echo $voice $text
	curl --get \
		--data-urlencode "voice=$voice" \
		--data-urlencode "text=$text" \
	"https://api.streamelements.com/kappa/v2/speech" -o "audio/$voice $text.mp3"
done < $input
