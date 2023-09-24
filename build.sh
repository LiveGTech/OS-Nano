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

pushd $SOURCE_DIR
    buildRootScript rootsrc/api.js rootfs/system/api.min.js

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