#include "biginteger.hpp"
#include "errors.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>

unsigned long int maximum(unsigned long int op1, unsigned long int op2){
    if (op1 >= op2) return op1;
    else return op2;
}

unsigned long int minimum(unsigned long int op1, unsigned long int op2){
    if (op1 <= op2) return op1;
    else return op2;
}

char int_to_char(int body){return body + '0';}

int char_to_int(char character){return character - '0';}

char* perevorot(char* tmp_char){
    unsigned long int length = strlen(tmp_char), op = length;
    char* tmp_big = (char*)malloc((length + 1) * sizeof(char));
    
    if (tmp_big == NULL) {
        throw memory_allocated_ERROR((length + 1) * sizeof(char));
    }
    tmp_big[length] = '\0';
    
    for (unsigned long int i = 0; i < op; i++) {
        tmp_big[i] = tmp_char[length - 1];
        length--;
    }
    
    return tmp_big;
}

Big_Integer module(const Big_Integer& op){
    
    if (op > Big_Integer("0")) return op;
    else {
        Big_Integer retInt = op;
        retInt.sign_change();
        return retInt;
    }
}


Big_Integer::Big_Integer(){
    sign = 1; //1 = +. 0 = -;
    body = (char*)malloc(2 * sizeof(char));
    if (body == NULL) {
        throw memory_allocated_ERROR(2 * sizeof(char));
    }
    body[0] = '0';
    body[1] = '\0';
}

Big_Integer::Big_Integer(long int op){
    if (!op) {
        sign = 1;
        body = (char*)malloc(2 * sizeof(char));
        if (body == NULL) {
            throw memory_allocated_ERROR(2 * sizeof(char));
        }
        body[0] = '0';
        body[1] = '\0';
        return;
    }
    
    long int tmp_int = op;
    int ost = 0;
    int i = 0;
    
    if (op > 0) sign = 1;
    else {
        sign = 0;
        tmp_int = -tmp_int;
    }
    
    body = NULL;
    while (tmp_int > 0) {
        ost = tmp_int % 10;
        tmp_int /= 10;
        body = (char*)realloc(body, (i + 1) * sizeof(char));
        if (body == NULL) {
            throw memory_allocated_ERROR((i + 1) * sizeof(char));
        }
        body[i] = int_to_char(ost);
        i++;
    }
    body = (char*)realloc(body, (i + 1) * sizeof(char));
    if (body == NULL) {
        throw memory_allocated_ERROR((i + 1) * sizeof(char));
    }
    body[i] = '\0';
}

Big_Integer::Big_Integer(const char* op){
    char* tmp_int;
    if (op[0] == '-') {
        if (!char_to_int(op[1])) {
            sign = 1; //1 = +. 0 = -;
            body = (char*)malloc(2 * sizeof(char));
            if (body == NULL) {
                throw memory_allocated_ERROR(2 * sizeof(char));
            }
            body[0] = '0';
            body[1] = '\0';
            return;
        }
        sign = 0;
        body = (char*)malloc(strlen(op) * sizeof(char));
        if (body == NULL) {
            throw memory_allocated_ERROR((int)strlen(op) * sizeof(char));
        }
        int j = 0;
        for (int i = 1; i < (int)strlen(op); i++) {
            body[j] = op[i];
            j++;
        }
        body[j] = '\0';
        tmp_int = perevorot(body);
        free(body);
        body = tmp_int;
    }
    else{
        if (!char_to_int(op[0])) Big_Integer();
        sign = 1;
        body = (char*)malloc((strlen(op) + 1) * sizeof(char));
        if (body == NULL) {
            throw memory_allocated_ERROR(((int)strlen(op) + 1) * sizeof(char));
        }
        int j = 0;
        for (int i = 0; i < (int)strlen(op); i++) {
            body[j] = op[i];
            j++;
        }
        body[j] = '\0';
        tmp_int = perevorot(body);
        free(body);
        body = tmp_int;
    }
}

Big_Integer::Big_Integer(const Big_Integer& op){
    sign = op.sign;
    body = (char*)malloc((strlen(op.body) + 1) * sizeof(char));
    if (body == NULL) {
        throw memory_allocated_ERROR(((int)strlen(op.body) + 1) * sizeof(char));
    }
    strcpy(body, op.body);
}


Big_Integer::~Big_Integer(){
    free(body);
    body = NULL;
}


void Big_Integer::sign_change(){
    if (sign) sign = 0;
    else sign = 1;
};

void Big_Integer::fix_sign(int tmp_sign){
    sign = tmp_sign;
}

void Big_Integer::print()const{
    char* op = perevorot(body);
    if (sign) printf("%s", op);
    else printf("-%s", op);
    free(op);
};

Big_Integer Big_Integer::what_Big_Integer()const{
    return *this;
};

char* Big_Integer::what_body()const{
    return body;
};

int Big_Integer::what_sign()const{
    return sign;
};

void Big_Integer::delete_0(){
    unsigned long int zeroPosition = 1, i = 0;
    bool flag_delete = 0;
    char* tmp_body = NULL;
    
    while (body[i] != '\0') {
        if (body[i] != '0') flag_delete = 0;
        else {
            if (!flag_delete) {
                zeroPosition = i;
            }
            flag_delete = 1;
        }
        i++;
    }
    
    if (flag_delete) {
        if (!zeroPosition) {
            body = (char*)realloc(body, 2 * sizeof(char));
            if (body == NULL) {
                throw memory_allocated_ERROR(2 * sizeof(char));
            }
            body[0] = '0';
            body[1] = '\0';
            return;
        }
        
        tmp_body = body;
        body = (char*)malloc((zeroPosition + 1) * sizeof(char));
        if (body == NULL) {
            throw memory_allocated_ERROR((zeroPosition + 1) * sizeof(char));
        }
        for (i = 0; i < zeroPosition; i++) {
            body[i] = tmp_body[i];
        }
        body[zeroPosition] = '\0';
        
        free(tmp_body);
    }
}

void Big_Integer::insert_start(int x){
    char *tmp;
    unsigned long int size = strlen(body);
    
    if (body == NULL) return;
    
    if ((size == 1) && (body[0] == '0')){
        body[0] = int_to_char(x);
        return;
    }
    
    tmp = (char*)realloc(body, (size + 2) * sizeof(char));
    if (tmp == NULL) throw memory_allocated_ERROR(size + 2);
    body = tmp;
    
    for (unsigned long int i = 0; i <= size; i++){
        body[size + 1 - i] = body[size - i];
    }
    
    body[0] = int_to_char(x);
}

void Big_Integer::insert_end(int x){
    char *tmp;
    unsigned long int size = strlen(body);
    
    if (body == NULL) return;
    
    tmp = (char*)realloc(body, (size + 2) * sizeof(char));
    if (tmp == NULL) throw memory_allocated_ERROR(size + 2);
    body = tmp;
    body[size] = int_to_char(x);
    body[size + 1] = '\0';
}

int Big_Integer::to_int(){
    Big_Integer maximum((unsigned int)-1);
    int flag = 1, result = 0;
    unsigned long int size = strlen(body);
    
    if (sign == 0) flag = -1;
    
    if (*this * flag > maximum)
        throw types_ERROR();
    
    for (unsigned long int i = 0; i < size; i++){
        if (i == size - 1)
            result += (body[size - 1 - i] - '0');
        else{
            result += (body[size - 1 - i] - '0');
            result *= 10;
        }
    }
    
    result *= flag;
    
    return result;
}

long int Big_Integer::to_long_int(){
    Big_Integer maximum((unsigned long int)-1);
    int result = 0;
    unsigned long int size = strlen(body);
    
    if (sign == 1){
        if (*this > maximum)
            throw types_ERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (body[size - 1 - i] - '0');
            else{
                result += (body[size - 1 - i] - '0');
                result *= 10;
            }
        }
        
    }
    
    if (sign == 0){
        if ((*this) * (-1) > maximum)
            throw types_ERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (body[size - 1 - i] - '0');
            else{
                result += (body[size - 1 - i] - '0');
                result *= 10;
            }
        }
        result *= -1;
    }
    return result;
}

short Big_Integer::to_short(){
    Big_Integer maximum((unsigned short)-1);
    int result = 0;
    unsigned long int size = strlen(body);
    
    if (sign == 1){
        if (*this > maximum)
            throw types_ERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (body[size - 1 - i] - '0');
            else{
                result += (body[size - 1 - i] - '0');
                result *= 10;
            }
        }
        
    }
    
    if (sign == 0){
        if ((*this) * (-1) > maximum)
            throw types_ERROR();
        
        for (unsigned long int i = 0; i < size; i++){
            if (i == size - 1)
                result += (body[size - 1 - i] - '0');
            else{
                result += (body[size - 1 - i] - '0');
                result *= 10;
            }
        }
        result *= -1;
    }
    return result;
}

void Big_Integer::print_file(FILE *output){
    unsigned long int size = strlen(body);
    
    if(sign == 0) fprintf(output, "-");
    
    for (unsigned long int i = 0; i < size; i++){
        fprintf(output, "%c", body[size - 1 - i]);
    }
}


Big_Integer Big_Integer::operator+(const Big_Integer& op)const{
    Big_Integer tmp_int;
    int summ, perenos = 0;
    unsigned long int i, lgth1 = strlen(body), lgth2 = strlen(op.body);
    unsigned long int minlgth = minimum(lgth1, lgth2), maxlgth = maximum(lgth1, lgth2);
    
    if (*this == 0) {
        return op;
    }
    
    if (op == 0) {
        return *this;
    }
    
    if (sign == op.sign) {
        if (sign) tmp_int.sign = 1;
        else tmp_int.sign = 0;
        
        free(tmp_int.body);
        tmp_int.body = NULL;
        
        for (i = 0; i < minlgth; i++) {
            summ = perenos + char_to_int(op.body[i]) + char_to_int(body[i]);
            perenos = summ/10;
            summ %= 10;
            tmp_int.body = (char*)realloc(tmp_int.body, (i + 1) * sizeof(char));
            if (tmp_int.body == NULL) {
                throw memory_allocated_ERROR((i + 1) * sizeof(char));
            }
            tmp_int.body[i] = int_to_char(summ);
            summ = 0;
        }
        for (i = minlgth; i < maxlgth; i++) {
            if (lgth1 > lgth2) {
                summ = perenos + char_to_int(body[i]);
            }
            else {
                summ = perenos + char_to_int(op.body[i]);
            }
            perenos = summ/10;
            summ %= 10;
            tmp_int.body = (char*)realloc(tmp_int.body, (i + 1) * sizeof(char));
            if (tmp_int.body == NULL) {
                throw memory_allocated_ERROR((i + 1) * sizeof(char));
            }
            tmp_int.body[i] = int_to_char(summ);
            summ = 0;
        }
        if (perenos) {
            tmp_int.body = (char*)realloc(tmp_int.body, (maxlgth + 2) * sizeof(char));
            if (tmp_int.body == NULL) {
                throw memory_allocated_ERROR((maxlgth + 2) * sizeof(char));
            }
            tmp_int.body[maxlgth] = int_to_char(perenos);
            tmp_int.body[maxlgth + 1] = '\0';
            return tmp_int;
        }
        
        tmp_int.body = (char*)realloc(tmp_int.body, (maxlgth + 1) * sizeof(char));
        if (tmp_int.body == NULL) {
            throw memory_allocated_ERROR((maxlgth + 1) * sizeof(char));
        }
        tmp_int.body[maxlgth] = '\0';
        
        return tmp_int;
    }
    
    switch (sign) {
        case 1: return tmp_int = *this - module(op);
            
        case 0: return tmp_int = op - module(*this);
            
        default: return Big_Integer();
    }
}

Big_Integer Big_Integer::operator+(long int op)const{
    return *this + Big_Integer(op);
}

Big_Integer operator+(long int op1, const Big_Integer& op2){
    return Big_Integer(op1) + op2;
}


Big_Integer Big_Integer::operator-(const Big_Integer& op)const{
    Big_Integer tmp_int;
    char *op1 = NULL, *op2 = NULL;
    int zaim = 0;
    unsigned long int minlgth = minimum(strlen(body), strlen(op.body)), maxlgth = maximum(strlen(body), strlen(op.body));
    
    if (*this == 0) {
        tmp_int = op;
        tmp_int.sign_change();
        return tmp_int;
    }
    
    if (op == 0) {
        return *this;
    }
    
    if (!sign && op.sign) {
        tmp_int = module(*this) + module(op);
        tmp_int.sign = 0;
        return tmp_int;
    }
    else{
        if (sign && !op.sign) {
            return tmp_int = *this + module(op);
        }
        else{
            
            if (module(*this) >= module(op)) {
                op1 = body;
                op2 = op.body;
                if (!sign) tmp_int.sign_change();
            }
            else{
                op1 = op.body;
                op2 = body;
                if (sign) tmp_int.sign_change();
            }
            for (unsigned long int i = 0; i < minlgth; i++) {
                if (op1[i] >= op2[i]) {
                    tmp_int.body = (char*)realloc(tmp_int.body, (i + 1) * sizeof(char));
                    if (tmp_int.body == NULL) {
                        throw memory_allocated_ERROR((i + 1) * sizeof(char));
                    }
                    tmp_int.body[i] = int_to_char(op1[i] - op2[i]);
                }
                else{
                    unsigned long int j = i + 1;
                    while (j < maxlgth) {
                        if (char_to_int(op1[j]) >= 1) {
                            op1[j] = op1[j] - 1;
                            zaim = 1;
                            break;
                        }
                        else{j++;}
                    }
                    j--;
                    while (j != i) {
                        op1[j] = '9';
                        j--;
                    }
                    tmp_int.body = (char*)realloc(tmp_int.body, (j + 1) * sizeof(char));
                    if (body == NULL) {
                        throw memory_allocated_ERROR((j + 1) * sizeof(char));
                    }
                    tmp_int.body[j] = int_to_char(char_to_int(op1[i]) + 10 - char_to_int(op2[i]));
                }
            }
            
            for (unsigned long int i = minlgth; i < maxlgth; i++) {
                tmp_int.body = (char*)realloc(tmp_int.body, (i + 1) * sizeof(char));
                if (tmp_int.body == NULL) {
                    throw memory_allocated_ERROR((i + 1) * sizeof(char));
                }
                tmp_int.body[i] = op1[i];
            }
            
            tmp_int.body = (char*)realloc(tmp_int.body, (maxlgth + 1) * sizeof(char));
            if (tmp_int.body == NULL) {
                throw memory_allocated_ERROR((maxlgth + 1) * sizeof(char));
            }
            tmp_int.body[maxlgth] = '\0';
            
            tmp_int.delete_0();
            return tmp_int;
        }
    }
}

Big_Integer Big_Integer::operator-(long int op)const{
    return *this - Big_Integer(op);
}

Big_Integer operator-(long int op1, const Big_Integer& op2){
    return Big_Integer(op1) - op2;
}


Big_Integer Big_Integer::operator*(const Big_Integer& op)const{
    Big_Integer tmp_int;
    unsigned long int lgth1 = strlen(body), lgth2 = strlen(op.body);
    int carry = 0;
    char *summ = NULL, *toDelete;
    
    if ((op == 0) || (*this == 0)) return Big_Integer();
    
    if (op == 1) return *this;
    
    summ = (char*)malloc((lgth1 + lgth2 + 1) * sizeof(char));
    for (unsigned long int j = 0; j < lgth1 + lgth2; j++) {
        summ[j] = '0';
    }
    summ[lgth1 + lgth2] = '\0';
    
    for (unsigned long int i = 0; i < lgth2; i++) {
        if (op.body[i] == '0') continue;
        
        for (unsigned long int j = 0; j < lgth1; j++) {
            carry += char_to_int(body[j]) * char_to_int(op.body[i]) + char_to_int(summ[i + j]);
            summ[i + j] = int_to_char(carry % 10);
            carry /= 10;
        }
        if (carry) summ[lgth1 + i] = int_to_char(carry);
        carry = 0;
        
    }
    toDelete = summ;
    summ = perevorot(summ);
    tmp_int = summ;
    free(toDelete);
    free(summ);
    if (*this == 1) return op;
    
    if (sign) {
        if (!op.sign) tmp_int.sign = 0;
    }
    else{
        if (op.sign) tmp_int.sign = 0;
    }
    
    tmp_int.delete_0();
    return tmp_int;
}

Big_Integer Big_Integer::operator*(long int op)const{
    return *this * Big_Integer(op);
}

Big_Integer operator*(long int op1, const Big_Integer& op2){
    return Big_Integer(op1) * op2;
}


Big_Integer Big_Integer::operator/(const Big_Integer& op2)const{
    Big_Integer result, tmp, current, sOp = op2;
    unsigned long int lgth1 = strlen(body);
    bool flag = false;
    
    if (*this == 0) return result;
    
    if (op2 == 0) throw delenie_na_0_ERROR();
    
    if (op2 == 1) return *this;
    
    if (op2 == -1) {
        result = *this;
        result.sign_change();
        return result;
    }
    
    if (*this == op2) return result = 1;
    
    if (module(*this) < module(op2)) return result;
    
    sOp.fix_sign(1);
    
    for(unsigned long int i = 0; i < lgth1;){
        tmp.insert_start(char_to_int(body[lgth1 - 1 - i]));
        i++;
        while (tmp < sOp){
            result.insert_start(0);
            if (i >= lgth1){
                flag = true;
                break;
            }
            tmp.insert_start(char_to_int(body[lgth1 - 1 - i]));
            i++;
        }
        if (flag) break;
        
        int l = 0, r = 10, m, x = 0;
        
        while (l <= r){
            m = (l+r) >> 1;
            current = sOp * m;
            if (tmp >= current){
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }
        
        result.insert_start(x);
        current = sOp * x;
        tmp = tmp - current;
    }
    
    result.delete_0();
    
    if (sign) {
        switch (op2.sign) {
            case 0: {
                result.fix_sign(0);
                break;
            }
                
            default: break;
        }
    }
    else{
        switch (op2.sign) {
            case 1: {
                result.fix_sign(0);
                break;
            }
                
            default: break;
        }
    }
    
    return result;
}

Big_Integer Big_Integer::operator/(long int op)const{
    return *this / Big_Integer(op);
}

Big_Integer operator/(long int op1, const Big_Integer& op2){
    return Big_Integer(op1) / op2;
}


Big_Integer Big_Integer::operator=(const Big_Integer& op){
    if (&op == this) return *this;
    
    sign = op.sign;
    free(body);
    body = NULL;
    body = (char*)malloc((strlen(op.body) + 1) * sizeof(char));
    if (body == NULL) {
        throw memory_allocated_ERROR(((int)strlen(op.body) + 1) * sizeof(char));
    }
    strcpy(body, op.body);
    
    return *this;
}

Big_Integer Big_Integer::operator=(long int op){
    return (*this = Big_Integer(op));
}


Big_Integer Big_Integer::operator+=(const Big_Integer& op){
    return *this = *this + op;
}

Big_Integer Big_Integer::operator+=(long int op){
    return *this = *this + Big_Integer(op);
}


Big_Integer Big_Integer::operator-=(const Big_Integer& op){
    return *this = *this - op;
}

Big_Integer Big_Integer::operator-=(long int op){
    return *this = *this - Big_Integer(op);
}


Big_Integer Big_Integer::operator*=(const Big_Integer& op){
    return *this = *this * op;
}

Big_Integer Big_Integer::operator*=(long int op){
    return *this = *this * Big_Integer(op);
}


Big_Integer Big_Integer::operator/=(const Big_Integer& op){
    return *this = *this / op;
}

Big_Integer Big_Integer::operator/=(long int op){
    return *this = *this / Big_Integer(op);
}


Big_Integer Big_Integer::operator++()const{
    return Big_Integer(*this + 1);
}


Big_Integer Big_Integer::operator--()const{
    return Big_Integer(*this - 1);
}


bool Big_Integer::operator<(const Big_Integer& op)const{
    if (sign > op.sign) return false;
    else {
        if (sign < op.sign) return true;
        else{
            if (strlen(body) > strlen(op.body)) return false;
            else{
                if (strlen(body) < strlen(op.body)) return true;
                else{
                    for (int i = ((int)strlen(body) - 1); i >= 0; i--) {
                        if (body[i] > op.body[i]) return false;
                        else{
                            if (body[i] < op.body[i]) return true;
                        }
                    }
                    return false;
                }
            }
        }
    }
}

bool Big_Integer::operator<(long int op)const{
    return (*this < Big_Integer(op));
}

bool operator<(const int op1, const Big_Integer& op2){
    return (Big_Integer(op1) < op2);
}


bool Big_Integer::operator>(const Big_Integer& op)const{
    if (sign > op.sign) return true;
    else {
        if (sign < op.sign) return false;
        else{
            if (strlen(body) > strlen(op.body)) return true;
            else{
                if (strlen(body) < strlen(op.body)) return false;
                else{
                    for (int i = ((int)strlen(body) - 1); i >= 0; i--) {
                        if (body[i] > op.body[i]) return true;
                        else{
                            if (body[i] < op.body[i]) return false;
                        }
                    }
                    return false;
                }
            }
        }
    }
}

bool Big_Integer::operator>(long int op)const{
    return (*this > Big_Integer(op));
}

bool operator>(long int op1, const Big_Integer& op2){
    return (Big_Integer(op1) > op2);
}


bool Big_Integer::operator<=(const Big_Integer& op)const{
    if ((*this < op) || (*this == op)) return true;
    else return false;
}

bool Big_Integer::operator<=(long int op)const{
    return (*this <= Big_Integer(op));
}

bool operator<=(long int op1, const Big_Integer& op2){
    return (Big_Integer(op1) <= op2);
}


bool Big_Integer::operator>=(const Big_Integer& op)const{
    if ((*this > op) || (*this == op)) return true;
    else return false;
}

bool Big_Integer::operator>=(long int op)const{
    return (*this >= Big_Integer(op));
}

bool operator>=(long int op1, const Big_Integer& op2){
    return (Big_Integer(op1) >= op2);
}


bool Big_Integer::operator==(const Big_Integer& op)const{
    if (sign == op.sign){
        if (strlen(body) == strlen(op.body)) {
            for (int i = ((int)strlen(body) - 1); i >= 0; i--) {
                if (body[i] != op.body[i]) return false;
            }
            return true;
        }
        else return false;
    }
    else return false;
}

bool Big_Integer::operator==(long int op)const{
    return (*this == Big_Integer(op));
}

bool operator==(long int op1, const Big_Integer& op2){
    return (Big_Integer(op1) == op2);
}


bool Big_Integer::operator!=(const Big_Integer& op)const{
    if (*this == op) return false;
    else return true;
}

bool Big_Integer::operator!=(long int op)const{
    return (*this != Big_Integer(op));
}

bool operator!=(long int op1, const Big_Integer& op2){
    return (Big_Integer(op1) != op2);
}


Big_Integer Big_Integer::operator%(const Big_Integer &op)const{
    return *this - (*this/op) * op;
}

Big_Integer Big_Integer::operator%(long int op)const{
    return *this - (*this/op) * op;
}