 #include "list.hpp"
#include "functions.hpp"
#include "errors.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

//extern char marked_symbol;
char marked_symbol;
//extern unsigned int otstup_list, width_screen;
unsigned int otstup_list;
extern unsigned int width_screen;

bool number_pred = false; //предыд. элемент является номером
bool empty_list = false; //пустой элемент списка
unsigned int numListCounter = 1, **otstup_lvl = NULL, size_otstup_array = 0;
extern char *str_global;
//char *str_global;

List_text::List_text()
{
    list = NULL;
    counter = 0;
}

List_text::~List_text()
{
    if (list != NULL)
    {
        for (unsigned int q = 0; q < counter; q++)
            free(list[q].text);
        free(list);
    }
}

void List_text::push(char *txt, unsigned int otstup_tmp, bool type)
{
    if (txt == NULL)
    {
        list = (ListElement*)realloc(list, (counter + 1) * sizeof(ListElement));
        list[counter].text = (char*)malloc(2);
        list[counter].text[0] = '\n';
        list[counter].text[1] = '\0';
    }
    else
    {
        list = (ListElement*)realloc(list, (counter + 1) * sizeof(ListElement));
        list[counter].text = (char*)malloc(strlen(txt) + 1);
        strcpy(list[counter].text, txt);
    }
    list[counter].otstup = otstup_tmp;
    list[counter].listType = type;
    counter++;
}

unsigned long int List_text::count_symbols()const
{
    unsigned int counter_tmp = 0, j = 0;
    
    for (unsigned int i = 0; i < counter; i++)
    {
        j = 0;
        while (list[i].text[j] != '\0')
        {
            if ((list[i].text[j] == '\xd0') || (list[i].text[j] == '\xd1'))
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
    }
    
    return counter_tmp;
}

unsigned long int List_text::count_words()const
{
    unsigned int counter_tmp = 0, j = 0;
    for (unsigned int i = 0; i < counter; i++)
    {
        j = 0;
        while (list[i].text[j] != '\0')
        {
            while ((list[i].text[j] != '\n') && (list[i].text[j] != ' ') && (list[i].text[j] != '\0')) 
                j++;

            counter_tmp++;

            if (list[i].text[j] == '\0') 
                break;
            else 
                j++;
        }
    }
    
    return counter_tmp;
}


void List_text::print(FILE *output)const
{
    //printf("%i\n",width_screen-2);
    //printf("8888484484\n");
    for (unsigned int q = 0; q < counter; q++)
    {
        if (list[q].otstup >= width_screen-2)
            continue;

        prepare(list[q]);

        if (!empty_list)
            fprintf(output, "%s\n", str_global);
        else
            fprintf(output, "%s", str_global);

        free(str_global);
        str_global = NULL;
    }
    for (unsigned int q = 0; q < size_otstup_array; q++) 
        free(otstup_lvl[q]);

    if (otstup_lvl != NULL)
    {
        free(otstup_lvl);
        otstup_lvl = NULL;
    }
    size_otstup_array = 0;
    fprintf(output, "\n");
}

void List_text::read(FILE *input)
{
    bool between = false; //предыд слово
    bool perexod = false;//предыд символ оканчивался на \n
    bool empty_paragr = true, str_numbers = true, str_start = false, first = true;
    unsigned int i = 0, otstup_str = 0;
    char char1 = '\0', *wordtmp = NULL;
    
    do{
        if (!str_start)
        {
            do{
                char1 = getc(input);
                switch (char1)
                {
                    case '*':
                    {
                        str_numbers = false;
                        str_start = true;
                        break;
                    }
                    
                    case ' ':
                    {
                        otstup_str++;
                        break;
                    }
                        
                    case EOF:
                    {
                        if (empty_paragr) 
                        {
                            ungetc(char1, input);
                            throw EmptyParagraphERROR();
                        }
                        else
                        {
                            ungetc(char1, input);
                            return;
                        }
                    }
                        
                    case '\n':
                    {
                        ungetc(char1, input);
                        str_start = true;
                        break;
                    }
                        
                    default:
                    {
                        i = 0;
                        ungetc(char1, input);
                        do{
                            if (isdigit(char1)) 
                            {
                                char1 = getc(input);
                                wordtmp = (char*)realloc(wordtmp, i + 1);
                                wordtmp[i] = char1;
                                i++;
                            }
                            else 
                            {
                                if (char1 != '.') 
                                {
                                    for (unsigned int q = 0; q < i; q++) 
                                    {
                                        ungetc(wordtmp[i - q - 1], input);
                                    }
                                    if (wordtmp != NULL) 
                                    {
                                        free(wordtmp); 
                                        wordtmp = NULL;
                                    }
                                    str_numbers = true;
                                    str_start = true;
                                }
                                else
                                {
                                    if (wordtmp != NULL) 
                                    {
                                        free(wordtmp); 
                                        wordtmp = NULL;
                                    }
                                    str_numbers = true;
                                    str_start = true;
                                }
                            }
                        }while(!str_start);
                        break;
                    }
                }
                i = 0;
            }while(!str_start);
        }
        
        char1 = getc(input);
        
        if (first) 
        {
            if (char1 == ' ') 
                char1 = getc(input);
            first = false;
        }
        
        switch (char1) 
        {
            case ' ':
            {
                perexod = false;
                empty_paragr = false;
                if (!between)
                {
                    between = true;
                    wordtmp = (char*)realloc(wordtmp, i + 1);
                    wordtmp[i] = ' ';
                    i++;
                }
                break;
            }
                
            case EOF:
            {
                if (empty_paragr)
                {
                    ungetc(char1, input);
                    throw EmptyParagraphERROR();
                }
                if (wordtmp != NULL) 
                {
                    wordtmp = (char*)realloc(wordtmp, i + 1);
                    wordtmp[i] = '\0';
                    push(wordtmp, otstup_str, str_numbers);
                    if (wordtmp != NULL) {free(wordtmp); wordtmp = NULL;}
                }
                ungetc(char1, input);
                return;
            }
                
            case '\n':
            {
                empty_paragr = false;
                perexod = true;;
                if (wordtmp != NULL) 
                {
                    wordtmp = (char*)realloc(wordtmp, i + 1);
                    wordtmp[i] = '\0';
                    push(wordtmp, otstup_str, str_numbers);
                    if (wordtmp != NULL) {free(wordtmp); wordtmp = NULL;}
                }
                else push(NULL, otstup_str, 1);
                break;
            }
                
            default:
            {
                empty_paragr = false;
                perexod = false;
                between = false;
                wordtmp = (char*)realloc(wordtmp, i + 1);
                wordtmp[i] = char1;
                i++;
                break;
            }
        }
        
        if (perexod) 
        {
            char1 = getc(input);
            if (char1 == EOF) {
                ungetc(char1, input);
                return;
            }
            if (char1 == '\n') {
                return;
            }
            ungetc(char1, input);
            otstup_str = 0;
            str_start = false;
            str_numbers = true;
            first = true;
            i = 0;
        }
    }while(1);
}

void List_text::prints(FILE *out)const
{
    fprintf(out, "^^list^^ ");
    
    unsigned int i = 0, counterList = 1;
    bool previousIsNum = false;
    
    for (unsigned int j = 0; j < counter; j++)
    {
        for (unsigned int q = 0; q < list[j].otstup; q++) fprintf(out, " ");
        
        if (list[j].listType)
        {
            if (previousIsNum) 
            {
                if (list[j].otstup == list[j - 1].otstup) {
                    counterList ++;
                }
                else counterList = 1;
            }
            previousIsNum = true;
            fprintf(out, "%i. ", counterList);
        }
        else 
        {
            fprintf(out, "*");
            previousIsNum = false;
            counterList = 1;
        }
        
        i = 0;
        while (list[j].text[i] != '\0') 
        {
            fprintf(out, "%c", list[j].text[i]);
            i++;
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void List_text::prepare(ListElement elem)const
{
    char *text = elem.text, *num = NULL;
    unsigned int position = 0, kolvo_str = 0;//колво симв в str
    unsigned int kolvo_print_sym = 0; //в str
    unsigned int lgth_word_tmp = 0, i = 0, extraIndent = 0;
    bool notendword = false;
    
    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
    kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
    
    if (elem.listType) {
        if ((elem.otstup / 2) + 1 > size_otstup_array) {
            otstup_lvl = (unsigned int**)realloc(otstup_lvl, ((elem.otstup / 2) + 1) * sizeof(unsigned int*));
            if ((elem.otstup / 2) + 1 - size_otstup_array > 1) {
                for (unsigned int q = size_otstup_array; q < (elem.otstup / 2); q++) {
                    otstup_lvl[q] = (unsigned int*)malloc(2 * sizeof(unsigned int));
                    otstup_lvl[q][0] = q;
                    otstup_lvl[q][1] = 0;
                }
            }
            otstup_lvl[(elem.otstup / 2)] = (unsigned int*)malloc(2 * sizeof(unsigned int));
            otstup_lvl[(elem.otstup / 2)][0] = (elem.otstup / 2);
            otstup_lvl[(elem.otstup / 2)][1] = 1;
            size_otstup_array = (elem.otstup / 2) + 1;
        }
        else{
            for (unsigned int q = (elem.otstup / 2) + 1; q < size_otstup_array; q++) free(otstup_lvl[q]);
            otstup_lvl = (unsigned int**)realloc(otstup_lvl, ((elem.otstup / 2) + 1) * sizeof(unsigned int*));
            otstup_lvl[(elem.otstup / 2)][1]++;
            size_otstup_array = (elem.otstup / 2) + 1;
        }
        
        numListCounter = otstup_lvl[(elem.otstup / 2)][1];
        num = giveNum();
        //extraIndent = 2 + (unsigned int)strlen(num); //!!
        extraIndent = 0; ///!!
        //printf("kolvo=\n");
        //printf("%i\n",kolvo_str);
        for (unsigned int i = 0; i < strlen(num); i++) {
            if (kolvo_str < width_screen-2) {
                str_global = (char*)realloc(str_global, position + 1);
                str_global[position] = num[i];
                position++;
                kolvo_str++;
                printf("%i\n",kolvo_str);
            }
            else{
                //printf("********\n");
                str_global = (char*)realloc(str_global, position + 1);
                str_global[position] = '\n';
                position++;
                position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
                str_global = (char*)realloc(str_global, position + 1);
                str_global[position] = num[i];
                position++;
                kolvo_str = 1;
            }
        }
        //printf("kolvo=\n");
        //printf("%i\n",kolvo_str);
        if (kolvo_str < (width_screen-2)) {
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
            kolvo_str++;
        }
        else{
            //printf("5555555\n");
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = '\n';
            position++;
            position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
            kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
            kolvo_str++;
        }
        number_pred = true;
        free(num);
        num = NULL;
    }
    else{
        extraIndent = 2;
        //extraIndent = 0;
        for (unsigned int q = 0; q < size_otstup_array; q++) free(otstup_lvl[q]);
        if (otstup_lvl != NULL){
            free(otstup_lvl);
            otstup_lvl = NULL;
        }
        size_otstup_array = 0;
        
        if (kolvo_str < width_screen-2) {
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = marked_symbol;
            position++;
            kolvo_str++;
        }
        else{
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = '\n';
            position++;
            position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
            kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = marked_symbol;
            position++;
            kolvo_str++;
        }
        if (kolvo_str < width_screen-2) {
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
            kolvo_str++;
        }
        else{
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = '\n';
            position++;
            position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
            kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
            kolvo_str++;
        }
        number_pred = false;
    }
    
    
    do{
        if (notendword || (i == 0) || ((text[i] != ' ') && (text[i - 1] == ' '))){
            lgth_word_tmp = lgth_word(text, i);
            if (lgth_word_tmp + kolvo_str <= width_screen-2) {
                kolvo_str += lgth_word_tmp;
                for (unsigned int q = 0; q < lgth_word_tmp; q++) {
                    if ((text[i] == '\xd0') || (text[i] == '\xd1')){
                        str_global = (char*)realloc(str_global, position + 2);
                        str_global[position] = text[i];
                        str_global[position + 1] = text[i + 1];
                        position += 2;
                        i += 2;
                    }
                    else{
                        str_global = (char*)realloc(str_global, position + 1);
                        str_global[position] = text[i];
                        position++;
                        i++;
                    }
                }
                notendword = false;
                continue;
            }
        }
        
        if ((text[i] == '\xd0') || (text[i] == '\xd1')){
            if (kolvo_str < width_screen - 1 -2) {
                if ((kolvo_print_sym == 0) && (width_screen -2 - kolvo_str == 2)) {
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = '\n';
                    position++;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = text[i];
                    str_global[position + 1] = text[i + 1];
                    position += 2;
                    i += 2;
                    kolvo_print_sym = 1;
                    kolvo_str++;
                }
                else{
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = text[i];
                    str_global[position + 1] = text[i + 1];
                    position += 2;
                    i += 2;
                    kolvo_str++;
                    kolvo_print_sym++;
                }
            }
            else{
                if (kolvo_print_sym == 0) {
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = '\n';
                    position++;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = text[i];
                    str_global[position + 1] = text[i + 1];
                    position += 2;
                }
                else{
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = '-';
                    str_global[position + 1] = '\n';
                    position += 2;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = text[i];
                    str_global[position + 1] = text[i + 1];
                    position += 2;
                }
                kolvo_print_sym = 1;
                i += 2;
                kolvo_str = (((elem.otstup / 2) * otstup_list) % (width_screen-2)) + 1;
                notendword = true;
            }
        }
        else{
            if (text[i] == ' ') {
                kolvo_print_sym = 0;
                if (kolvo_str < width_screen-2) {
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = ' ';
                    position++;
                    i++;
                    kolvo_str++;
                }
                else{
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = '\n';
                    position++;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = ' ';
                    position++;
                    i++;
                    kolvo_str++;
                }
                continue;
            }
            
            if (kolvo_str < width_screen-2 - 1) {
                if ((kolvo_print_sym == 0) && (width_screen-2 - kolvo_str == 2)) {
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = '\n';
                    position++;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    kolvo_str = ((elem.otstup / 2) * otstup_list) % (width_screen-2);
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = text[i];
                    position++;
                    i++;
                    kolvo_print_sym = 1;
                    kolvo_str++;
                }
                else{
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = text[i];
                    position++;
                    i++;
                    kolvo_str++;
                    kolvo_print_sym++;
                }
            }
            else{
                if (kolvo_print_sym == 0) {
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = '\n';
                    position++;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = text[i];
                    position++;
                }
                else{
                    str_global = (char*)realloc(str_global, position + 2);
                    str_global[position] = '-';
                    str_global[position + 1] = '\n';
                    position += 2;
                    position = insert_otstup(position, (elem.otstup / 2) * otstup_list + extraIndent);
                    str_global = (char*)realloc(str_global, position + 1);
                    str_global[position] = text[i];
                    position++;
                }
                kolvo_print_sym = 1;
                i++;
                kolvo_str = (((elem.otstup / 2) * otstup_list) % (width_screen-2)) + 1;
                notendword = true;
            }
        }
    }while (i < strlen(text));
    
    str_global = (char*)realloc(str_global, position + 1);
    str_global[position] = '\0';
    return;
}

unsigned int List_text::insert_otstup(unsigned int position, unsigned int otstup)const{
    unsigned int i = 0;
    
    if (otstup <= width_screen-2) {
        str_global = (char*)realloc(str_global, position + otstup);
        for (unsigned int q = 0; q < otstup; q++) {
            str_global[position] = ' ';
            position++;
        }
        return position ;
    }
    else {
        while (otstup != 0) {
            if (i == width_screen-2) {
                str_global = (char*)realloc(str_global, position + 1);
                str_global[position] = '\n';
                position++;
                i = 0;
            }
            str_global = (char*)realloc(str_global, position + 1);
            str_global[position] = ' ';
            position++;
            otstup--;
            i++;
        }
    }
    
    return position;
}

char* List_text::giveNum()const{
    char *integer = NULL;
    
    unsigned int tempInt = numListCounter;
    int ost = 0;
    int i = 0;
    
    while (tempInt > 0) {
        ost = tempInt % 10;
        tempInt /= 10;
        integer = (char*)realloc(integer, (i + 1) * sizeof(char));
        integer[i] = int_to_char(ost);
        i++;
    }
    integer = (char*)realloc(integer, (i + 2) * sizeof(char));
    integer[i] = '.';
    integer[i + 1] = '\0';
    
    return integer;
}

char List_text::int_to_char(int integer)const
{
    return integer + '0';
}