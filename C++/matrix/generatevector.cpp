#include "generatevector.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstdio>

extern bool limitsFlag, frequencyFlag, randomFlag, noUpperLimitFlag;
extern unsigned long int vectorSize, frequency, content;
extern long int upperLimits, lowerLimits;
extern FILE *output;

void generate_vector(){
    float tmpFrequency = frequency;
    
    srand((unsigned int)time(0));
    fprintf(output, "vector\t%zu\n\n", vectorSize);
    
    if (randomFlag) {
        if (frequencyFlag) {
            for (unsigned long int i = 0; i < (unsigned long int)(vectorSize * (tmpFrequency/100)); i++) {
                fprintf(output, "%zu\t", rand()%vectorSize + 1);
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
            for (unsigned long int i = 0; i < vectorSize; i++) {
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
    }
    else{
        if (frequencyFlag) {
            for (unsigned long int i = 0; i < (unsigned long int)(vectorSize * (tmpFrequency/100)); i++) {
                fprintf(output, "%zu\t", rand()%vectorSize + 1);
                fprintf(output, "%li\n", content);
            }
        }
        else{
            for (unsigned long int i = 0; i < vectorSize; i++) {
                fprintf(output, "%zu\t", i + 1);
                fprintf(output, "%li\n", content);
            }
        }
    }
    
    fclose(output);
}
