#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "elements.hpp"
#include "ratlib.hpp"
#include "biginteger.hpp"

struct gps_matrix
{
    unsigned long int row, column;
    
    gps_matrix()
    {
        row = column = 1;
    }
    gps_matrix(unsigned long int a, unsigned long int b)
    {
        row = a;
        column = b;
    }
};

struct gps_row
{
    unsigned long int row;
    
    gps_row()
    {
        row = 1;
    }
    gps_row(unsigned long int a)
    {
        row = a;
    }
};

struct gps_column
{
    unsigned long int column;
    
    gps_column()
    {
        column = 1;
    }
    gps_column(unsigned long int a)
    {
        column = a;
    }
};

class Matrix
{
protected:
    unsigned long int kolvo_row;
    unsigned long int kolvo_column;
    Elements subject;
public:
    Matrix();
    Matrix(unsigned long int, unsigned long int);
    Matrix(unsigned long int, unsigned long int, const RatNum&);
    Matrix(const Matrix&);
    Matrix(FILE*);
    
    ~Matrix();
    
    unsigned long int what_rows()const;
    unsigned long int what_columns()const;
    
    void read(FILE*);
    void write(FILE*);
    void matrix_print(FILE*);
    void fix(unsigned long int, unsigned long int, const RatNum&);
    void pravilo();
    
    
    Matrix operator+(const Matrix&)const;
    
    Matrix operator-(const Matrix&)const;
    Matrix operator-();
    
    Matrix operator*(const RatNum&)const;
    Matrix operator*(const Matrix&)const;
    friend Matrix operator*(const RatNum&, const Matrix&);
    
    Matrix operator=(const Matrix&);
    
    Matrix operator/(const RatNum&)const;
    
    Matrix operator~()const;
    
    RatNum operator[](const gps_matrix&)const;
    Matrix operator[](const gps_row&)const;
    Matrix operator[](const gps_column&)const;
};

#endif