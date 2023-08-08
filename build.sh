#!/bin/bash

# LiveG OS Nano
# 
# Copyright (C) LiveG. All Rights Reserved.
# 
# https://liveg.tech/os
# Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.

set -e

SOURCE_DIR=$(dirname "$0")

pushd $SOURCE_DIR
    if [ "$1" == "--sim" ]; then
        source installdev/emsdk/emsdk_env.sh

        emcmake cmake .
        make
    else
        if [ "$1" == "--upload" ]; then
            platformio run --target upload --environment um_feathers3
            platformio run --target uploadfs --environment um_feathers3
        else
            platformio run --environment um_feathers3
        fi
    fi
popd