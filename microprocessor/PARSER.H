#ifndef PARSER_H
#define PARSER_H

#include "CONSTANTS.H"
#include "FUNCTIONS.H"
#include "complier.h"

struct PROGRAM
{
	int program_counter;
	int current_jmp_size;
	int total_lines;
	int if_counter;
	int while_counter;
};

/*
	lines denotes the next expression or the current comparison statement??
*/
struct IFPAIR
{
	int if_line;
	int else_line;
	int endif_line;
	int* jump_line;
	int skip_else;
	struct IFPAIR* nested_if;
};

struct JMPPAIR
{
	int label_line;
	char* label_name;
};

struct VARIABLE
{
	char* name;
	int location;
	int val;
};

struct WHILEPAIR
{
	int dowhile_line;
	int enddo_line;
	int loop;
};

struct SMARTRACK
{
	int smartreg[SIZE];
	int reg_count;
	int smartvar[SIZE];
	int var_count;
};


int interpreter(int** _registers, int** _variables, char* _params[], int size, void** file);

int switch_inst(const char* inst);
void initstructs(void);
void initprog(void);
void copyprog(char* prog[], int size);
void set_programcounter(int line);
void set_total_lines(int lines);
int get_total_lines(void);
char* get_inst(int i);

#endif