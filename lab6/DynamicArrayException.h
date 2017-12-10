#pragma once

#include <stdexcept>

class DynamicArrayException : public std::runtime_error{
public:
    explicit DynamicArrayException(std::string const &exception) : runtime_error(exception){}
};

class ArrayIndexOutOfBoundsException : public DynamicArrayException{
public:
    ArrayIndexOutOfBoundsException() : DynamicArrayException("ArrayIndexOutOfBoundsException"){}
};

class ArraySizeException : public DynamicArrayException{
public:
    ArraySizeException() : DynamicArrayException("ArraySizeException"){}
};

class ArrayCopyException : public DynamicArrayException{
public:
    ArrayCopyException() : DynamicArrayException("ArrayCopyException"){}
};
