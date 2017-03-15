#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "ratlib.hpp"
#include "elements.hpp"
#include "matrix.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class Vector
{
private:
    unsigned long int razmer;
    Elements subject;
public:
    Vector();
    Vector(unsigned long int, const RatNum&);
    Vector(const Vector&);
    Vector(FILE*);
    
    ~Vector();
    
    unsigned long int kolvo_columns()const;
    void read(FILE*);
    void write(FILE*);
    void vector_print(FILE*);
    void fix(unsigned long int, const RatNum&);
    void pravilo();
    
    
    Vector operator+(const Vector&)const;
    
    Vector operator-(const Vector&)const;
    Vector operator-();
    
    Vector operator*(const RatNum&)const;
    Vector operator*(const Matrix&)const;
    //RatNum operator*(const Vector&)const;
    friend Vector operator*(const RatNum&, const Vector&);
    
    Vector operator=(const Vector&);
    
    Vector operator/(const RatNum&)const;
    
    RatNum operator[](unsigned long int)const;
};

#endif