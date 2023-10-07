#!/bin/bash

# LiveG OS Nano
# 
# Copyright (C) LiveG. All Rights Reserved.
# 
# https://liveg.tech/os
# Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.

set -e

SOURCE_DIR=$(dirname "$0")

function buildRootScript {
    mkdir -p ${2%/*}

    npx babel \
        --config-file $SOURCE_DIR/.babelrc.json \
        $1 -o $2

    npx terser $2 -o $2 -c

    printf "// " > src/_forcebuild.cpp
    openssl rand -hex 16 >> src/_forcebuild.cpp
}

function buildFont {
    mkdir -p ${2%/*}

    npx lv_font_conv \
        --font $1 \
        --format bin \
        --size $3 \
        --range 0x20-0xFB04 \
        --bpp 3 \
        -o $2
}

pushd $SOURCE_DIR
    buildRootScript rootsrc/system/api.js rootfs/system/api.min.js

    buildFont rootsrc/system/fonts/main.otf rootfs/system/fonts/main.gosnfont 32

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