#include "vector.hpp"
#include "ratlib.hpp"
#include "errors.hpp"
#include "matrix.hpp"
#include "elements.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

Vector::Vector()
{
    razmer = 1;
    subject = Elements();
}

Vector::Vector(unsigned long int i, const RatNum &op)
{
    if (i <= 0)
        throw vector_razmer_ERROR();
    
    razmer = i;
    subject = Elements();
    
    if (op == 0)
        return;
    for (unsigned long int k = 1; k <= razmer; k++)
    {
        subject.add_element(1, k, op);
    }
}

Vector::Vector(const Vector &op)
{
    razmer = op.razmer;
    subject = Elements(op.subject);
}

Vector::Vector(FILE *input)
{
    unsigned long int temp, size_tmp = 0;
    long int gps;//координаты columns
    char char1 = '!', *tmp = NULL;
    
    if (input == NULL){
        razmer = 1;
        subject = Elements();
        return;
    }
    
    fscanf(input, "%c", &char1);
    while ((char1 != EOF) && ((char1 == '\n') || (char1 == '#')))
    {
        if (char1 == '#')
        {
            while (char1 != '\n') fscanf(input, "%c", &char1);
            fscanf(input, "%c", &char1);
            continue;
        }
        
        if (char1 == '\n')
            fscanf(input, "%c", &char1);
    }
    
    if (char1 == EOF)
    {
        razmer = 1;
        subject = Elements();
        return;
    }
    
    while (char1 != '\t')
    {
        tmp= (char*)realloc(tmp, size_tmp + 1);
        size_tmp++;
        tmp[size_tmp - 1] = char1;
        if(fscanf(input, "%c", &char1) == EOF)
            char1 = EOF;
    }
    
    tmp = (char*)realloc(tmp, size_tmp + 1);
    size_tmp++;
    tmp[size_tmp - 1] = '\0';
    
    if (strcmp(tmp, "vector") == 0)
    {
        fscanf(input, "%zu", &temp);
        razmer = temp;
        subject = Elements();
        
        fscanf(input, "%c", &char1);
        while (char1 != EOF)
        {
            if (char1 == '#')
            {
                while (char1 != '\n') 
                    fscanf(input, "%c", &char1);
                if(fscanf(input, "%c", &char1) == EOF)
                    char1 = EOF;
                continue;
            }
            gps = -1;
            fscanf(input, "%zu", &gps);
            
            free(tmp);
            tmp = NULL;
            size_tmp = 0;
            if(fscanf(input, "%c", &char1) == EOF)
                char1 = EOF;
            while ((char1 != '\n') && (char1!= EOF))
            {
                if (isdigit(char1) || (char1 == '/'))
                {
                    tmp = (char*)realloc(tmp, size_tmp + 1);
                    size_tmp++;
                    tmp[size_tmp - 1] = char1;
                }
                if(fscanf(input, "%c", &char1) == EOF)
                    char1 = EOF;
            }
            tmp = (char*)realloc(tmp, size_tmp + 1);
            size_tmp++;
            tmp[size_tmp - 1] = '\0';
            if (gps != -1)
                fix(gps, RatNum(tmp));
            
            if (char1 == EOF)
                break;
        }
        free(tmp); 
    }
    else
    {
        free(tmp);
        razmer = 1;
        subject = Elements();
    }
}


Vector::~Vector()
{}


unsigned long int Vector::kolvo_columns()const
{
    return razmer;
}

void Vector::write(FILE *output)
{
    fprintf(output, "vector\t%zu\n\n", razmer);
    
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        fprintf(output, "%zu\t", subject.array[k].j);
        subject.array[k].subject.rat_print_file(output);
        fprintf(output, "\n");
    }
}

void Vector::read(FILE *input)
{
    Vector tmp(input);
    *this = tmp;
}

void Vector::vector_print(FILE *output)
{
    for (unsigned long int j = 1; j <= razmer; j++)
    {
        subject.get(1, j).rat_print_file(output);
        fprintf(output, " ");
    }
    fprintf(output, "\n");
}

void Vector::fix(unsigned long int j, const RatNum &op)
{
    if ((j > razmer) || (j <= 0))
        throw vector_razmer_ERROR();
    
    if ((op == 0) && (subject.get(1, j) == 0))
        return;
    
    if (op == 0)
    {
        subject.delete_element(1, j);
        return;
    }
    
    subject.add_element(1, j, op);
}

void Vector::pravilo()
{
    for (unsigned long int i = 0; i < subject.size; i++)
    {
        subject.array[i].subject.pravilo();
    }
}


Vector Vector::operator+(const Vector &op)const
{
    if ((razmer != op.razmer))
        throw vector_razmer_ERROR();
    
    unsigned long int coordinates;
    Vector result(*this);
    
    for (unsigned long int k = 0; k < op.subject.size; k++)
    {
        coordinates = op.subject.array[k].j;
        result.fix(coordinates, result[coordinates] + op.subject.array[k].subject);
    }
    return result;
}


Vector Vector::operator-(const Vector &op)const
{
    if ((razmer != op.razmer)) throw vector_razmer_ERROR();
    
    unsigned long int coordinates;
    Vector result(*this);
    
    for (unsigned long int k = 0; k < op.subject.size; k++){
        coordinates = op.subject.array[k].j;
        result.fix(coordinates, result[coordinates] - op.subject.array[k].subject);
    }
    return result;
}

Vector Vector::operator-()
{
    Vector result(*this);
    
    for (unsigned long int k = 0; k < subject.size; k++){
        result.subject.array[k].subject *= -1;
    }
    
    return result;
}


Vector Vector::operator*(const RatNum &op)const
{
    Vector result(*this);
    
    for (unsigned long int k = 0; k < subject.size; k++){

        result.subject.array[k].subject *= op;
    }
    return result;
}

Vector Vector::operator*(const Matrix &op)const
{
    if (razmer != op.what_columns())
        throw matrix_razmer_ERROR();
    
    gps_matrix coordinates;
    RatNum tmp;
    Vector result(razmer, RatNum());
    
    for (unsigned long int i = 1; i <= razmer; i++)
    {
        coordinates.column = i;
        tmp = 0;
        for (unsigned long int j = 1; j <= razmer; j++)
        {
            coordinates.row = j;
            tmp += (*this)[j] * op[coordinates];
        }
        result.fix(i, tmp);
    }
    
    return result;
}

/*RatNum Vector::operator*(const Vector &op)const
{
    RatNum tmp;
    Vector result(razmer, RatNum());
    tmp = 0;

    for (unsigned long int i = 1; i <= razmer; i++)
    {
        //tmp = 0;
        for (unsigned long int j = 1; j <= razmer; j++)
        {
            tmp += (*this)[j] * op[i];
        }
        //result.fix(i, tmp);
    }
    return tmp;
}*/

Vector operator*(const RatNum &operand1, const Vector &operand2)
{
    return operand2 * operand1;
}


Vector Vector::operator=(const Vector &op)
{
    razmer = op.razmer;
    subject = op.subject;
    return *this;
}


Vector Vector::operator/(const RatNum &op)const
{
    Vector result(*this);
    
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        result.subject.array[k].subject /= op;
    }
    return result;
}


RatNum Vector::operator[](unsigned long int op)const
{
    if ((op > razmer) || (op <= 0)) 
        throw vector_razmer_ERROR();
    return subject.get(1, op);
}