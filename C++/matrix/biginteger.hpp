#ifndef BIGINTEGER_HPP
#define BIGINTEGER_HPP

#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class Big_Integer
{
private:
    char* body;
    int sign;
public:
    Big_Integer();
    Big_Integer(long int);
    Big_Integer(const char*);
    Big_Integer(const Big_Integer&);
    
    ~Big_Integer();
    
    void sign_change();
    void fix_sign(int);
    void print()const;
    Big_Integer what_Big_Integer()const;
    char* what_body()const;
    int what_sign()const;
    void delete_0();
    void insert_start(int);
    void insert_end(int);
    int to_int();
    long int to_long_int();
    short to_short();
    void print_file(FILE*);
    
    Big_Integer operator+(const Big_Integer&)const; //класс+класс
    Big_Integer operator+(long int)const; //класс+long int 
    friend Big_Integer operator+(long int, const Big_Integer&); //long int+класс
    
    Big_Integer operator-(const Big_Integer&)const;
    Big_Integer operator-(long int)const;
    friend Big_Integer operator-(long int, const Big_Integer&);
    
    Big_Integer operator*(const Big_Integer&)const;
    Big_Integer operator*(long int)const;
    friend Big_Integer operator*(long int, const Big_Integer&);
    
    Big_Integer operator/(const Big_Integer&)const;
    Big_Integer operator/(long int)const;
    friend Big_Integer operator/(long int, const Big_Integer&);
    
    Big_Integer operator=(const Big_Integer&);
    Big_Integer operator=(long int);
    
    Big_Integer operator+=(const Big_Integer&);
    Big_Integer operator+=(long int);
    
    Big_Integer operator-=(const Big_Integer&);
    Big_Integer operator-=(long int);
    
    Big_Integer operator*=(const Big_Integer&);
    Big_Integer operator*=(long int);
    
    Big_Integer operator/=(const Big_Integer&);
    Big_Integer operator/=(long int);
    
    Big_Integer operator++()const;
    
    Big_Integer operator--()const;
    
    bool operator<(const Big_Integer&)const;
    bool operator<(long int)const;
    friend bool operator<(long int, const Big_Integer&);
    
    bool operator>(const Big_Integer&)const;
    bool operator>(long int)const;
    friend bool operator>(long int, const Big_Integer&);
    
    bool operator<=(const Big_Integer&)const;
    bool operator<=(long int)const;
    friend bool operator<=(long int, const Big_Integer&);
    
    bool operator>=(const Big_Integer&)const;
    bool operator>=(long int)const;
    friend bool operator>=(long int, const Big_Integer&);
    
    bool operator==(const Big_Integer&)const;
    bool operator==(long int)const;
    friend bool operator==(long int, const Big_Integer&);
    
    bool operator!=(const Big_Integer&)const;
    bool operator!=(long int)const;
    friend bool operator!=(long int, const Big_Integer&);
    
    Big_Integer operator%(const Big_Integer&)const;
    Big_Integer operator%(long int temp)const;
};

unsigned long int maximum(unsigned long int, unsigned long int);
unsigned long int minimum(unsigned long int, unsigned long int);
char int_to_char(int);
int char_to_int(char);
char* perevorot(char*);
Big_Integer module(const Big_Integer&);

#endif
