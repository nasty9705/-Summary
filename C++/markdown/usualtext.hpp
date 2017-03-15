#ifndef Usualtext_hpp
#define Usualtext_hpp

#include "abstractText.hpp"
#include <stdio.h>

class Usual_text:public Abstract_Text
{
    char* text;
public:

    Usual_text();
    ~Usual_text();
    
    unsigned long int count_symbols()const;
    unsigned long int count_words()const;
    void print(FILE*)const;
    void read(FILE*);
    void prints(FILE*)const;
    char* prepare()const;
};

#endif
