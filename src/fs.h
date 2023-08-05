/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef FS_H_
#define FS_H_

#include <Arduino.h>

namespace fs {
    class FileHandle {
        public:
            FileHandle(String path);
            ~FileHandle();

            String path();
            void close();

        private:
            String _path;
            bool _isOpen;
    };

    FileHandle* open(String path);
    bool isFileOpen(String path);
}

#endif