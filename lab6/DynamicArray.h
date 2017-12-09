#pragma once

template<class T>
class DynamicArray{
public:
    using size_type = size_t;
    using value_type = T;
    explicit DynamicArray(const size_type &size = 0, const value_type &value = value_type()):
            size_(size)
    {
        data_ = new value_type[size_];
        for (size_type i = 0; i < size_; ++i){
            data_[i] = value;
        }
    }

    explicit DynamicArray(const DynamicArray &other){
        size_ = other.size();
        data_ = new value_type[size_];
        for (size_type i = 0; i < size_; ++i){
            data_[i] = other[i];
        }
    }

    explicit DynamicArray(const std::initializer_list<value_type> &il){
        data_ = new value_type[std::distance(il.begin(), il.end())];
        size_type i = 0;
        for (auto it = il.begin(); it != il.end(); ++it){
            data_[i++] = *it;
        }
    }

    ~DynamicArray(){
        delete[] data_;
    }

    DynamicArray& operator = (DynamicArray &other){
        delete[] data_;
        size_ = other.size();
        data_ = new value_type[size_];
        for (size_type i = 0; i < size_; ++i){
            data_[i] = other[i];
        }
    }

    size_type size() const{
        return size_;
    }

    void resize(const size_type &size){
        value_type newData_ = new value_type[size];
        for (size_type i = 0; i < size; ++i){
            if (i >= size_){
                newData_[i] = value_type();
            }
            else{
                newData_[i] = data_[i];
            }
        }
        delete[] data_;
        size_ = size;
        data_ = newData_;
    }

    void reserve(const size_type &size){
        delete [] data_;
        size_ = size;
        data_ = new value_type[size_];
    }

    value_type & at(const size_type &x) const {
        if (x >= size_){

        }
        else{
            return data_[x];
        }
    }

    value_type & operator[](size_type &x) const {
        return const_cast<value_type&>(const_cast<const DynamicArray&>(*this)[x]);
    }

    value_type const & operator[](const size_type &x) const{
        return data_[x];
    }



private:
    size_type size_;
    value_type * data_;
};
