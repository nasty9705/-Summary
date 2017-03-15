#ifndef List_text_hpp
#define List_text_hpp

#include "abstractText.hpp"
#include <stdio.h>

extern char *str_global;
extern unsigned int otstup_list, width_screen;
extern char marked_symbol;

struct ListElement
{
    char* text;
    unsigned int otstup;
    bool listType; // 1-нумерованный, 0-маркированный
};

class List_text:public Abstract_Text
{
    ListElement *list;
    unsigned int counter;
public:
    List_text();
    ~List_text();
    
    void push(char*, unsigned int, bool);
    unsigned long int count_symbols()const;
    unsigned long int count_words()const;
    void print(FILE*) const;
    void read(FILE*);
    void prints(FILE*)const;
    void prepare(ListElement)const;
    unsigned int insert_otstup(unsigned int, unsigned int)const;
    char* giveNum()const;
    char int_to_char(int)const;
};

#endif
