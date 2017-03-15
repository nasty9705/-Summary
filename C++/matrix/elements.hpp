#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "ratlib.hpp"
#include <cstdio>

struct element
{
    unsigned long int i, j;
    RatNum subject;
};

class Elements
{
public:
    element* array;
    unsigned long int size;
    
    Elements();
    Elements(const Elements&);
    
    ~Elements();
    
    Elements operator=(const Elements&);
    
    void add_element(unsigned long int, unsigned long int, const RatNum&);
    void delete_element(unsigned long int, unsigned long int);
    RatNum get(unsigned long int, unsigned long int) const;
};

#endif