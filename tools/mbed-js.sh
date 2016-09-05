#!/bin/bash
set -e

cjsc index.js -o /workspace/js/main.js && cd /workspace/ && js2c && mbed-cli compile -t GCC_ARM -m $1

if [ -f ./.build/$1/GCC_ARM/workspace.bin ]; then
mv .build/$1/GCC_ARM/workspace.bin /mbed_home/mbed-js.bin
elif [ -f ./.build/$1/GCC_ARM/workspace.hex ]; then
mv .build/$1/GCC_ARM/workspace.hex /mbed_home/mbed-js.hex
else
echo Unable to find output binary.
exit 1
fi


