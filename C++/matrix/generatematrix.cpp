#include "generatematrix.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstdio>

extern bool diagonalFlag, solidFlag, limitsFlag, frequencyFlag, randomFlag, noUpperLimitFlag;
extern unsigned long int matrixSizeColumns, matrixSizeRows, frequency, content;
extern long int upperLimits, lowerLimits;
extern FILE *output;

void generate_matrix(){
    float tmpFrequency = frequency;
    
    srand((unsigned int)time(0));
    fprintf(output, "matrix\t%zu\t%zu\n\n", matrixSizeRows, matrixSizeColumns);
    
    if (randomFlag) {
        if (frequencyFlag) {
            for (unsigned long int i = 0; i < (unsigned long int)(matrixSizeColumns * matrixSizeRows * (tmpFrequency/100)); i++) {
                fprintf(output, "%zu\t", rand()%matrixSizeRows + 1);
                fprintf(output, "%zu\t", rand()%matrixSizeColumns + 1);
                if (limitsFlag) {
                    if (noUpperLimitFlag) fprintf(output, "%li\n", rand() + lowerLimits);
                    else fprintf(output, "%li\n", rand()%upperLimits + lowerLimits);
                }
                else{
                    if (rand()%2) fprintf(output, "%d\n", rand());
                    else fprintf(output, "-%d\n", rand());
                }
            }
        }
        else{
            if (diagonalFlag) {
                for (unsigned long int i = 0; i < matrixSizeRows; i++) {
                    fprintf(output, "%zu\t", i + 1);
                    fprintf(output, "%zu\t", i + 1);
                    if (limitsFlag) {
                        if (noUpperLimitFlag) fprintf(output, "%li\n", rand() + lowerLimits);
                        else fprintf(output, "%li\n", rand()%upperLimits + lowerLimits);
                    }
                    else{
                        if (rand()%2) fprintf(output, "%d\n", rand());
                        else fprintf(output, "-%d\n", rand());
                    }
                }
            }
            else{
                for (unsigned long int i = 0; i < matrixSizeRows; i++) {
                    for (unsigned long int j = 0; j < matrixSizeColumns; j++) {
                        fprintf(output, "%zu\t", i + 1);
                        fprintf(output, "%zu\t", j + 1);
                        if (limitsFlag) {
                            if (noUpperLimitFlag) fprintf(output, "%li\n", rand() + lowerLimits);
                            else fprintf(output, "%li\n", rand()%upperLimits + lowerLimits);
                        }
                        else{
                            if (rand()%2) fprintf(output, "%d\n", rand());
                            else fprintf(output, "-%d\n", rand());
                        }
                    }
                }
            }
        }
    }
    else{
        if (frequencyFlag) {
            for (unsigned long int i = 0; i < (unsigned long int)(matrixSizeColumns * matrixSizeRows * (tmpFrequency/100)); i++) {
                fprintf(output, "%zu\t", rand()%matrixSizeRows + 1);
                fprintf(output, "%zu\t", rand()%matrixSizeColumns + 1);
                fprintf(output, "%li\n", content);
            }
        }
        else{
            if (diagonalFlag) {
                for (unsigned long int i = 0; i < matrixSizeRows; i++) {
                    fprintf(output, "%zu\t", i + 1);
                    fprintf(output, "%zu\t", i + 1);
                    fprintf(output, "%li\n", content);
                }
            }
            else{
                for (unsigned long int i = 0; i < matrixSizeRows; i++) {
                    for (unsigned long int j = 0; j < matrixSizeColumns; j++) {
                        fprintf(output, "%zu\t", i + 1);
                        fprintf(output, "%zu\t", j + 1);
                        fprintf(output, "%li\n", content);
                    }
                }
            }
        }
    }
    
    fclose(output);
}
