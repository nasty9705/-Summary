#include "header.hpp"
#include "functions.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

extern unsigned int width_screen;

char *str_global = NULL;

Header_text::Header_text()
{
    text = NULL;
    lvl = 0;
}

Header_text::~Header_text()
{
    if (text != NULL)
        free(text);
}

unsigned long int Header_text::count_symbols()const
{
    unsigned int counter_tmp = 0, j = 0;
    
    while (text[j] != '\0') 
    {
        if ((text[j] == '\xd0') || (text[j] == '\xd1')) 
        {
            counter_tmp++;
            j += 2;
        }
        else
        {
            counter_tmp++;
            j++;
        }
    }
    
    return counter_tmp;
}

unsigned long int Header_text::count_words()const
{
    unsigned int counter_tmp = 0, j = 0;
    
    while (text[j] != '\0') 
    {
        while ((text[j] != '\n') && (text[j] != ' ') && (text[j] != '\0')) 
            j++;

        counter_tmp++;

        if (text[j] == '\0') 
            break;
        else j++;
    }
    
    return counter_tmp;
}

void Header_text::print(FILE *output)const
{
    if (width_screen < 2 * lvl + 4) return;
    
    try {
        prepare();
    } catch (TextProcessingERROR) {
        if (str_global != NULL) {
            free(str_global);
            str_global = NULL;
        }
        return;
    }
    fprintf(output, "%s\n\n", str_global);
    
    if (str_global != NULL) {
        free(str_global);
        str_global = NULL;
    }
}

void Header_text::read(FILE *input)
{
    char char1 = '\0';
    bool betweenWords = false;
    unsigned int i = 0;
    
    char1 = getc(input);
    while (char1 == '#') {
        lvl++;
        char1 = getc(input);
    }
    
    if (char1 == ' ') betweenWords = true;
    else ungetc(char1, input);
    
    do{
        char1 = getc(input);
        switch (char1) {
            case ' ':{
                if (!betweenWords) {
                    betweenWords = true;
                    text = (char*)realloc(text, i + 1);
                    text[i] = ' ';
                    i++;
                }
                break;
            }
                
            case EOF:{
                if (text != NULL) {
                    text = (char*)realloc(text, i + 1);
                    text[i] = '\0';
                    return;
                }
                else {
                    ungetc(char1, input);
                    throw EmptyParagraphERROR();
                }
            }
                
            case '\n':{
                if (text != NULL) {
                    text = (char*)realloc(text, i + 1);
                    text[i] = '\0';
                    return;
                }
                else throw EmptyParagraphERROR();
            }
                
            default:{
                betweenWords = false;
                text = (char*)realloc(text, i + 1);
                text[i] = char1;
                i++;
                break;
            }
        }
    }while(1);
}

void Header_text::prints(FILE *out)const
{
    fprintf(out, "^^header^^ ");
    
    for (unsigned int i = 0; i < lvl; i++) fprintf(out, "#");
    fprintf(out, " ");
    unsigned int i = 0;
    while (text[i] != '\0') {
        fprintf(out, "%c", text[i]);
        i++;
    }
    
    fprintf(out, "\n\n");
}

void Header_text::prepare()const{
    unsigned int position = 0; //в str
    unsigned int lgth_word_tmp = 0, width_screen_tmp = 0, kolvo_for_arraywords = 0, k = 0, length = 0, q = 0, a = 0,  i = 0;
    char **arrayOfWords = NULL;
    bool tmpBool = false;
    
    while (text[i] != '\0') {
        if (text[i] == ' ') {
            arrayOfWords[kolvo_for_arraywords] = (char*)realloc(arrayOfWords[kolvo_for_arraywords], k + 1);
            arrayOfWords[kolvo_for_arraywords][k] = '\0';
            kolvo_for_arraywords++;
            i++;
            k = 0;
            if (text[i] != '\0') {
                arrayOfWords = (char**)realloc(arrayOfWords, (kolvo_for_arraywords + 1) * sizeof(char*));
                arrayOfWords[kolvo_for_arraywords] = NULL;
            }
        }
        else{
            if (i == 0) {
                arrayOfWords = (char**)realloc(arrayOfWords, (kolvo_for_arraywords + 1) * sizeof(char*));
                arrayOfWords[kolvo_for_arraywords] = NULL;
            }
            arrayOfWords[kolvo_for_arraywords] = (char*)realloc(arrayOfWords[kolvo_for_arraywords], k + 1);
            arrayOfWords[kolvo_for_arraywords][k] = text[i];
            i++;
            k++;
        }
    }
    
    arrayOfWords[kolvo_for_arraywords] = (char*)realloc(arrayOfWords[kolvo_for_arraywords], k + 1);
    arrayOfWords[kolvo_for_arraywords][k] = '\0';
    
    k = 0;
    i = 0;
    
    str_global = (char*)realloc(str_global, width_screen + 1);
    for (unsigned int q = 0; q < width_screen; q++) {
        str_global[q] = '#';
    }
    position = width_screen + 1;
    str_global[position - 1] = '\n';
    
    width_screen_tmp = width_screen - 2 * lvl;
    
    
    do{
        position = insertResh(position);
        for (q = k; q < kolvo_for_arraywords + 1; q++) {
            length += lgth_word(arrayOfWords[q], 0) + 1;
            if (q == kolvo_for_arraywords) length--;
            k++;
            
            if (length > width_screen_tmp) {
                length -= lgth_word(arrayOfWords[q], 0) + 2;
                k--;
                if (q == kolvo_for_arraywords) length++;
                break;
            }
        }
        
        if (q == a) {
            if (arrayOfWords != NULL) {
                for (unsigned int z = 0; z < kolvo_for_arraywords + 1; z++) {
                    free(arrayOfWords[z]);
                }
                free(arrayOfWords);
            }
            
            throw TextProcessingERROR();
        }
            
        for (unsigned int w = 0; w < (width_screen_tmp - length)/2; w++) {
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
        }
        
        for (unsigned int t = a; t < q; t++) {
            lgth_word_tmp = lgth_word(arrayOfWords[t], 0);
            i = 0;
            if (t != a) {
                str_global = (char*)realloc(str_global, position + 1);
                str_global[position] = ' ';
                position++;
            }
            
            do {
                if ((arrayOfWords[t][i] == '\xd0') || (arrayOfWords[t][i] == '\xd1')){
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = arrayOfWords[t][i];
                    str_global[position + 1] = arrayOfWords[t][i + 1];
                    position += 2;
                    i += 2;
                    if (i/2 == lgth_word_tmp) break;
                }
                else{
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = arrayOfWords[t][i];
                    position++;
                    i++;
                    if (i == lgth_word_tmp) break;
                }
            }while(1);
        }
        
        for (unsigned int w = 0; w < (width_screen_tmp - length)/2; w++) {
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
        }
        
        length = 0;
        
        position = insertResh(position);
        str_global = (char*)realloc(str_global, position + 1);
        str_global[position] = '\n';
        position++;
        a = q;
    }while (q - 1 != kolvo_for_arraywords);

    str_global = (char*)realloc(str_global, position + width_screen + 1);
    for (unsigned int q = position; q < position + width_screen; q++) {
        str_global[q] = '#';
    }
    position += width_screen;
    str_global[position] = '\0';
    
    if (arrayOfWords != NULL) {
        for (unsigned int z = 0; z < kolvo_for_arraywords + 1; z++) {
            free(arrayOfWords[z]);
        }
        free(arrayOfWords);
    }
    
    return;
}

unsigned int Header_text::insertResh(unsigned int position)const
{
    unsigned int headerLvl = lvl;
    
    str_global = (char*)realloc(str_global, position + headerLvl);
    for (unsigned int q = 0; q < headerLvl; q++) {
        str_global[position] = '#';
        position++;
    }
    return position;
}