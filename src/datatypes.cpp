/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include "datatypes.h"
#include "proc.h"

template<typename T> T* store(T value) {
    auto storedValue = new dataTypes::StoredValue<T>(value);

    return &storedValue->value;
}

template<typename T> T discard(T* itemPtr) {
    auto itemValue = *itemPtr;

    delete itemPtr;

    return itemValue;
}

template<typename T> dataTypes::StoredValue<T>::StoredValue(T valueToStore) {
    value = valueToStore;
}

template<typename T> dataTypes::StoredValue<T>::~StoredValue() {}

template<typename T> dataTypes::List<T>::List() {
    _firstItemPtr = nullptr;
}

template<typename T> dataTypes::List<T>::~List() {
    if (!_firstItemPtr) {
        return;
    }

    dataTypes::_ListItem<T>* currentItemPtr = _firstItemPtr;

    while (currentItemPtr) {
        auto nextItemPtr = currentItemPtr->nextItemPtr;

        delete currentItemPtr;

        currentItemPtr = nextItemPtr;
    }
}

template<typename T> dataTypes::_ListItem<T>* dataTypes::List<T>::getItemAtIndex(int index) {
    if (!_firstItemPtr) {
        return nullptr;
    }

    if (index < 0) {
        int resolvedIndex = length() + index;

        if (resolvedIndex < 0) {
            return nullptr;
        }

        return getItemAtIndex(resolvedIndex);
    }

    auto currentItemPtr = _firstItemPtr;

    while (index > 0) {
        currentItemPtr = currentItemPtr->nextItemPtr;
        index--;
    }

    return currentItemPtr;
}

template<typename T> T* dataTypes::List<T>::operator[](int index) {
    auto item = getItemAtIndex(index);

    if (!item) {
        return nullptr;
    }

    return item->valuePtr;
}

template<typename T> Count dataTypes::List<T>::length() {
    Count _length = 0;
    auto currentItemPtr = _firstItemPtr;

    while (currentItemPtr) {
        currentItemPtr = currentItemPtr->nextItemPtr;
        _length++;
    }

    return _length;
}

template<typename T> dataTypes::_ListItem<T>* dataTypes::List<T>::getLastItem() {
    if (!_firstItemPtr) {
        return nullptr;
    }

    auto currentItemPtr = _firstItemPtr;
    _ListItem<T>* previousItemPtr;

    do {
        previousItemPtr = currentItemPtr;
        currentItemPtr = currentItemPtr->nextItemPtr;
    } while (currentItemPtr);

    return previousItemPtr;
}

template<typename T> Count dataTypes::List<T>::push(T* valuePtr) {
    auto itemPtr = new _ListItem<T> {
        .valuePtr = valuePtr,
        .nextItemPtr = nullptr
    };

    if (!_firstItemPtr) {
        _firstItemPtr = itemPtr;

        return 1;
    }

    getLastItem()->nextItemPtr = itemPtr;

    return length();
}

template<typename T> T* dataTypes::List<T>::pop() {
    if (!_firstItemPtr) {
        return nullptr;
    }

    if (!_firstItemPtr->nextItemPtr) {
        auto item = _firstItemPtr;

        _firstItemPtr = nullptr;

        return item->valuePtr;
    }

    auto lastItem = getLastItem();

    getItemAtIndex(-2)->nextItemPtr = nullptr;

    return lastItem->valuePtr;
}

template<typename T> T* dataTypes::List<T>::shift() {
    if (!_firstItemPtr) {
        return nullptr;
    }

    auto item = _firstItemPtr;

    _firstItemPtr = _firstItemPtr->nextItemPtr;

    return item->valuePtr;
}

template<typename T> void dataTypes::List<T>::forEach(IterationCallback iterationCallback) {
    auto currentItemPtr = _firstItemPtr;
    Count index = 0;

    while (currentItemPtr) {
        iterationCallback(currentItemPtr->valuePtr, index);

        currentItemPtr = currentItemPtr->nextItemPtr;
        index++;
    }
}

template class dataTypes::StoredValue<int>;

template class dataTypes::List<proc::Process>;
template class dataTypes::List<int>;

template int* store<int>(int value);
template int discard<int>(int* itemPtr);