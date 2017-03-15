#include "generatematrix.h"
#include "generatevector.h"
//#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
//#include <fstream>


bool vectorFlag = false, matrixFlag = false, diagonalFlag = false, solidFlag = true, limitsFlag = false, frequencyFlag = false, randomFlag = true, noUpperLimitFlag = false;
unsigned long int vectorSize = 100, matrixSizeColumns = 100, matrixSizeRows = 100, frequency = 100, content = 0;
long int upperLimits = 0, lowerLimits = 0;
FILE *output;

int main(int argc, const char* argv[]) {
    char* tmpWord = NULL;
    unsigned int i = 0, l = 0;
    bool customFileFlag = false;
    
    
    if (argc == 0) {
        return 0;
    }
    
    if (((argv[1])[0] == '-') && (argv[1])[1] == 'v') {
        vectorFlag = true;
        if ((argv[1])[2] == '=') {
            for (i = 3; i < strlen(argv[1]); i++) {
                if (isdigit(((argv[1])[i]))) {
                    tmpWord = (char*)realloc(tmpWord, l + 1);
                    tmpWord[l] = (argv[1])[i];
                    l++;
                }
                else break;
            }
            tmpWord = (char*)realloc(tmpWord, l + 1);
            tmpWord[l] = '\0';
            if (tmpWord != NULL) {
                vectorSize = atoi(tmpWord);
                free(tmpWord);
                tmpWord = NULL;
            }
        }
    }
    else{
        if (((argv[1])[0] == '-') && (argv[1])[1] == 'm') {
            matrixFlag = true;
            if ((argv[1])[2] == '=') {
                for (i = 3; i < strlen(argv[1]); i++) {
                    if (isdigit(((argv[1])[i]))) {
                        tmpWord = (char*)realloc(tmpWord, l + 1);
                        tmpWord[l] = (argv[1])[i];
                        l++;
                    }
                    else break;
                }
                tmpWord = (char*)realloc(tmpWord, l + 1);
                tmpWord[l] = '\0';
                if (tmpWord != NULL) {
                    matrixSizeRows = atoi(tmpWord);
                    free(tmpWord);
                    tmpWord = NULL;
                }
                if (i != strlen(argv[1])) {
                    if ((argv[1])[i] == ',') {
                        i++;
                        l = 0;
                        for (; i < strlen(argv[1]); i++) {
                            if (isdigit(((argv[1])[i]))) {
                                tmpWord = (char*)realloc(tmpWord, l + 1);
                                tmpWord[l] = (argv[1])[i];
                                l++;
                            }
                            else break;
                        }
                        tmpWord = (char*)realloc(tmpWord, l + 1);
                        tmpWord[l] = '\0';
                        if (tmpWord != NULL) {
                            matrixSizeColumns = atoi(tmpWord);
                            free(tmpWord);
                            tmpWord = NULL;
                        }
                    }
                }
            }
        }
        else return 0;
    }
    
    for (int j = 2; j < argc; j++) {
        if ((argv[j])[0] == '-') {
            switch ((argv[j])[1]) {
                case 'o':{
                    if (tmpWord != NULL) {
                        free(tmpWord);
                        tmpWord = NULL;
                    }
                    l = 0;
                    if ((argv[j])[2] == '=') {
                        for (i = 3; i < strlen(argv[j]); i++) {
                            tmpWord = (char*)realloc(tmpWord, l + 1);
                            tmpWord[l] = (argv[j])[i];
                            l++;
                        }
                        tmpWord = (char*)realloc(tmpWord, l + 1);
                        tmpWord[l] = '\0';
                        if (tmpWord != NULL) {
                            output = fopen(tmpWord, "wb");
                            if (output != NULL) {
                                customFileFlag = true;
                            }
                            free(tmpWord);
                            tmpWord = NULL;
                        }
                    }
                    break;
                }
                    
                case 'd':{
                    if (matrixFlag) {
                        if (matrixSizeRows == matrixSizeColumns){
                            diagonalFlag = true;
                            solidFlag = false;
                            frequencyFlag = false;
                        }
                    }
                    break;
                }
                    
                case 'l':{
                    limitsFlag = true;
                    randomFlag = true;
                    if (tmpWord != NULL) {
                        free(tmpWord);
                        tmpWord = NULL;
                    }
                    l = 0;
                    if ((argv[j])[2] == '=') {
                        if ((argv[j])[3] == '-') {
                            tmpWord = (char*)realloc(tmpWord, l + 1);
                            tmpWord[l] = (argv[j])[3];
                            l++;
                        }
                        for (i = 3 + l; i < strlen(argv[j]); i++) {
                            if (isdigit(((argv[j])[i]))) {
                                tmpWord = (char*)realloc(tmpWord, l + 1);
                                tmpWord[l] = (argv[j])[i];
                                l++;
                            }
                            else break;
                        }
                        tmpWord = (char*)realloc(tmpWord, l + 1);
                        tmpWord[l] = '\0';
                        if (tmpWord != NULL) {
                            lowerLimits = atoi(tmpWord);
                            free(tmpWord);
                            tmpWord = NULL;
                        }
                        if (i != strlen(argv[j])) {
                            if ((argv[j])[i] == ',') {
                                i++;
                                l = 0;
                                if ((argv[j])[i] == '-') {
                                    tmpWord = (char*)realloc(tmpWord, l + 1);
                                    tmpWord[l] = (argv[j])[3];
                                    l++;
                                }
                                for (i += l; i < strlen(argv[j]); i++) {
                                    if (isdigit(((argv[j])[i]))) {
                                        tmpWord = (char*)realloc(tmpWord, l);
                                        tmpWord[l] = (argv[j])[i];
                                        l++;
                                    }
                                    else break;
                                }
                                tmpWord = (char*)realloc(tmpWord, l + 1);
                                tmpWord[l] = '\0';
                                if (tmpWord != NULL) {
                                    upperLimits = atoi(tmpWord);
                                    free(tmpWord);
                                    tmpWord = NULL;
                                }
                                if (lowerLimits > upperLimits) limitsFlag = false;
                            }
                            else noUpperLimitFlag = true;
                        }
                    }
                    break;
                }
                    
                case 'f':{
                    frequencyFlag = true;
                    diagonalFlag = false;
                    if (tmpWord != NULL) {
                        free(tmpWord);
                        tmpWord = NULL;
                    }
                    l = 0;
                    if ((argv[j])[2] == '=') {
                        for (i = 3; i < strlen(argv[j]); i++) {
                            if (isdigit(((argv[j])[i]))) {
                                tmpWord = (char*)realloc(tmpWord, l + 1);
                                tmpWord[l] = (argv[j])[i];
                                l++;
                            }
                            else break;
                        }
                        tmpWord = (char*)realloc(tmpWord, l + 1);
                        tmpWord[l] = '\0';
                        if (tmpWord != NULL) {
                            frequency = atoi(tmpWord);
                            free(tmpWord);
                            tmpWord = NULL;
                        }
                        if (frequency > 100) frequency = 100;
                    }
                    else frequencyFlag = true;
                    break;
                }
                    
                case 'c':{
                    randomFlag = false;
                    limitsFlag = false;
                    if (tmpWord != NULL) {
                        free(tmpWord);
                        tmpWord = NULL;
                    }
                    l = 0;
                    if ((argv[j])[2] == '=') {
                        if ((argv[j])[3] == '-') {
                            tmpWord = (char*)realloc(tmpWord, l + 1);
                            tmpWord[l] = (argv[j])[3];
                            l++;
                        }
                        for (i = 3 + l; i < strlen(argv[j]); i++) {
                            if (isdigit(((argv[j])[i]))) {
                                tmpWord = (char*)realloc(tmpWord, l + 1);
                                tmpWord[l] = (argv[j])[i];
                                l++;
                            }
                            else break;
                        }
                        tmpWord = (char*)realloc(tmpWord, l + 1);
                        tmpWord[l] = '\0';
                        if (tmpWord != NULL) {
                            content = atoi(tmpWord);
                            free(tmpWord);
                            tmpWord = NULL;
                        }
                    }
                    else randomFlag = true;
                    break;
                }
                default: break;
            }
        }
    }
    
    if (!customFileFlag) {
        output = fopen("output.txt", "wb");
    }
    
    if (vectorFlag) generate_vector();
    else generate_matrix();
    
    return 0;
}
