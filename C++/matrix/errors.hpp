#ifndef ERRORS_HPP
#define ERRORS_HPP

class znam_ERROR
{
public:
    znam_ERROR();
};

class delenie_na_0_ERROR
{
public:
    delenie_na_0_ERROR();
};

class memory_allocated_ERROR
{
public:
    unsigned long int size;
    memory_allocated_ERROR(unsigned long int);
};

class types_ERROR
{
public:
    types_ERROR();
};

class matrix_razmer_ERROR
{
public:
    matrix_razmer_ERROR();
};

class vector_razmer_ERROR
{
public:
    vector_razmer_ERROR();
};

#endif