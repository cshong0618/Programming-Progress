#include "CONSTANTS.H"
#include "FUNCTIONS.H"
#include "LEXER.H"
#include "PARSER.H"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clrscr();

int main(int argc, char** argv)
{
	int* _registers;
	int* _variables;

	char* params[100];
	int pos = 0;
	initstructs();
	params[0] = " ";
	_registers = (int*) calloc(SIZE, sizeof(int));
	_variables = (int*) calloc(SIZE, sizeof(int));
	if (argc == 2)
	{
		asm(_registers, _variables, argv[1]);
/*
		for (int i = 0; i < get_total_lines(); i++)
		{
			printf("%s", get_inst(i));
		}
		printf("\n\n");
*/
		//print_regvar(_registers, _variables);
		getch();
		return 0;
	}

	FILE* fp = NULL;
	do
	{
		char input[1000];
	
		print_regvar(_registers, _variables);
		//print_reg(_registers);
		printf(">> ");
		fgets(input, 1000, stdin);
		int i = 0;

		if (input[0] != '\n')
		{
			if (input[strlen(input) - 1] == '\n')
			{
				input[strlen(input) - 1] = '\0';
			}
			while (pos < strlen(input))
			{
				params[i] = a_lexer(input, &pos, ' ', ',');
				//printf("%s\n", params[i]);
				i++;
			}
		}


		fflush(stdin);
		pos = 0;
		if (input[0] != '\n')
		{
			interpret_inst(input);
			set_programcounter(0);
		    interpreter(_registers, _variables, params, i, &fp);
		}
		system("cls");
	} while (_strcmpi(params[0], EXIT) != 0);
		 

	getch();
	return 0;
}

void clrscr()
{
	for (int i = 0; i < 1; printf("\n"), i++);
}