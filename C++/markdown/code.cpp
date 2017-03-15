#include "code.hpp"
#include "errors.hpp"
#include <iostream>
#include <stdlib.h>
#include <cstring>

extern unsigned int width_screen;

Code_text::Code_text()
{
    text = NULL;
    counter = 0;
}

Code_text::~Code_text()
{
    if (text != NULL) 
    {
        for (unsigned int q = 0; q < counter; q++) 
            free(text[q]);
        free(text);
    }
}

void Code_text::push(const char *txt)
{
    if (txt == NULL) 
    {
        text = (char**)realloc(text, (counter + 1) * sizeof(char*));
        text[counter] = (char*)malloc(2);
        text[counter][0] = '\n';
        text[counter][1] = '\0';
    }
    else
    {
        text = (char**)realloc(text, (counter + 1) * sizeof(char*));
        text[counter] = (char*)malloc(strlen(txt) + 1);
        strcpy(text[counter], txt);
    }
    counter++;
}

unsigned long int Code_text::count_symbols()const
{
    unsigned int counter_tmp = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++) {
        j = 0;
        while (text[i][j] != '\0') {
            if ((text[i][j] == '\xd0') || (text[i][j] == '\xd1')) {
                counter_tmp++;
                j += 2;
            }
            else{
                counter_tmp++;
                j++;
            }
        }
    }
    
    return counter_tmp;
}

unsigned long int Code_text::count_words()const
{
    unsigned int counter_tmp = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++) {
        j = 0;
        while (text[i][j] != '\0') {
            while ((text[i][j] != '\n') && (text[i][j] != ' ') && (text[i][j] != '\0')) j++;
            counter_tmp++;
            if (text[i][j] == '\0') break;
            else j++;
        }
    }
    
    return counter_tmp;
}

void Code_text::print(FILE *output)const
{
    bool firstTime = true;
    
    for (unsigned int i = 0; i < counter; i++) {
        for (unsigned int j = 0; (j < strlen(text[i])) && (j < width_screen - 4); j++) {
            if (firstTime) {
                fprintf(output, "@   %c", text[i][j]);
                firstTime = false;
            }
            else fprintf(output, "%c", text[i][j]);
        }
        firstTime = true;
        if ((strlen(text[i]) != 1) && (text[i][0] != '\n')) fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

void Code_text::read(FILE *input)
{
    bool between = false, perexod = false, empty_parag = true;
    unsigned int i = 0;
    char char1 = '\0', *word_tmp = NULL;
    
    do{
        char1 = getc(input);
        switch (char1) {
            case ' ':{
                perexod = false;
                empty_parag = false;
                if (!between) {
                    between = true;
                    word_tmp = (char*)realloc(word_tmp, i + 1);
                    word_tmp[i] = ' ';
                    i++;
                }
                break;
            }
                
            case EOF:{
                if (empty_parag) {
                    ungetc(char1, input);
                    throw EmptyParagraphERROR();
                }
                if (word_tmp != NULL) {
                    word_tmp = (char*)realloc(word_tmp, i + 1);
                    word_tmp[i] = '\0';
                    push(word_tmp);
                    if (word_tmp != NULL) {free(word_tmp); word_tmp = NULL;}
                }
                ungetc(char1, input);
                return;
            }
                
            case '\n':{
                empty_parag = false;
                perexod = true;
                if (word_tmp != NULL) {
                    word_tmp = (char*)realloc(word_tmp, i + 1);
                    word_tmp[i] = '\0';
                    push(word_tmp);
                    if (word_tmp != NULL) {free(word_tmp); word_tmp = NULL;}
                }
                else push(NULL);
                break;
            }
                
            default:{
                empty_parag = false;
                perexod = false;
                between = false;
                word_tmp = (char*)realloc(word_tmp, i + 1);
                word_tmp[i] = char1;
                i++;
                break;
            }
        }
        
        if (perexod) {
            char1 = getc(input);
            if (char1 == EOF) {
                ungetc(char1, input);
                return;
            }
            if (char1 == '\n') {
                return;
            }
            if (char1 != '\t') {
                if (char1 != ' ') {
                    ungetc(char1, input);
                    throw TextProcessingERROR();
                }
                else {
                    for (unsigned int q = 0; q < 3; q++) {
                        char1 = getc(input);
                        if (char1 != ' ') {
                            ungetc(char1, input);
                            for (unsigned int w = 0; w < q; w++) {
                                ungetc(' ', input);
                            }
                            throw TextProcessingERROR();
                        }
                    }
                }
            }
            i = 0;
        }
    }while(1);
}

void Code_text::prints(FILE *output)const
{
    fprintf(output, "^^code^^ ");
    
    bool firstTime = true;
    
    for (unsigned int i = 0; i < counter; i++) {
        for (unsigned int j = 0; j < strlen(text[i]); j++) {
            if (firstTime) {
                fprintf(output, "@   %c", text[i][j]);
                firstTime = false;
            }
            else fprintf(output, "%c", text[i][j]);
        }
        firstTime = true;
        if ((strlen(text[i]) != 1) && (text[i][0] != '\n')) fprintf(output, "\n");
    }
    fprintf(output, "\n");
}