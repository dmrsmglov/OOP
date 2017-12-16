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

    class ArrayIterator : std::iterator<std::random_access_iterator_tag,
    value_type>{
    public:
        explicit ArrayIterator (value_type *ptr):
        data_ptr_(ptr){}
        ArrayIterator &operator ++(){
            data_ptr_++;
            return *this;
        }
        ArrayIterator operator ++ (int){
            return ArrayIterator(data_ptr_++);
        }
        ArrayIterator &operator --(){
            data_ptr_--;
            return *this;
        }
        ArrayIterator operator --(int){
            return ArrayIterator(data_ptr_--);
        }
        ArrayIterator &operator += (size_type x){
            return *(this -> data_ptr_ += x);
        }
        ArrayIterator operator + (size_type x){
            return ArrayIterator(this -> data_ptr_ + x);
        }
        ArrayIterator &operator -= (size_type x){
            return *(this -> data_ptr_ -= x);
        }
        ArrayIterator operator - (size_type x){
            return ArrayIterator(this -> data_ptr_ - x);
        }
        bool operator < (const ArrayIterator &other) const{
            return this -> data_ptr_ < other.data_ptr_;
        }
        bool operator > (const ArrayIterator &other) const{
            return other < *this;
        }
        bool operator == (const ArrayIterator &other) const{
            return !(*this < other) && !(other < *this);
        }
        bool operator != (const ArrayIterator &other) const{
            return (*this < other) || (other < *this);
        }
        bool operator <= (const ArrayIterator &other) const{
            return !(other < *this);
        }
        bool operator >= (const ArrayIterator &other) const{
            return !(*this < other);
        }
        value_type *operator ->() const {
            return data_ptr_;
        }
        value_type &operator *() const {
            return *data_ptr_;
        }
    private:
        value_type *data_ptr_;
    };

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

    template<class ...Args>
    void emplace_back(Args... newValue){
        if (size_ == capacity_){
            capacity_ *= 2;
            value_type * newData = new value_type[capacity_];
            memcpy(newData, data_, size_);
            deleteData();
            data_ = newData;
        }
        data_[size_++] = new value_type(newValue);
    }

    template<class ...Args>
    void emplace(ArrayIterator iterator, Args... newValue){
        ArrayIterator curIterator = end();
        while (curIterator != iterator){
            (*curIterator)
        }
    }

    ArrayIterator begin(){
        return ArrayIterator(data_);
    }

    ArrayIterator end(){
        return ArrayIterator(data_ + size_);
    }

    void printDynamicArray(){
        for (size_type i = 0; i < size_; ++i){
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
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
