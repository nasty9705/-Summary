#include "ratlib.hpp"
#include "errors.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>


RatNum::RatNum()
{
    sign = 1; //+
	chislit = "0";
    znam = "0";
}

RatNum::RatNum(long int a)
{
    if (!a)
    {
        RatNum(); 
        return;
    }
    chislit = a;
    znam = 1;
    sign = chislit.what_sign();
}

RatNum::RatNum(long int verx, unsigned long int niz)
{
    if (!niz)
    {
        if (!verx) 
        {
            RatNum();
            return;
        }
        else
            throw znam_ERROR();
    }
    else
    {
        if (!verx)
        {
            RatNum();
            return;
        }
    }
    chislit = verx;
    znam = niz;
    sign = chislit.what_sign();
}

RatNum::RatNum(const RatNum& op)
{
    sign = op.sign;    
    chislit = op.chislit;
    znam = op.znam;
}

RatNum::RatNum(const char* op)
{
    char* str = NULL;
    unsigned long int i = 0;
    unsigned long int size = 0;//размер str
    
    while ((op[i] != '/') && (op[i] != '\0'))
    {
        str = (char*)realloc(str, size + 1);
        str[size] = op[i];
        size++;
        i++;
    }
    
    if (op[i] == '\0')
    {
        chislit = op;
        znam = 1;
        sign = chislit.what_sign();
        free(str);
        str = NULL;
    }
    else
    {
        str = (char*)realloc(str, size + 1);
        str[size] = '\0';
        size = 0;
        chislit = str;
        sign = chislit.what_sign();
        free(str);
        str = NULL;
        i++;
        while (op[i] != '\0')
        {
            if (size == 0) str = (char*)malloc(sizeof(char));
            else{ 
            str = (char*)realloc(str, size + 1);}
            str[size] = op[i];
            size++;
            i++;
        }
        str = (char*)realloc(str, size + 1);
        str[size] = '\0';
        znam = str;
        free(str);
    }
    
    return;
}

RatNum::RatNum(const char* verx, const char* niz)
{
    if (!char_to_int(niz[0]))
    {
        if (verx[0] == '-')
        {
            if (!char_to_int(verx[1]))
            {
                RatNum();
                return;
            }
            else
                throw znam_ERROR();
        }
        else
        {
            if (!char_to_int(verx[0]))
            {
                RatNum();
                return;
            }
            else
                throw znam_ERROR();
        }
    }
    else
    {
        if (verx[0] == '-')
        {
            if (!char_to_int(verx[1]))
            {
                RatNum();
                return;
            }
        }
        else
        {
            if (!char_to_int(verx[0]))
            {
                RatNum();
                return;
            }
        }
    }
    
    chislit = verx;
    znam = niz;
    sign = chislit.what_sign();
}


RatNum::~RatNum()
{}


void RatNum::print()const
{
    if ((znam == 1) || (chislit == 0))
    {
        chislit.print();
        return;
    }
    else
    {
        chislit.print();
        printf("/");
        znam.print();
        return;
    }
    
}

void RatNum::check()
{
    if (chislit == 0)
        znam = 0;
    sign = 1;
}

void RatNum::floor()
{
    if(*this == RatNum())
        return;
    
    chislit = chislit / znam;
    if (chislit == 0)
        znam = 0;
    else znam = 1;
}

void RatNum::rat_round()
{
    if(*this == RatNum())
        return;
    
    RatNum tmp;
    tmp.chislit = chislit % znam;
    tmp.znam = znam;
    
    if (tmp >= RatNum(1, 2))
        chislit += tmp.znam - tmp.chislit;
    else
        chislit -= tmp.chislit;
    
    if (chislit == 0)
        znam = 0;
}

void RatNum::pravilo()
{
    bool sign = 1;
    Big_Integer tmp;
    Big_Integer verx(chislit);
    Big_Integer niz(znam);
    
    if (chislit < 0)
    {
        chislit.fix_sign(1);
        sign = 0;
    }
    
    if (*this == RatNum())
        return;
    /*printf("verx = ");
    printf("%s\n",verx.what_body());
    printf("niz = ");
    printf("%s\n",niz.what_body());
    verx = verx % niz;
    printf("verx = ");
    printf("%s\n", verx.what_body());
    printf("33333\n");*/
    while ((verx != 0) && (niz != 0))
    {
        //printf("77777\n");
        if (verx > niz)
            verx = verx % niz;
        else niz = niz % verx;
        //printf("verx = ");
        //printf("%s\n",verx.what_body());
        //printf("niz = ");
        //printf("%s\n",niz.what_body());
    }

    tmp = verx + niz;
    chislit /= tmp;
    znam /= tmp;
    chislit.fix_sign(sign);
}

void RatNum::rat_print_file(FILE *out)
{
    if ((chislit == 0) || (znam == 1))
    {
        chislit.print_file(out);
        return;
    }
    chislit.print_file(out);
    fprintf(out, "/");
    znam.print_file(out);
}


RatNum::operator int()const
{
    if(*this == RatNum())
        return 0;
    return (chislit / znam).to_int();
}

RatNum::operator long()const
{
    if(*this == RatNum())
        return 0;
    return (chislit / znam).to_long_int();
}

RatNum::operator short() const
{
    if(*this == RatNum())
        return 0;
    return (chislit / znam).to_short();
}


RatNum RatNum::operator+(const RatNum& op)const
{
    RatNum temp;

    if (*this == 0)
        return op;
    if (op == 0)
        return *this;
    
    temp.znam = znam * op.znam;
    temp.chislit = chislit * op.znam + op.chislit * znam;
    temp.sign = temp.chislit.what_sign();
    temp.check();
    
    return temp;
}

RatNum RatNum::operator+(long int op)const
{
    return *this + RatNum(op);
}

RatNum operator+(long int op1, const RatNum& op2)
{
    return RatNum(op1) + op2;
}


RatNum RatNum::operator-(const RatNum& op)const
{
    RatNum temp;
    
    temp.znam = znam * op.znam;
    temp.chislit = chislit * op.znam - op.chislit * znam;
    temp.sign = temp.chislit.what_sign(); 
    temp.check();
    
    return temp;
}

RatNum RatNum::operator-(long int op)const
{
    return *this - RatNum(op);
}

RatNum operator-(long int op1, const RatNum& op2)
{
    return RatNum(op1) - op2;
}


RatNum RatNum::operator*(const RatNum& op)const
{
    RatNum temp; 
    temp.znam = znam * op.znam;
    temp.chislit = chislit * op.chislit;
    temp.sign = temp.chislit.what_sign();
    temp.check();
    
    return temp;
}

RatNum RatNum::operator*(long int op)const
{
    return *this * RatNum(op);
}

RatNum operator*(long int op1, const RatNum& op2)
{
    return RatNum(op1) * op2;
}


RatNum RatNum::operator/(const RatNum& op)const
{
    RatNum temp;
    if (op == (int)0)
    {
        throw delenie_na_0_ERROR();
    } 
    temp.znam = znam * op.chislit;
    temp.chislit = chislit * op.znam;
    temp.sign = temp.chislit.what_sign();
    temp.check();
    
    return temp;
}

RatNum RatNum::operator/(long int op)const
{
    return *this / RatNum(op);
}

RatNum operator/(long int op1, const RatNum& op2)
{
    return RatNum(op1) / op2;
}


RatNum RatNum::operator=(const RatNum& op)
{
    chislit = op.chislit;
    znam = op.znam;
    sign = op.sign;  
    return *this;
}

RatNum RatNum::operator=(long int op)
{
    return (*this = RatNum(op));
}


RatNum RatNum::operator+=(const RatNum& op)
{
    if (*this == 0)
        return *this = op;
    if (op == 0)
        return *this;
    return *this = *this + op;
}

RatNum RatNum::operator+=(long int op)
{
    return *this = *this + RatNum(op);
}


RatNum RatNum::operator-=(const RatNum& op)
{
    return *this = *this - op;
}

RatNum RatNum::operator-=(long int op)
{
    return *this = *this - RatNum(op);
}


RatNum RatNum::operator*=(const RatNum& op)
{
    return *this = *this * op;
}

RatNum RatNum::operator*=(long int op)
{
    return *this = *this * RatNum(op);
}


RatNum RatNum::operator/=(const RatNum& op)
{
    return *this = *this / op;
}

RatNum RatNum::operator/=(long int op)
{
    return *this = *this / RatNum(op);
}


RatNum RatNum::operator++()const
{
    return RatNum(*this + 1);
}


RatNum RatNum::operator--()const
{
    return RatNum(*this - 1);
}


bool RatNum::operator<(const RatNum& op)const
{
    Big_Integer op1, op2;
    op1 = chislit * op.znam;
    op2 = op.chislit * znam;
    
    return op1 < op2;
    
}

bool RatNum::operator<(long int op)const
{
    return (*this < RatNum(op));
}

bool operator<(long int op1, const RatNum& op2)
{
    return (RatNum(op1) < op2);
}


bool RatNum::operator>(const RatNum& op)const
{
    Big_Integer op1, op2;
    op1 = chislit * op.znam;
    op2 = op.chislit * znam;
    
    return op1 > op2;
}

bool RatNum::operator>(long int op)const
{
    return (*this > RatNum(op));
}

bool operator>(long int op1, const RatNum& op2){
    return (RatNum(op1) > op2);
}


bool RatNum::operator<=(const RatNum& op)const
{
    if ((*this < op) || (*this == op))
        return true;
    else
        return false;
}

bool RatNum::operator<=(long int op)const
{
    return (*this <= RatNum(op));
}

bool operator<=(long int op1, const RatNum& op2)
{
    return (RatNum(op1) <= op2);
}


bool RatNum::operator>=(const RatNum& op)const
{
    if ((*this > op) || (*this == op))
        return true;
    else
        return false;
}

bool RatNum::operator>=(long int op)const
{
    return (*this >= RatNum(op));
}

bool operator>=(long int op1, const RatNum& op2)
{
    return (RatNum(op1) >= op2);
}


bool RatNum::operator==(const RatNum& op)const
{
    Big_Integer op1, op2; 
    if (op.chislit == 0) {
        if (chislit == 0) return true;
        else return false;
    } 
    op1 = chislit * op.znam;
    op2 = op.chislit * znam;
    
    return op1 == op2;
}

bool RatNum::operator==(long int op)const
{
    return (*this == RatNum(op));
}

bool operator==(long int op1, const RatNum& op2)
{
    return (RatNum(op1) == op2);
}


bool RatNum::operator!=(const RatNum& op)const
{
    if (*this == op) return false;
    else return true;
}

bool RatNum::operator!=(long int op)const
{
    return (*this != RatNum(op));
}

bool operator!=(long int op1, const RatNum& op2)
{
    return (RatNum(op1) != op2);
}
