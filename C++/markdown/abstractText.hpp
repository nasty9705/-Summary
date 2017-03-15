#ifndef abstractText_hpp
#define abstractText_hpp

#include <stdio.h>

class Abstract_Text{
public:
    virtual ~Abstract_Text();
    
    virtual unsigned long int count_symbols()const = 0;
    virtual unsigned long int count_words()const = 0;
    virtual void print(FILE*)const = 0;
    virtual void read(FILE*) = 0;
    virtual void prints(FILE*)const = 0;
};

#endif