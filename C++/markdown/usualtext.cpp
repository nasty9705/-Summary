#include "usualtext.hpp"
#include "functions.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

extern unsigned int width_screen, redline;

Usual_text::Usual_text()
{
    text = NULL;
}

Usual_text::~Usual_text()
{
    if (text != NULL)
        free(text);
}

unsigned long int Usual_text::count_symbols()const
{
    unsigned int counter = 0, j = 0;
    while (text[j] != '\0') 
    {
        if ((text[j] == '\xd0') || (text[j] == '\xd1'))
        {
            counter++;
            j += 2;
        }
        else
        {
            counter++;
            j++;
        }
    }
    
    return counter;
}

unsigned long int Usual_text::count_words()const
{
    unsigned int counter = 0, j = 0;    
    while (text[j] != '\0')
    {
        while ((text[j] != '\n') && (text[j] != ' ') && (text[j] != '\0')) 
            j++;
        counter++;
        if (text[j] == '\0')
            break;
        else
            j++;
    }
    
    return counter;
}

void Usual_text::print(FILE *output)const
{
    char *str = prepare();    
    fprintf(output, "%s\n", str);
    free(str);
}

void Usual_text::read(FILE *input)
{
    bool between = false; //предыдущие слова
    bool perexod = false; //предыд. символ оканчивадся на \n
    unsigned int i = 0;
    char char1 = '\0';
    
    do{
        char1 = getc(input);
        switch (char1)
        {
            case ' ':
            {
                perexod = false;
                if (!between) 
                {
                    between = true;
                    text = (char*)realloc(text, i + 1);
                    text[i] = ' ';
                    i++;
                }
                break;
            }
                
            case EOF:
            {
                if (text != NULL)
                {
                    text = (char*)realloc(text, i + 1);
                    text[i] = '\0';
                    return;
                }
                else
                {
                    ungetc(char1, input);
                    throw EmptyParagraphERROR();
                }
            }
                
            case '\n':
            {
                perexod = true;
                text = (char*)realloc(text, i + 1);
                text[i] = '\n';
                i++;
                break;
            }
                
            default:
            {
                perexod = false;
                between = false;
                text = (char*)realloc(text, i + 1);
                text[i] = char1;
                i++;
                break;
            }
        }
        
        if (perexod)
        {
            char1 = getc(input);
            if (char1 == EOF)
            {
                text = (char*)realloc(text, i + 1);
                text[i] = '\0';
                ungetc(char1, input);
                return;
            }
            if (char1 == '\n')
            {
                text = (char*)realloc(text, i + 1);
                text[i] = '\0';
                return;
            }
            ungetc(char1, input);
        }
    }while(1);
}

void Usual_text::prints(FILE *out)const
{
    fprintf(out, "^^usual^^ ");
    
    unsigned int i = 0;
    while (text[i] != '\0')
    {
        fprintf(out, "%c", text[i]);
        i++;
    }
    
    fprintf(out, "\n");
}

char* Usual_text::prepare()const //обработка строки
{
    char *str = NULL;
    unsigned int pointer = 0; //указатель в str
    unsigned int kolvo_str = 0; //колво символов в str
    unsigned int kolvo_in_word = 0; //количество напечатанных символов в слове в str
    unsigned int lgth_word_tmp = 0; //длина текущего слова
    unsigned int i = 0;
    bool notendword = false; //продолжение слова
    
    for (unsigned int i = 0; i < redline; i++) {
        if (kolvo_str < width_screen) {
            str = (char*)realloc(str, pointer + 1);
            str[pointer] = ' ';
            pointer++;
            kolvo_str++;
        }
        else{
            str = (char*)realloc(str, pointer + 2);
            str[pointer] = '\n';
            str[pointer + 1] = ' ';
            pointer += 2;
            kolvo_str = 1;
        }
    }
    
    do{
        if (notendword || (i == 0) || (((text[i] != ' ') && (text[i] != '\n')) && ((text[i - 1] == ' ') || (text[i - 1] == '\n')))){
            lgth_word_tmp = lgth_word(text, i);
            if (lgth_word_tmp + kolvo_str <= width_screen) {
                kolvo_str += lgth_word_tmp;
                for (unsigned int q = 0; q < lgth_word_tmp; q++) {
                    if ((text[i] == '\xd0') || (text[i] == '\xd1')){
                        str = (char*)realloc(str, pointer + 2);
                        str[pointer] = text[i];
                        str[pointer + 1] = text[i + 1];
                        pointer += 2;
                        i += 2;
                    }
                    else{
                        str = (char*)realloc(str, pointer + 1);
                        str[pointer] = text[i];
                        pointer++;
                        i++;
                    }
                }
                notendword = false;
                continue;
            }
        }
        
        if ((text[i] == '\xd0') || (text[i] == '\xd1')){
            if (kolvo_str < width_screen - 1) {
                if ((kolvo_in_word == 0) && (width_screen - kolvo_str == 2)) {
                    str = (char*)realloc(str, pointer + 3);
                    str[pointer] = '\n';
                    str[pointer + 1] = text[i];
                    str[pointer + 2] = text[i + 1];
                    pointer += 3;
                    i += 2;
                    kolvo_in_word = 1;
                    kolvo_str = 1;
                }
                else{
                    str = (char*)realloc(str, pointer + 2);
                    str[pointer] = text[i];
                    str[pointer + 1] = text[i + 1];
                    pointer += 2;
                    i += 2;
                    kolvo_str++;
                    kolvo_in_word++;
                }
            }
            else{
                if (kolvo_in_word == 0) {
                    str = (char*)realloc(str, pointer + 3);
                    str[pointer] = '\n';
                    str[pointer + 1] = text[i];
                    str[pointer + 2] = text[i + 1];
                    pointer += 3;
                }
                else{
                    str = (char*)realloc(str, pointer + 4);
                    str[pointer] = '-';
                    str[pointer + 1] = '\n';
                    str[pointer + 2] = text[i];
                    str[pointer + 3] = text[i + 1];
                    pointer += 4;
                }
                kolvo_in_word = 1;
                i += 2;
                kolvo_str = 1;
                notendword = true;
            }
        }
        else{
            if ((text[i] == ' ') || (text[i] == '\n')) {
                kolvo_in_word = 0;
                
                if (text[i] == '\n') {
                    str = (char*)realloc(str, pointer + 1);
                    str[pointer] = text[i];
                    pointer++;
                    i++;
                    kolvo_str = 0;
                }
                else{
                    if (kolvo_str < width_screen) {
                        str = (char*)realloc(str, pointer + 1);
                        str[pointer] = ' ';
                        pointer++;
                        i++;
                        kolvo_str++;
                    }
                    else{
                        str = (char*)realloc(str, pointer + 2);
                        str[pointer] = '\n';
                        str[pointer + 1] = ' ';
                        pointer += 2;
                        i++;
                        kolvo_str = 1;
                    }
                }
                continue;
            }
            
            if (kolvo_str < width_screen - 1) {
                if ((kolvo_in_word == 0) && (width_screen - kolvo_str == 2)) {
                    str = (char*)realloc(str, pointer + 2);
                    str[pointer] = '\n';
                    str[pointer + 1] = text[i];
                    pointer += 2;
                    i++;
                    kolvo_in_word = 1;
                    kolvo_str = 1;
                }
                else{
                    str = (char*)realloc(str, pointer + 1);
                    str[pointer] = text[i];
                    pointer++;
                    i++;
                    kolvo_str++;
                    kolvo_in_word++;
                }
            }
            else{
                if (kolvo_in_word == 0) {
                    str = (char*)realloc(str, pointer + 2);
                    str[pointer] = '\n';
                    str[pointer + 1] = text[i];
                    pointer += 2;
                }
                else{
                    str = (char*)realloc(str, pointer + 3);
                    str[pointer] = '-';
                    str[pointer + 1] = '\n';
                    str[pointer + 2] = text[i];
                    pointer += 3;
                }
                kolvo_in_word = 1;
                i++;
                kolvo_str = 1;
                notendword = true;
            }
        }
    }while (i < strlen(text));
    
    str = (char*)realloc(str, pointer + 1);
    str[pointer] = '\0';
    
    return str;
}