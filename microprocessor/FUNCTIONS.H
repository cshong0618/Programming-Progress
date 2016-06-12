#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

void print_reg(int* reg);
void print_regvar(int* reg, int* var);
void print_regvarx(int* reg, int* var);
void print_regxvarx(int* reg, int* var);


int load(int LOC, int*** Ra);
int load_i(int*** LOC, int index, int*** Ra);
int store(int Ra, int*** LOC);
int store_i(int*** Ra, int index, int*** LOC);
int add(int*** Ra, int Rb, int Rc);
int mult(int*** Ra, int Rb, int Rc);
int divi(int*** Ra, int Rb, int Rc);
int comp(int oper_1, int oper_2, int code);

int reset_mem(int*** mem);
int set_var(int*** LOC, int num);

FILE* openfile(const char* fulldirectory);
int set_if(int line_num);
int set_else(int line_num);
int set_endif(int line_num);
char* set_jump(const char* label, int* line_num);
void jump(const char* label);
void display(const char* input);
void print(int var);
void newline(void);
void space(void);
void f_print(char* s_param, int val[]);
void readinput(int*** LOC);


#endif