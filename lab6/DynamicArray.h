#pragma once

#include <cstring>
#include "DynamicArrayException.h"

template<class T>
class DynamicArray{

public:
    using size_type = size_t;
    using value_type = T;

    explicit DynamicArray(const size_type &size = 0, const value_type &value = value_type()):
            capacity_(size * sizeof(value_type)), size_(size)
    {
        data_ = new value_type[capacity_];
        for (size_type i = 0; i < size_; ++i){
            data_[i] = value;
        }
    }

    DynamicArray(const DynamicArray &other){
        size_ = other.size();
        capacity_ = other.capacity();
        data_ = new value_type[capacity_];
        memcpy(data_, other.data_, capacity_);
    }

    DynamicArray(DynamicArray &&other) noexcept {
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

     DynamicArray(const std::initializer_list<value_type> &il){
        data_ = new value_type[std::distance(il.begin(), il.end())];
        size_ = 0;
        for (auto it = il.begin(); it != il.end(); ++it){
            data_[size_++] = *it;
        }
        capacity_ = size_ * sizeof(value_type);
    }

    ~DynamicArray(){
        deleteData();
    }

    DynamicArray &operator = (const DynamicArray &other){
        if (this != &other) {
            deleteData();
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new value_type[capacity_];
            memcpy(data_, other.data_, capacity_);
        }
        return *this;
    }

    DynamicArray &operator = (DynamicArray && other) noexcept {
        if (this != &other){
            deleteData();
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.capacity_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    size_type size() const{
        return size_;
    }

    size_type capacity() const{
        return capacity_;
    }

    void pushBack(const value_type &value){
        if (capacity_ / sizeof(value_type ) > size_){
            data_[size_++] = value;
        } else{
            value_type *newData_ = new value_type[capacity_ * 2];
            memcpy(newData_, data_, size_);
            capacity_ *= 2;
            newData_[size_++] = value;
            deleteData();
            data_ = newData_;
        }
    }

    void resize(const size_type nwCapacity) {
        value_type *newData_ = new value_type[nwCapacity * sizeof(value_type)];
        memcpy(newData_, data_, std::min(nwCapacity * sizeof(value_type), capacity_));
        capacity_ = nwCapacity * sizeof(value_type);
        deleteData();
        size_ = std::min(capacity_ / sizeof(value_type), size_);
        data_ = newData_;
    }

    void reserve(const size_type nwCapacity){
        if (nwCapacity * sizeof(value_type) > capacity_){
            value_type newData_ = new value_type[nwCapacity * sizeof(value_type)];
            memcpy(newData_, data_, size_);
            capacity_ = nwCapacity * sizeof(value_type);
            deleteData();
            data_ = newData_;
        }
    }

    value_type & at(const size_type x) {
        return const_cast<value_type &>(const_cast<const DynamicArray&>(*this).at(x));
    }

    value_type const & at(const size_type x) const {
        if (x >= size_){
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
    void deleteData(){
        for (size_type i = 0; i < size_; ++i){
            data_[i].~value_type();
        }
        delete data_;
    }
    size_type size_;
    size_type capacity_;
    value_type * data_;
};
