# LiveG OS Nano
# 
# Copyright (C) LiveG. All Rights Reserved.
# 
# https://liveg.tech/os
# Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.

rm -rf installdev
mkdir -p installdev

pushd installdev
    git clone https://github.com/juj/emsdk.git

    emsdk/emsdk install latest
    emsdk/emsdk activate latest

    git clone https://github.com/bxparks/EpoxyDuino
popd