#include "complier.h"

int asm(longptr _registers, longptr _variables, const char* filename)
{
	initprogram();
	initstructs();
	char* program_lines[PROGLINES_MAX];
	int total_lines = 0;
	int current_line = 0;
	char line[256];

	char* params[100];
	int pos = 0;

	for (int i = 0; i < SIZE; i++)
	{
		_registers[i] = 0;
		_variables[i] = 0;
	}

	params[0] = " ";

	FILE* fp = fopen(filename, "r");

	if (fp != NULL)
	{
		while (fgets(line, 256, fp))
		{
			if (line[0] != '\n')
			{
				program_lines[current_line] = (char*) malloc(sizeof(char) * strlen(line));
				strcpy(program_lines[current_line], line);
				if ((program_lines[current_line])[strlen(program_lines[current_line]) - 1] == '\n')
				{
					program_lines[current_line][strlen(program_lines[current_line]) - 1] = '\0';
				}
				current_line++;
				total_lines++;
			}
		}
		fclose(fp);
	}
	copyprog(program_lines, total_lines);
	void set_total_lines(total_lines);
	for (int i = 0; i < total_lines;)
	{
		int j = 0;


		while (pos < strlen(program_lines[i]))
		{
			params[j] = a_lexer(program_lines[i], &pos, ' ', ',');
			j++;
		}

		pos = 0;
		set_programcounter(i);
		i = interpreter(_registers, _variables, params, j, NULL);
		//print_regvar(_registers, _variables);
		//printf("%s", program_lines[i]);
	}
}