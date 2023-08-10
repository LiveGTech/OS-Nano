#!/bin/bash

# LiveG OS Nano
# 
# Copyright (C) LiveG. All Rights Reserved.
# 
# https://liveg.tech/os
# Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.

npm install -g terser

python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"

pushd installdev
    rm -rf duktape

    wget https://duktape.org/duktape-2.7.0.tar.xz -O duktape.tar.xz
    tar -xf duktape.tar.xz
    mv duktape-2.7.0 duktape

    emsdk/emsdk install latest
    emsdk/emsdk activate latest
popd