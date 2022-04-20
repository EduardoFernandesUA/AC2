#!/bin/bash

pcompile $1
fileName=${1::-2}

if [ $? -eq 0 ]; then
	ldpic32 $fileName.hex && pterm

	rm */*.elf */*.hex */*.map */*.o */*.sym
fi
