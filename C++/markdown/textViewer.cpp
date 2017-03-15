#include "textViewer.hpp"
//#include "abstractText.hpp"
#include "usualtext.hpp"
#include "header.hpp"
#include "list.hpp"
#include "code.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstdlib>

Text_viewer::Text_viewer()
{
    count_paragraphs = 0;
    count_symbols = 0;
    count_words = 0;
    text = NULL;
}

Text_viewer::Text_viewer(FILE *input)
{
    char char1 = '\0', *word_tmp = NULL;
    bool tmpBool = true;
    unsigned int i = 0;
    
    if (input == NULL) throw NoSuchFileERROR();
    
    count_paragraphs = 0;
    count_symbols = 0;
    count_words = 0;
    text = NULL;
    
    do{
        char1 = getc(input);
        if (char1 == EOF) {
            for (unsigned int q = 0; q < count_paragraphs; q++) {
                count_words += text[q] -> count_words();
                count_symbols += text[q] -> count_symbols();
            }
            return;
        }
        if (char1 == '\n') continue;
        try {
            text = (Abstract_Text**)realloc(text, (count_paragraphs + 1) * sizeof(Abstract_Text*));
            if (text == NULL) throw MemoryAllocatingERROR((count_paragraphs + 1) * sizeof(Abstract_Text*));
            
            
            switch (char1) {
                case '#':{
                    ungetc(char1, input);
                    text[count_paragraphs] = new Header_text;
                    text[count_paragraphs] -> read(input);
                    break;
                }
                    
                case '*':{
                    ungetc(char1, input);
                    text[count_paragraphs] = new List_text;
                    text[count_paragraphs] -> read(input);
                    break;
                }
                    
                case '\t':{
                    text[count_paragraphs] = new Code_text;
                    text[count_paragraphs] -> read(input);
                    break;
                }
                    
                case ' ':{
                    for (unsigned int q = 0; q < 3; q++) {
                        char1 = getc(input);
                        if (char1 != ' ') {
                            ungetc(char1, input);
                            for (unsigned int w = 0; w < q; w++) {
                                ungetc(' ', input);
                            }
                            text[count_paragraphs] = new Usual_text;
                            text[count_paragraphs] -> read(input);
                            tmpBool = false;
                            break;
                        }
                        else tmpBool = true;
                    }
                    if (tmpBool) {
                        text[count_paragraphs] = new Code_text;
                        text[count_paragraphs] -> read(input);
                    }
                    break;
                }
                    
                default:{
                    i = 0;
                    tmpBool = false;
                    ungetc(char1, input);
                    do{
                        if (isdigit(char1)) {
                            char1 = getc(input);
                            word_tmp = (char*)realloc(word_tmp, i + 1);
                            word_tmp[i] = char1;
                            i++;
                        }
                        else {
                            if (char1 != '.') {
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(word_tmp[i - q - 1], input);
                                }
                                if (word_tmp != NULL) {free(word_tmp); word_tmp = NULL;}
                                text[count_paragraphs] = new Usual_text;
                                text[count_paragraphs] -> read(input);
                                tmpBool = true;
                            }
                            else{
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(word_tmp[i - q - 1], input);
                                }
                                if (word_tmp != NULL) {free(word_tmp); word_tmp = NULL;}
                                text[count_paragraphs] = new List_text;
                                text[count_paragraphs] -> read(input);
                                tmpBool = true;
                            }
                        }
                    }while(!tmpBool);
                    break;
                }
            }
            count_paragraphs++;
        }
        catch (EmptyParagraphERROR) {
            delete text[count_paragraphs];
            count_paragraphs--;
            text = (Abstract_Text**)realloc(text, (count_paragraphs) * sizeof(Abstract_Text*));
            if (text == NULL) throw MemoryAllocatingERROR((count_paragraphs) * sizeof(Abstract_Text*));
        }
        catch(TextProcessingERROR){}
    }while(1);
}

Text_viewer::~Text_viewer()
{
    if(text != NULL) {
        for (unsigned int q = 0; q < count_paragraphs; q++) 
            delete text[q];
        free(text);
    }
}

void Text_viewer::read(FILE *input)
{
    char char1 = '\0', *word_tmp = NULL;
    bool tmpBool = true;
    unsigned int i = 0;
    
    if (input == NULL) throw NoSuchFileERROR();
    
    count_paragraphs = 0;
    count_symbols = 0;
    count_words = 0;
    text = NULL;
    
    do{
        char1 = getc(input);
        if (char1 == EOF) {
            for (unsigned int q = 0; q < count_paragraphs; q++) {
                count_words += text[q] -> count_words();
                count_symbols += text[q] -> count_symbols();
            }
            return;
        }
        if (char1 == '\n') continue;
        try {
            text = (Abstract_Text**)realloc(text, (count_paragraphs + 1) * sizeof(Abstract_Text*));
            if (text == NULL) throw MemoryAllocatingERROR((count_paragraphs + 1) * sizeof(Abstract_Text*));
            
            
            switch (char1) {
                case '#':{
                    ungetc(char1, input);
                    text[count_paragraphs] = new Header_text;
                    text[count_paragraphs] -> read(input);
                    break;
                }
                    
                case '*':{
                    ungetc(char1, input);
                    text[count_paragraphs] = new List_text;
                    text[count_paragraphs] -> read(input);
                    break;
                }
                    
                case '\t':{
                    text[count_paragraphs] = new Code_text;
                    text[count_paragraphs] -> read(input);
                    break;
                }
                    
                case ' ':{
                    for (unsigned int q = 0; q < 3; q++) {
                        char1 = getc(input);
                        if (char1 != ' ') {
                            ungetc(char1, input);
                            for (unsigned int w = 0; w < q; w++) {
                                ungetc(' ', input);
                            }
                            text[count_paragraphs] = new Usual_text;
                            text[count_paragraphs] -> read(input);
                            tmpBool = false;
                            break;
                        }
                        else tmpBool = true;
                    }
                    if (tmpBool) {
                        text[count_paragraphs] = new Code_text;
                        text[count_paragraphs] -> read(input);
                    }
                    break;
                }
                    
                default:{
                    i = 0;
                    tmpBool = false;
                    ungetc(char1, input);
                    do{
                        if (isdigit(char1)) {
                            char1 = getc(input);
                            word_tmp = (char*)realloc(word_tmp, i + 1);
                            word_tmp[i] = char1;
                            i++;
                        }
                        else {
                            if (char1 != '.') {
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(word_tmp[i - q - 1], input);
                                }
                                if (word_tmp != NULL) {free(word_tmp); word_tmp = NULL;}
                                text[count_paragraphs] = new Usual_text;
                                text[count_paragraphs] -> read(input);
                                tmpBool = true;
                            }
                            else{
                                for (unsigned int q = 0; q < i; q++) {
                                    ungetc(word_tmp[i - q - 1], input);
                                }
                                if (word_tmp != NULL) {free(word_tmp); word_tmp = NULL;}
                                text[count_paragraphs] = new List_text;
                                text[count_paragraphs] -> read(input);
                                tmpBool = true;
                            }
                        }
                    }while(!tmpBool);
                    break;
                }
            }
            count_paragraphs++;
        }
        catch (EmptyParagraphERROR) {
            delete text[count_paragraphs];
            count_paragraphs--;
            text = (Abstract_Text**)realloc(text,(count_paragraphs)*sizeof(Abstract_Text*));
            if (text == NULL) 
                throw MemoryAllocatingERROR((count_paragraphs)*sizeof(Abstract_Text*));
        }
        catch(TextProcessingERROR){}
    }while(1);
}

void Text_viewer::print(FILE *output)
{
    for (unsigned int q = 0; q < count_paragraphs; q++) 
        text[q] -> print(output);
}