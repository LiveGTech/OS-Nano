/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef FS_H_
#define FS_H_

#include <Arduino.h>

#ifndef GOSN_SIMULATOR
    #include <SPIFFS.h>
#else
    #include <stdio.h>
#endif

namespace fs {
    enum FileMode {
        READ,
        WRITE,
        APPEND
    };

    class FileHandle {
        public:
            FileHandle(String path, FileMode mode = FileMode::READ);
            ~FileHandle();

            String path();
            FileMode mode();
            bool isOpen();
            bool isAvailable();
            char read();
            void write(char c);
            void start();
            void close();

        private:
            String _path;
            FileMode _mode;
            bool _isOpen;

            #ifndef GOSN_SIMULATOR
                File _file;
            #else
                FILE* _file;
            #endif
    };

    FileHandle* open(String path, FileMode mode = FileMode::READ);
    bool isFileOpen(String path);
}

#endif