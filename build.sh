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
    echo "Building root script \`$2\`..."

    mkdir -p ${2%/*}

    npx babel \
        --config-file $SOURCE_DIR/.babelrc.json \
        $1 -o $2

    npx terser $2 -o $2 -c

    printf "// " > src/_forcebuild.cpp
    openssl rand -hex 16 >> src/_forcebuild.cpp
}

function buildFont {
    echo "Building font \`$2\`..."

    mkdir -p ${2%/*}

    npx lv_font_conv \
        --font $1 \
        --format bin \
        --size $3 \
        --range $4 \
        --bpp 2 \
        --no-compress \
        -o $2
}

pushd $SOURCE_DIR
    buildRootScript rootsrc/system/api.js rootfs/system/api.min.js

    buildFont rootsrc/system/fonts/main.otf rootfs/system/fonts/main-20.gosnf 20 0x20-0xFB04
    buildFont rootsrc/system/fonts/main.otf rootfs/system/fonts/numerals-32.gosnf 32 0x20,0x22,0x25,0x27,0x2C,0x2D,0x2E,0x2F,0x30-0x39,0x3A
    buildFont rootsrc/system/fonts/main.otf rootfs/system/fonts/numerals-64.gosnf 64 0x20,0x22,0x25,0x27,0x2C,0x2D,0x2E,0x2F,0x30-0x39,0x3A

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