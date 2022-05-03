#!/bin/bash

fileName=${1::-2}

pcompile $1

port="-p $2"
if [ -z "$2" ]; then
    port=""
fi

if [ $? -eq 0 ]; then
	ldpic32 $fileName.hex $port
	
	if [ $? -eq 0 ]; then
		pterm $port
	fi

fi

rm -f */*.elf */*.hex */*.map */*.o */*.sym
