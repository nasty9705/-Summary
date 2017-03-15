#ifndef code_hpp
#define code_hpp

#include "abstractText.hpp"
#include <stdio.h>

class Code_text:public Abstract_Text
{
    char **text;
    unsigned int counter;
public:
    Code_text(); 
    ~Code_text();
    
    void push(const char*);
    unsigned long int count_symbols()const;
    unsigned long int count_words()const;
    void print(FILE*)const;
    void read(FILE*);
    void prints(FILE*)const;
};

#endif
