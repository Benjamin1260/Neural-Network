#include <iostream>
#include <exception>
#include <cstdlib>

/*create a bundle which basically acts like an array but with a size attached,
idk abt performance though, might aswel use a vector at this point*/
template <typename T>
struct custArr {
    T *ptr;
    int size = 0;
    bool memAlocated = false;

    custArr() : ptr(NULL) {
        //std::cout << "NULLPTR CREATED: " << this << std::endl;
    };
    custArr(int iSize) : size(iSize) {
        if (size > 0) {
            ptr = new T[size];
            memAlocated = true;
            //std::cout << "PTR CREATED: " << this << std::endl;
        } else {
            std::cout << "UNDEFINED BEHAVIOUR: SIZE = " << size << std::endl;
            ptr = NULL;
        }
    };
    custArr(int iSize, T val) : custArr(iSize) {
        for (int i = 0; i < iSize; i++) {
            this->ptr[i] = val;
        }
    };

    ~custArr() {
        memAlocated = false;
        //std::cout << "DESTROYING TYPE: " << typeid(T).name() << std::endl;
        //std::cout << (ptr != NULL? " DELETED\n" : " NULLPTR\n");
        if (memAlocated) {
            try {
                delete[] ptr;
            } catch (const std::exception& e) {
                std::cout << "ERROR: " << e.what() << std::endl;
            }
        }
    };

    T& operator[](int i) {
        try {
            if (!memAlocated || ptr == NULL) {
                throw ("ERROR: invalid mem access attempt");
            }
            if (i < 0 || i >= size) {
                throw ("ERROR: out of bound");
            }
            return ptr[i];
        } catch (const char* error) {
            std::cout << error;
            exit(EXIT_FAILURE);
        }
    };

    custArr<T>& operator=(const custArr<T>& other) {
        this->size = other.size;
        this->memAlocated = other.memAlocated;
        this->ptr = memAlocated? other.ptr : NULL;
        //std::cout << "custArr coppied\n";
        return *this;
    };

    T* begin() {
        return &ptr[0];
    };

    T* end() {
        return &ptr[size];
    };
};