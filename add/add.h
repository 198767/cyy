#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "ln.h"
#include "vector.h"

#define MAX_BASE 200
#define MIN_BASE 2
#define MACRO2STR(a)  _MACRO2STR(a)
#define _MACRO2STR(a)  #a


int test_base(const char* base);
int test_num(const char* n,int base);
lN get_denominator(const char* num,int b);
lN get_numerator(const char* num,int b);
vector get_decimal_digit(const lN numerator,const lN denominator);
char* get_decimalstr(vector digits,const lN base);
char* add_nums(const char* num1,const char* base1,const char* num2,const char* base2);
void free_digit(vector digits);
int gcd(int a,int b);
