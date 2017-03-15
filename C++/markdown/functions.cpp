#include "functions.hpp"
#include "textViewer.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

extern char *filename, list_sym;
extern unsigned int otstup, width_screen, redline;
extern char *optarg;
//char *optarg;

void help()
{
    printf("-f - имя файла с разметкой\n-w - ширина экрана\n");
    printf("-t - отступ для второй и последующих строк списка (число пробелов)\n");
    printf("-m - символ-маркер ненумерованного списка\n-r - величина красной строки\n");
    printf("-h - выдача help с описанием поведения программы\n-v - отображение версии программы\n");
    printf("Параметры по ключам -w,-t,-m,-r - не обязательны. Если параметр не указан, то задаётся значение по умолчанию.\n");
}
           
void scan_arg(char param)
{
    char *tmp = NULL;
    
    if (optarg[0] == '=')
    {
        tmp = (char*)malloc(strlen(optarg));
        tmp[strlen(optarg) - 1] = '\0';
        for (unsigned int i = 1; i < strlen(optarg); i++)
        {
            tmp[i - 1] = optarg[i];
        }
    }
    else
    {
        tmp = (char*)malloc(strlen(optarg) + 1);
        strcpy(tmp, optarg);
    }
    
    switch (param)
    {
        case 'f': filename = (char*)malloc(strlen(tmp) + 1); strcpy(filename, tmp); break;
        case 'w': width_screen = atoi(tmp); break;
        case 't': otstup = atoi(tmp); break;
        case 'm': list_sym = tmp[0]; break;
        case 'r': redline = atoi(tmp); break;
    }
    if (tmp != NULL)
        free(tmp);
}

unsigned int lgth_word(char *str, unsigned int position)
{
    unsigned int i = position, lgth = 0;
    
    while ((str[i] != ' ') && (str[i] != '\n') && (str[i] != '\0'))
    {
        if ((str[i] == '\xd0') || (str[i] == '\xd1'))
            i += 2;
        else
            i++;

        lgth++;
    }
    
    return lgth;
}