#include "CONSTANTS.H"
#include "FUNCTIONS.H"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

void print_reg(int* reg)
{
	for (int i = 0; i < SIZE / 3; i++)
	{
		int trioffset = (SIZE / 3) + i;
		int trioffset2 = 2 * (SIZE / 3) + i;
		printf("Register #%-3d --> %10d\tRegister #%-3d --> %10d\tRegister #%-3d --> %10d\n", 
			   i, reg[i], trioffset, reg[trioffset], trioffset2, reg[trioffset2]);
	}
}


void print_regvar(int* reg, int* var)
{
	for (int i = 0; i < SIZE; i++)
	{
		printf("Register #%-3d --> %20d\t\tMemory #%-3d --> %20d\n", i, reg[i], i, var[i]);
	}
}

void print_regvarx(int* reg, int* var)
{
	for (int i = 0; i < SIZE; i++)
	{
		printf("Register #%-3d --> %10d \t\tMemory #%x --> %10d\n", i, reg[i], &var[i], var[i]);
	}
}

void print_regxvarx(int* reg, int* var)
{
	for (int i = 0; i < SIZE; i++)
	{
		printf("Register #%x --> %10d \t\tVariable #%x --> %10d\n", &reg[i], reg[i], &var[i], var[i]);
	}
}

int load(int LOC, int*** Ra)
{
	*Ra = LOC;

	if (LOC != 0 && *Ra == LOC)
	{
		return LOAD_SUCCESS;
	}
}

int load_i(int*** LOC, int index, int*** Ra)
{
	*Ra = *(LOC + index);

	if (*(LOC + index) != 0 && *Ra == *(LOC + index))
	{
		return LOAD_SUCCESS;
	}
}

int store(int Ra, int*** LOC)
{
	*LOC = Ra;
}

int store_i(int*** Ra, int index, int* LOC)
{
	*LOC = *(Ra + index);
}


/*
Implementation:
Rc receives an integer whether from a number or a variable

Check Rb is a register or not first before call
*/
int add(int*** Ra, int Rb, int Rc)
{
	*Ra = (Rb) + (Rc);
}

int minus(int*** Ra, int Rb, int Rc)
{
	*Ra = (Rb) - (Rc);
}

int mult(int*** Ra, int Rb, int Rc)
{
	*Ra = (Rb) * (Rc);
}

int divi(int*** Ra, int Rb, int Rc)
{
	if (Rc > 0)
	{
		*Ra = (Rb) / (Rc);
	}
	else
	{
		*Ra = 0;
		printf("%s\n", DIVISION_BY_ZERO);
		getch();
	}
	return B_DIVISON_BY_ZERO;
}

int comp(int oper_1, int oper_2, int code)
{
	int ret = 0;
	switch (code)
	{
		case B_MORE:
			(oper_1 > oper_2) ? ret = 1 : NULL;
			break;
		case B_LESS:
			(oper_1 < oper_2) ? ret = 1 : NULL;
			break;
		case B_EQUALS:
			(oper_1 == oper_2) ? ret = 1 : NULL;
			break;
		case B_NOT:
			(oper_1 != oper_2) ? ret = 1 : NULL;
			break;
		case B_MORE_EQUAL:
			(oper_1 >= oper_2) ? ret = 1 : NULL;
			break;
		case B_LESS_EQUAL:
			(oper_1 <= oper_2) ? ret = 1 : NULL;
			break;
	}
	return ret;
}

int reset_mem(int*** mem)
{
	*mem = 0;
}

int set_var(int*** LOC, int num)
{
	*LOC = num;

	if (num != 0 && *LOC == num)
	{
		return SET_SUCCESS;
	}
}

/*
	Compiler mode commands
*/

FILE* openfile(const char* fulldirectory)
{
	FILE* fp = fopen(fulldirectory, "r");
	return fp;
}

int set_if(int line_num)
{
	return line_num;
}

int set_else(int line_num)
{
	return line_num;
}

int set_endif(int line_num)
{
	return line_num;
}

char* set_jump(const char* label, int* line_num)
{
	return label;
}

void jump(const char* label)
{

}

void display(const char* input)
{
	char* output = malloc(strlen(input) * sizeof(char));
	strncpy(output, &input[1], strlen(input));
	output[strlen(output) - 1] = '\0';

	printf("%s", output);
}

void print(int var)
{
	printf("%d", var);
}

void f_print(char* s_param, int val[], int val_count)
{
	int string_size = strlen(s_param);
	char* local_string = (char*) malloc(strlen(s_param) * sizeof(char));
	strcpy(local_string, s_param);
	char null = 1;
	char escape = 2;
	local_string[0] = null;
	local_string[(strlen(local_string)) - 1] = null;

	int local_string_size = strlen(local_string);

	int param_count = 0;
	int* insert_loc = (int*) malloc(local_string_size * sizeof(int));

	for (int i = 0; i < local_string_size; i++)
	{
		if (local_string[i] == '%')
		{
			local_string[i] = escape;
			param_count++;
		}
	}
	
	char** params = malloc((param_count + 1) * sizeof(char*));
	int total_digits = 0;
	for (int i = 0; i < param_count; i++)
	{
		int digits = log10((double) (val[i] == 0) ? 1 : (val[i] < 0 ? val[i] * - 1: val[i])) + 1;
		total_digits += digits;
		params[i] = malloc(digits * sizeof(char));

		sprintf(params[i], "%d", val[i]);
	}

	int final_size = local_string_size + total_digits;
	char* last_out = calloc(final_size, sizeof(char));
	int pos = 1;
	for (int i = 1, j = 0; i < final_size; i++)
	{
		if (local_string[i] == escape && i != final_size - 1)
		{
			int next_escape = 0;
			for (int k = i + 1; k < final_size; k++)
			{
				if (local_string[k] == escape || local_string[k] == null)
				{
					next_escape = k;
					break;
				}
			}

			if (j == 0)
			{
				strncpy((last_out + pos - 1), (local_string + pos), i - 1);
			}
			//last_out[strlen(last_out) - 2] = '\0';


			strcat((last_out) + 1, params[j]);
			
			int lo_len = i + 2;
			int next_start = strlen(last_out) + strlen(params[j]);
			int diff = strlen(&local_string[lo_len]) - strlen(&local_string[next_escape]);
			
			strncat((last_out + 1), &local_string[lo_len], diff);
			pos += strlen(last_out);
			//i++;
			j++;
		}
	}

	printf("%s", last_out);

#ifdef DEBUG
	printf("Final string: %s\n", last_out);
	printf("String: %s\n", local_string);
	printf("Char 0: %c\n", null);
	for (int i = 0; i < param_count; i++)
	{
		printf("Params: %d\n", val[i]);
		printf("Param string: %s\n", params[i]);
	}
#endif
}

void newline(void)
{
	printf("\n");
}

void space(void)
{
	printf(" ");
}

void readinput(int*** LOC)
{
	fflush(stdin);
	int input;
	char in[100];
	char temp[100];
	fgets(in, 100, stdin);
	int neg = 1;
	for (int i = 0; i < strlen(in); i++)
	{
		if (i == 0)
		{
			if (in[i] == '-')
			{
				temp[i] = in[i];
				neg = -1;
				continue;
			}
		}
		if (isdigit(in[i]))
		{
			temp[i] = in[i];
		}
		else
		{
			break;
		}
	}


	int out = atoi(temp);
	*LOC = out;

	fflush(stdin);
}
