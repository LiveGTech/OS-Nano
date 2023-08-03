/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef DATATYPES_H_
#define DATATYPES_H_

typedef unsigned int Count;

template<typename T> T* store(T value);
template<typename T> T discard(T* itemPtr);

namespace dataTypes {
    template<typename T> struct _ListItem {
        T* valuePtr;
        _ListItem<T>* nextItemPtr;
    };

    template<typename T> class StoredValue {
        public:
            T value;

            StoredValue<T>(T valueToStore);
            ~StoredValue<T>();
    };

    template<typename T> class List {
        public:
            typedef void (*IterationCallback)(T* itemPtr, Count index);
            typedef T* (*MappingFunction)(T* itemPtr, Count index);
            typedef bool (*FilteringFunction)(T* itemPtr, Count index);

            List<T>();
            ~List<T>();

            T* operator[](int index);

            void start();
            T* next();
            Count length();
            void empty();
            Count push(T* valuePtr);
            T* pop();
            Count unshift(T* valuePtr);
            T* shift();
            Count insert(Count index, T* valuePtr);
            T* remove(Count index);
            int indexOf(T* valuePtr);
            void forEach(IterationCallback iterationCallback);
            List<T> map(MappingFunction mappingFunction);
            List<T> filter(FilteringFunction filteringFunction);
            List<T> concat(List<T> otherList);

        private:
            _ListItem<T>* _firstItemPtr;
            _ListItem<T>* _currentItemPtr;
            Count _length;

            _ListItem<T>* getItemAtIndex(int index);
            _ListItem<T>* getLastItem();
    };
}

#endif