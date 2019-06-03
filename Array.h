//
// Created by Jorko P on 09-May-19.
//
#ifndef NFA_ARRAY_H
#define NFA_ARRAY_H

// А template dynamic array class from seminars with few changes
template<typename T>
class Array {
    T* arr;
    int numberOfElements;
    int capacity;

public:
    Array() {
        numberOfElements = 0;
        capacity = 16;
        arr = new T[capacity];
    }
    Array(const Array& other) {
        numberOfElements = other.numberOfElements;
        capacity = other.capacity;
        arr = new T[capacity];

        for (int i = 0; i < numberOfElements; i++) {
            arr[i] = other.arr[i];
        }
    }
    Array& operator=(const Array& other) {
        if(this != &other) {
            delete[] arr;
            numberOfElements = other.numberOfElements;
            capacity = other.capacity;
            arr = new T[capacity];

            for (int i = 0; i < numberOfElements; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    ~Array() {
        delete[] arr;
    }

    void reduceNumberOfElements() {
        numberOfElements--;
    }
    int getNumberOfElements() const {
        return numberOfElements;
    }
    void addElement(T element) {
        if (numberOfElements == capacity) {
            capacity *= 2;
            T* helper = new T[capacity];

            for(int i = 0; i < numberOfElements; i++) {
                helper[i] = arr[i];
            }

            delete[] arr;
            arr = helper;
        }
        arr[numberOfElements] = element;
        numberOfElements++;
    }
    void removeElement(T element) {
        if(numberOfElements > 0){
            for(int i = 0 ; i < numberOfElements ; i++){
                if(arr[i] == element){
                    for(int j = i; j < numberOfElements - 1; j++){
                        arr[j] = arr[j+1];
                    }
                    numberOfElements--;
                    break;
                }
            }
        }
    }

    T operator [] (int i) const {
        return arr[i];
    }
    T& operator [] (int i) {
        return arr[i];
    }
    bool operator== (const Array& other) const {
        if(numberOfElements != other.numberOfElements) return false;
        for (int i = 0; i < numberOfElements; ++i) {
            if(!(arr[i] == other.arr[i])) return false;
        }
        return true;
    }
};

#endif //NFA_ARRAY_H
