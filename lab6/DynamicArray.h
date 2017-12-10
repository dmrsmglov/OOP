#pragma once

#include "DynamicArrayException.h"

template<class T>
class DynamicArray{
public:
    using size_type = size_t;
    using value_type = T;
    explicit DynamicArray(const size_type &size = 0, const value_type &value = value_type()):
            size_(size), numberOfElements_(size)
    {
        data_ = new value_type[size_];
        for (size_type i = 0; i < numberOfElements_; ++i){
            data_[i] = value;
        }
    }

    explicit DynamicArray(const DynamicArray &other){
        numberOfElements_ = other.size();
        size_ = other.reservedSize();
        data_ = new value_type[size_];
        for (size_type i = 0; i < numberOfElements_; ++i){
            data_[i] = other[i];
        }
    }

    explicit DynamicArray(DynamicArray &&other){
        data_ = other.data_;
        size_ = other.size_;
        numberOfElements_ = other.numberOfElements_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.numberOfElements_ = 0;
    }

    explicit DynamicArray(const std::initializer_list<value_type> &il){
        data_ = new value_type[std::distance(il.begin(), il.end())];
        numberOfElements_ = 0;
        for (auto it = il.begin(); it != il.end(); ++it){
            data_[numberOfElements_++] = *it;
        }
        size_ = numberOfElements_;
    }

    ~DynamicArray(){
        delete[] data_;
    }

    DynamicArray &operator = (const DynamicArray &other){
        if (this != &other) {
            delete[] data_;
            numberOfElements_ = other.size();
            size_ = other.reservedSize();
            data_ = new value_type[size_];
            for (size_type i = 0; i < numberOfElements_; ++i) {
                data_[i] = other[i];
            }
        }
        return *this;
    }

    DynamicArray &operator = (DynamicArray && other){
        if (this != &other){
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            numberOfElements_ = other.numberOfElements_;
            other.data_ = nullptr;
            other.numberOfElements_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    size_type size() const{
        return numberOfElements_;
    }

    size_type reservedSize() const{
        return size_;
    }

    void printDynamicArray(){
        for (size_type i = 0; i < numberOfElements_; ++i){
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }
    void pushBack(const value_type &value){
        if (numberOfElements_ < size_){
            data_[numberOfElements_++] = value;
        } else{
            size_ *= 2;
            value_type *newData_ = new value_type[size_];
            for (size_type i = 0; i < numberOfElements_; ++i){
                newData_[i] = data_[i];
            }
            newData_[numberOfElements_++] = value;
            delete [] data_;
            data_ = newData_;
        }
    }

    void resize(const size_type size) {
        value_type newData_ = new value_type[size];
        size_ = size;
        for (size_type i = 0; i < std::min(size, numberOfElements_); ++i){
            newData_[i] = data_[i];
        }
        delete[] data_;
        numberOfElements_ = std::min(numberOfElements_, size_);
        data_ = newData_;
    }

    void reserve(const size_type size){
        if (size > size_){
            value_type newData_ = new value_type[size];
            size_ = size;
            for (size_type i = 0; i < numberOfElements_; ++i){
                newData_[i] = data_[i];
            }
            delete[] data_;
            data_ = newData_;
        }
    }

    value_type & at(const size_type x) {
        return const_cast<value_type &>(const_cast<const DynamicArray&>(*this).at(x));
    }

    value_type const & at(const size_type x) const {
        if (x >= numberOfElements_){
            throw ArrayIndexOutOfBoundsException();
        } else{
            return data_[x];
        }
    }

    value_type & operator[](const size_type x)  {
        return data_[x];
    }

    value_type const & operator[](const size_type x) const{
        return data_[x];
    }

private:
    size_type size_;
    size_type numberOfElements_;
    value_type * data_;
};
