/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>

#ifndef GOSN_SIMULATOR
    #include <SPIFFS.h>
#else
    #include <stdio.h>
#endif

#include "fs.h"
#include "datatypes.h"

auto openFileHandles = dataTypes::List<fs::FileHandle>();

const char* getStdFileMode(fs::FileMode mode) {
    switch (mode) {
        case fs::FileMode::READ:
        default:
            return "r";

        case fs::FileMode::WRITE:
            return "w";

        case fs::FileMode::APPEND:
            return "a";
    }
}

fs::FileHandle::FileHandle(String path, fs::FileMode mode) {
    bool isAlreadyOpen = isFileOpen(path);
    char pathCharArray[path.length() + 1];

    _path = path;
    _mode = mode;
    _isOpen = true;
    _errorOnOpen = false;

    path.toCharArray(pathCharArray, sizeof(pathCharArray));

    openFileHandles.push(this);

    if (isAlreadyOpen) {
        close();

        return;
    }

    #ifndef GOSN_SIMULATOR
        _file = SPIFFS.open(pathCharArray, getStdFileMode(_mode));
    #else
        _file = fopen(pathCharArray, getStdFileMode(_mode));
    #endif

    if (!_file) {
        _errorOnOpen = true;

        close();

        return;
    }
}

fs::FileHandle::~FileHandle() {
    close();
}

String fs::FileHandle::path() {
    return _path;
}

fs::FileMode fs::FileHandle::mode() {
    return _mode;
}

bool fs::FileHandle::isOpen() {
    return _isOpen;
}

bool fs::FileHandle::isAvailable() {
    #ifndef GOSN_SIMULATOR
        return _file.available();
    #else
        return !feof(_file);
    #endif
}

char fs::FileHandle::read() {
    if (!isAvailable()) {
        return '\0';
    }

    #ifndef GOSN_SIMULATOR
        return _file.read();
    #else
        int c = fgetc(_file);

        if (c < 0) {
            return '\0';
        }

        return (char)c;
    #endif
}

String fs::FileHandle::readString() {
    String data;

    while (isAvailable()) {
        char c = read();

        if (c == '\0') {
            break;
        }

        data += c;
    }

    return data;
}

void fs::FileHandle::write(char c) {
    if (!isAvailable()) {
        return;
    }

    if (!(_mode == FileMode::WRITE || _mode == FileMode::APPEND)) {
        return;
    }

    #ifndef GOSN_SIMULATOR
        _file.write(c);
    #else
        fputc(c, _file);
    #endif
}

void fs::FileHandle::start() {
    if (!isAvailable()) {
        return;
    }

    #ifndef GOSN_SIMULATOR
        _file.seek(0);
    #else
        fseek(_file, 0, SEEK_SET);
    #endif
}

void fs::FileHandle::close() {
    if (!_isOpen) {
        return;
    }

    _isOpen = false;

    openFileHandles.remove(openFileHandles.indexOf(this));

    if (_errorOnOpen) {
        return;
    }

    #ifndef GOSN_SIMULATOR
        _file.close();
    #else
        fclose(_file);
    #endif
}

fs::FileHandle* fs::open(String path, fs::FileMode mode) {
    auto fileHandle = new FileHandle(path, mode);

    if (!fileHandle->isOpen()) {
        return nullptr;
    }

    return fileHandle;
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