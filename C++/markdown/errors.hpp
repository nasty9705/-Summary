#ifndef errors_hpp
#define errors_hpp

#include <stdio.h>

class NoSuchFileERROR{};

class TextProcessingERROR{};

class EmptyParagraphERROR{};

class MemoryAllocatingERROR{
public:
    unsigned long int size;
    MemoryAllocatingERROR(unsigned long int);
};

#endif
