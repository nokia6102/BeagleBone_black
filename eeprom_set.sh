#!/bin/bash
NAME="$1"
VALUE="$2"

./eeprom $NAME "$VALUE"

cVALUE=`./eeprom $NAME | cut -d':' -f2 | sed 's/^ //'`

echo Write [$NAME] as [Value]
echo "$VALUE:$cVALUE"

if [ "$VALUE" = "$cVALUE" ];then
     echo "$NAME:PASS"
else
     echo "$NAME:fail"
fi


