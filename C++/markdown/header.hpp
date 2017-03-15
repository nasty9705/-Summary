#ifndef Header_hpp
#define Header_hpp

#include "abstractText.hpp"
#include <stdio.h>

class Header_text:public Abstract_Text
{
    char* text;
    int lvl;
public:
    Header_text();    
    ~Header_text();
    
    unsigned long int count_symbols()const;
    unsigned long int count_words()const;
    void print(FILE*)const;
    void read(FILE*);
    void prints(FILE*)const;
    void prepare()const;
    unsigned int insertResh(unsigned int)const;
};

#endif
