#ifndef textViewer_hpp
#define textViewer_hpp

#include "abstractText.hpp"
#include <stdio.h>

class Text_viewer
{
    Abstract_Text **text;
    unsigned int count_symbols, count_words, count_paragraphs;
public:
    Text_viewer();
    Text_viewer(FILE*);
    ~Text_viewer();
    
    void operator=(const Text_viewer&);
    void read(FILE*);
    void print(FILE*);
    void delete_obj();
};

#endif
