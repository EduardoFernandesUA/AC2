#!/bin/bash

pcompile $1
fileName=${1::-2}

if [ $? -eq 0 ]; then
	ldpic32 $fileName.hex -p /dev/ttyS$2 && pterm -p /dev/ttyS$2

	rm */*.elf */*.hex */*.map */*.o */*.sym
fi
