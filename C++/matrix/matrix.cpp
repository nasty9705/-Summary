#include "matrix.hpp"
#include "errors.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix::Matrix()
{
    kolvo_row = kolvo_column = 1;
    subject = Elements();
}

Matrix::Matrix(unsigned long int i, unsigned long int j)
{
    if ((i <= 0) || (j <= 0))
        throw matrix_razmer_ERROR();
    
    if (i != j)
    {
        kolvo_row = kolvo_column = 1;
        subject = Elements();
        throw matrix_razmer_ERROR();
    }
    
    kolvo_row = i;
    kolvo_column = j;
    subject = Elements();
    
    for (unsigned long int k = 1; k <= kolvo_row; k++)
    {
        subject.add_element(k, k, RatNum(1));
    }
}

Matrix::Matrix(unsigned long int i, unsigned long int j, const RatNum &op)
{
    if ((i <= 0) || (j <= 0))
        throw matrix_razmer_ERROR();
    
    kolvo_row = i;
    kolvo_column = j;
    subject = Elements();
    
    if (op == 0)
        return;
    for (unsigned long int k = 1; k <= kolvo_row; k++)
    {
        for (unsigned long int l = 1; l <= kolvo_column; l++)
        {
            subject.add_element(k, l, op);
        }
    }
}

Matrix::Matrix(const Matrix &op)
{
    kolvo_row = op.kolvo_row;
    kolvo_column = op.kolvo_column;
    subject = Elements(op.subject);
}

Matrix::Matrix(FILE *input)
{
    unsigned long int temp1, lgth_tmp = 0;
    long int gpsRow, gpsColumn;
    char char1 = EOF, *tmp = NULL;
    
    if (input == NULL)
    {
        kolvo_row = kolvo_column = 1;
        subject = Elements();
        return;
    }
    
    fscanf(input, "%c", &char1);
    while ((char1 != EOF) && ((char1 == '\n') || (char1 == '#')))
    {
        if (char1 == '#')
        {
            while (char1 != '\n')
                fscanf(input, "%c", &char1);
            fscanf(input, "%c", &char1);
            continue;
        }
        if (char1 == '\n')
            fscanf(input, "%c", &char1);
    }
    
    if (char1 == EOF)
    {
        kolvo_row = kolvo_column = 1;
        subject = Elements();
        return;
    }
    
    while (char1 != '\t')
    {
        tmp = (char*)realloc(tmp, lgth_tmp + 1);
        lgth_tmp++;
        tmp[lgth_tmp - 1] = char1;
        if(fscanf(input, "%c", &char1) == EOF)
            char1 = EOF;
    }
    
    tmp = (char*)realloc(tmp, lgth_tmp + 1);
    lgth_tmp++;
    tmp[lgth_tmp - 1] = '\0';
    
    if (strcmp(tmp, "matrix") == 0)
    {
        fscanf(input, "%zu", &temp1);
        kolvo_row = temp1;
        
        fscanf(input, "%zu", &temp1);
        kolvo_column = temp1;
        
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
            gpsRow = -1;
            fscanf(input, "%zu", &gpsRow);
            fscanf(input, "%zu", &gpsColumn);
            
            free(tmp);
            tmp = NULL;
            lgth_tmp = 0;
            if(fscanf(input, "%c", &char1) == EOF)
                char1 = EOF;
            while ((char1 != '\n') && (char1!= EOF))
            {
                if (isdigit(char1) || (char1 == '/'))
                {
                    tmp = (char*)realloc(tmp, lgth_tmp + 1);
                    lgth_tmp++;
                    tmp[lgth_tmp - 1] = char1;
                }
                if(fscanf(input, "%c", &char1) == EOF) char1 = EOF;
            }
            tmp = (char*)realloc(tmp, lgth_tmp + 1);
            lgth_tmp++;
            tmp[lgth_tmp - 1] = '\0';
            if (gpsRow != -1) fix(gpsRow, gpsColumn, RatNum(tmp));
            if (char1 == EOF) break;
        }
        free(tmp);
        
    }
    else
    {
        free(tmp);
        kolvo_row = kolvo_column = 1;
        subject = Elements();
    }
}

Matrix::~Matrix()
{}

unsigned long int Matrix::what_rows()const
{
    return kolvo_row;
}

unsigned long int Matrix::what_columns()const
{
    return kolvo_column;
}


void Matrix::write(FILE *output)
{
    fprintf(output, "matrix\t%zu\t%zu\n\n", kolvo_row, kolvo_column);
    
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        fprintf(output, "%zu\t%zu\t", subject.array[k].i, subject.array[k].j);
        subject.array[k].subject.rat_print_file(output);
        fprintf(output, "\n");
    }
}

void Matrix::read(FILE *input)
{
    Matrix tmp(input);
    *this = tmp;
}

void Matrix::matrix_print(FILE *output)
{
    for (unsigned long int i = 1; i <= kolvo_row; i++)
    {
        for (unsigned long int j = 1; j <= kolvo_column; j++)
        {
            if (subject.get(i, j) != 0)
                subject.get(i, j).rat_print_file(output);
            else
                subject.get(i, j).rat_print_file(output);
            fprintf(output, " ");
        }  
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

void Matrix::fix(unsigned long int i, unsigned long int j, const RatNum &op)
{
    if ((i > kolvo_row) || (i <= 0) || (j > kolvo_column) || (j <= 0))
        throw matrix_razmer_ERROR();
    
    if (op == RatNum((char*)"0") && (subject.get(i, j) == RatNum()))
        return;
    
    if (op == RatNum((char*)"0"))
    {
        subject.delete_element(i, j);
        return;
    }
    subject.add_element(i, j, op);
}

void Matrix::pravilo()
{
    for (unsigned long int i = 0; i < subject.size; i++)
    {
        subject.array[i].subject.pravilo();
    }
}


Matrix Matrix::operator+(const Matrix &op)const
{
    if ((kolvo_row != op.kolvo_row) || (kolvo_column != op.kolvo_column))
        throw matrix_razmer_ERROR();
    
    gps_matrix gps;
    Matrix result(*this);

    for (unsigned long int k = 0; k < op.subject.size; k++)
    {
        gps.row = op.subject.array[k].i;
        gps.column = op.subject.array[k].j;
        result.fix(gps.row, gps.column, result[gps] + op.subject.array[k].subject);
    }
    return result;
}


Matrix Matrix::operator-(const Matrix &op)const
{
    if ((kolvo_row != op.kolvo_row) || (kolvo_column != op.kolvo_column))
        throw matrix_razmer_ERROR();
    
    gps_matrix gps;
    Matrix result(*this);

    for (unsigned long int k = 0; k < op.subject.size; k++)
    {
        gps.row = op.subject.array[k].i;
        gps.column = op.subject.array[k].j;
        result.fix(gps.row, gps.column, result[gps] - op.subject.array[k].subject);
    }
    return result;
}

Matrix Matrix::operator-()
{
    Matrix result(*this);
    
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        result.subject.array[k].subject *= -1;
    }
    
    return result;
}


Matrix Matrix::operator*(const RatNum &op)const
{
    Matrix result(*this);
    
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        result.subject.array[k].subject *= op;
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &op)const
{
    if (kolvo_column != op.kolvo_row)
        throw matrix_razmer_ERROR();
    
    gps_matrix gps1, gps2;
    RatNum tmp;
    Matrix result(kolvo_row, op.kolvo_column);
    
    for (unsigned long int i = 1; i <= kolvo_row; i++)
    {
        gps1.row = i;
        for (unsigned long int j = 1; j <= op.kolvo_column; j++)
        {
            tmp = 0;
            gps2.column = j;
            
            for (unsigned long int k = 1; k <= kolvo_column; k++)
            {
                gps1.column = k;
                gps2.row = k;
                /*printf("m1: ");
                (*this)[gps1].print();
                printf("\n");
                printf("m2: ");
                op[gps2].print();
                printf("\n");*/
                tmp = tmp + (*this)[gps1] * op[gps2];
                /*printf("tmp = ");
                tmp.print();
                printf("\n");*/
            }
            //printf("tmp = ");
            //tmp.print();
            //printf("\n");
            result.fix(i, j, tmp);
        }
    }
    return result;
}

Matrix operator*(const RatNum &op1, const Matrix &op2)
{
    return op2 * op1;
}


Matrix Matrix::operator=(const Matrix &op)
{
    kolvo_row = op.kolvo_row;
    kolvo_column = op.kolvo_column;
    subject = op.subject;
    return *this;
}


Matrix Matrix::operator/(const RatNum &op)const
{
    Matrix result(*this);
    
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        result.subject.array[k].subject /= op;
    }
    return result;
}


Matrix Matrix::operator~()const
{
    unsigned long int tmp;
    Matrix result(*this);
    
    tmp = result.kolvo_row;
    result.kolvo_row = result.kolvo_column;
    result.kolvo_column = tmp;
    for (unsigned long int k = 0; k < subject.size; k++)
    {
        tmp = result.subject.array[k].i;
        result.subject.array[k].i = result.subject.array[k].j;
        result.subject.array[k].j = tmp;
    }
    return result;
}

RatNum Matrix::operator[](const gps_matrix &op)const
{
    if ((op.row > kolvo_row) || (op.row <= 0) || (op.column > kolvo_column) || (op.column <= 0))
        throw matrix_razmer_ERROR();
    return subject.get(op.row, op.column);
}

Matrix Matrix::operator[](const gps_row &op)const
{
    if ((op.row <= 0) || (op.row > kolvo_row))
        throw matrix_razmer_ERROR();
    gps_matrix gps;
    gps.row = op.row;
    Matrix result(1, kolvo_column, RatNum());
    for (unsigned long int k = 1; k <= kolvo_column; k++)
    {
        gps.column = k;
        result.fix(1, k, (*this)[gps]);
    }
    return result;
}

Matrix Matrix:: operator[](const gps_column &op)const
{
    if ((op.column <= 0) || (op.column > kolvo_row))
        throw matrix_razmer_ERROR();
    gps_matrix gps;
    gps.column = op.column;
    Matrix result(kolvo_row, 1, RatNum());
    for (unsigned long int k = 1; k <= kolvo_row; k++)
    {
        gps.row = k;
        result.fix(k, 1, (*this)[gps]);
    }
    return result;
}
