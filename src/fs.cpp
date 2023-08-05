/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>

#include "fs.h"
#include "datatypes.h"

auto openFileHandles = dataTypes::List<fs::FileHandle>();

fs::FileHandle::FileHandle(String path) {
    bool isAlreadyOpen = isFileOpen(path);

    _path = path;
    _isOpen = true;

    openFileHandles.push(this);

    if (isAlreadyOpen) {
        close();
    }
}

fs::FileHandle::~FileHandle() {
    close();
}

String fs::FileHandle::path() {
    return _path;
}

void fs::FileHandle::close() {
    if (!_isOpen) {
        return;
    }

    _isOpen = false;

    openFileHandles.remove(openFileHandles.indexOf(this));
}

fs::FileHandle* fs::open(String path) {
    if (isFileOpen(path)) {
        return nullptr;
    }

    return new FileHandle(path);
}

bool fs::isFileOpen(String path) {
    openFileHandles.start();

    while (auto fileHandle = openFileHandles.next()) {
        if (fileHandle->path() == path) {
            return true;
        }
    }

    return false;
}