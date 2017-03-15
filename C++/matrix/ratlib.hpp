#ifndef RATLIB_HPP
#define RATLIB_HPP

#include "biginteger.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class RatNum
{
private:
    int sign;
    Big_Integer chislit;
    Big_Integer znam;
public:
    RatNum();
    RatNum(long int);
    RatNum(long int, unsigned long int);
    RatNum(const RatNum&);
    RatNum(const char*);
    RatNum(const char*, const char*);
    
    ~RatNum();
    
    void print()const;
    void check();
    void floor();
    void rat_round();
    void pravilo();
    void rat_print_file(FILE*);
    
    explicit operator int()const;
    explicit operator long()const;
    explicit operator short()const;
    //RatNum operator int()const;
    //RatNum explicit operator long()const;
    //RatNum explicit operator short()const;

    RatNum operator+(const RatNum&)const;
    RatNum operator+(long int)const;
    friend RatNum operator+(long int, const RatNum&);
    
    RatNum operator-(const RatNum&)const;
    RatNum operator-(long int)const;
    friend RatNum operator-(long int, const RatNum&);
    
    RatNum operator*(const RatNum&)const;
    RatNum operator*(long int)const;
    friend RatNum operator*(long int, const RatNum&);
    
    RatNum operator/(const RatNum&)const;
    RatNum operator/(long int)const;
    friend RatNum operator/(long int, const RatNum&);
    
    RatNum operator=(const RatNum&);
    RatNum operator=(long int);
    
    RatNum operator+=(const RatNum&);
    RatNum operator+=(long int);
    
    RatNum operator-=(const RatNum&);
    RatNum operator-=(long int);
    
    RatNum operator*=(const RatNum&);
    RatNum operator*=(long int);
    
    RatNum operator/=(const RatNum&);
    RatNum operator/=(long int);
    
    RatNum operator++()const;
    
    RatNum operator--()const;
    
    bool operator<(const RatNum&)const;
    bool operator<(long int)const;
    friend bool operator<(long int, const RatNum&);
    
    bool operator>(const RatNum&)const;
    bool operator>(long int)const;
    friend bool operator>(long int, const RatNum&);
    
    bool operator<=(const RatNum&)const;
    bool operator<=(long int)const;
    friend bool operator<=(long int, const RatNum&);
    
    bool operator>=(const RatNum&)const;
    bool operator>=(long int)const;
    friend bool operator>=(long int, const RatNum&);
    
    bool operator==(const RatNum&)const;
    bool operator==(long int)const;
    friend bool operator==(long int, const RatNum&);
    
    bool operator!=(const RatNum&)const;
    bool operator!=(long int)const;
    friend bool operator!=(long int, const RatNum&);
};

#endif
