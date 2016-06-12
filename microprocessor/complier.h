#ifndef COMPILER_H
#define COMPILER_H

#include "FUNCTIONS.H"
#include "LEXER.H"
#include "PARSER.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef int**** longptr;


int asm(longptr _registers, longptr _variables, const char* filename);

#endif