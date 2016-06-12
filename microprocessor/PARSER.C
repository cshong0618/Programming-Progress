#include "PARSER.H"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* prog_lines[PROGLINES_MAX];
struct PROGRAM prog = { 0 };
struct IFPAIR ifp[BRANCH_MAX] = { 0 };
struct JMPPAIR jmp[BRANCH_MAX] = { 0 };
struct WHILEPAIR whp[BRANCH_MAX] = { 0 };
struct VARIABLE varname[BRANCH_MAX] = { 0 };
struct SMARTRACK st;

void set_programcounter(int line)
{
	prog.program_counter = line;
}

void set_total_lines(int lines)
{
	prog.total_lines = lines;
}

int get_total_lines(void)
{
	return prog.total_lines;
}

char* get_inst(int i)
{
	return prog_lines[i];
}

void initprogram(void)
{
	prog.program_counter = 0;
	prog.current_jmp_size = 0;
	prog.if_counter = 0;
	prog.while_counter = 0;
	prog.total_lines = 0;
	for (int i = 0; i < 1000; i++)
	{
		prog_lines[i] = NULL;
	}
}

void initstructs(void)
{
	for (int i = 0; i < SIZE * 2; i++)
	{
		jmp[i].label_line = -1;
		jmp[i].label_name = (char*) malloc(30, sizeof(char));
		strcpy(jmp[i].label_name, "");

		ifp[i].else_line = -1;
		ifp[i].endif_line = -1;
		ifp[i].if_line = -1;
		ifp[i].nested_if = NULL;

		whp[i].dowhile_line = -1;
		whp[i].enddo_line = -1;
	}

	for (int i = 0; i < SIZE; i++)
	{
		varname[i].location = -1;
		varname[i].name = (char*) malloc(30 * sizeof(char));
		strcpy(varname[i].name, "");

		st.smartreg[i] = -1;
		st.smartvar[i] = -1;
	}

	st.reg_count = 0;
	st.var_count = 0;
}

void copyprog(char* program[], int size)
{
	for (int i = 0; i < size; i++)
	{
		prog_lines[i] = (char*) malloc(strlen(program[i]) * sizeof(char));
		strcpy(prog_lines[i], program[i]);
	}
	prog.total_lines = size;
}

void interpret_inst(char* line)
{
	prog_lines[0] = line;
}

int checkparams(char* param, int pattern)
{
	switch (pattern)
	{
		case R_R:
			if (toupper(param[0]) == R_R) return B_R;
			break;
		case R_V:
			if (toupper(param[0]) == R_V) return B_V;
			break;
		case R_N:
			if (toupper(param[0]) == R_N) return B_N;
			break; 
		case R_MORE:
			if (toupper(param[0]) == R_MORE) return B_MORE;
			break;
		case R_LESS:
			if (toupper(param[0]) == R_LESS) return B_LESS;
			break;
		case R_EQUALS:
			if (toupper(param[0]) == R_EQUALS) return B_EQUALS;
			break;
		case R_NOT:
			if (toupper(param[0]) == R_NOT) return B_NOT;
			break;
		default:
			return 0;
	}
	return 0;
}

int check_param_char(char param, int pattern)
{
	switch (pattern)
	{
		case R_R:
			if (toupper(param) == R_R) return B_R;
			break;
		case R_V:
			if (toupper(param) == R_V) return B_V;
			break;
		case R_N:
			if (toupper(param) == R_N) return B_N;
			break;
		case R_MORE:
			if (toupper(param) == R_MORE) return B_MORE;
			break;
		case R_LESS:
			if (toupper(param) == R_LESS) return B_LESS;
			break;
		case R_EQUALS:
			if (toupper(param) == R_EQUALS) return B_EQUALS;
			break;
		case R_NOT:
			if (toupper(param) == R_NOT) return B_NOT;
			break;
		default:
			return 0;
	}
	return 0;
}

int extractparams(char* param)
{
	char temp[100] = { 0 };
	
	int neg = 1;
	int start = 0;

	for (int i = 1, j = 0; i < strlen(param); i++)
	{
		if (i == 1)
		{
			if (param[i] == '-')
			{
				neg = -1;
				continue;
			}
		}

		if (isdigit(param[i]))
		{
			temp[j] = param[i];
			j++;
		}
		else
		{
			return -1;
		}
	}

	return atoi(temp) * neg;
}

int condition(int*** _registers, char* _params[])
{
	int type = checkparams(_params[1], _params[1][0]);

	if (strlen(_params[1]) > 1)
	{
		type += check_param_char(_params[1][1], _params[1][1]);
	}

	int entry_type = checkparams(_params[2], R_R);
	int entry_type2 = checkparams(_params[3], R_R);
	entry_type2 |= checkparams(_params[3], R_N);

	int ret = 0;

	if (entry_type == B_R && entry_type2 == B_R)
	{
		ret = comp(_registers[extractparams(_params[2])],
				   _registers[extractparams(_params[3])],
				   type);
	}
	if (entry_type == B_R && entry_type2 == B_N)
	{
		ret = comp(_registers[extractparams(_params[2])],
				   extractparams(_params[3]),
				   type);
	}
}

int interpreter(int** _registers, int** _variables, char* _params[], int size, void** file)
{
	switch (switch_inst(_params[0]))
	{
		case B_LOAD:
		{
					   int state = -1;
					   if (size == 3)
					   {

						   int pass;
						   pass = checkparams(_params[1], R_V);
						   pass |= checkparams(_params[2], R_R);
						   if (pass == (B_V | B_R))
						   {
							   state = load(_variables[extractparams(_params[1])],
											&_registers[extractparams(_params[2])]);
						   }
					   }
					   else if (size == 4)
					   {
						   int pass;
						   pass = checkparams(_params[1], R_V);
						   pass |= checkparams(_params[2], R_N);
						   pass |= checkparams(_params[3], R_R);
						   if (pass == (B_V | B_R | B_N))
						   {
							   state = load_i(&_variables[extractparams(_params[1])],
									  extractparams(_params[2]),
									  &_registers[extractparams(_params[3])]);
						   }
					   }

					   if (state == B_OUT_OF_BOUNDS)
					   {
						   printf("%s\n", OUT_OF_BOUNDS);
					   }
					   break;
		}
		case B_STORE:
		{
						if (size == 3)
						{
							int pass;
							pass = checkparams(_params[1], R_R);
							pass |= checkparams(_params[2], R_V);

							if (pass == (B_R | B_V))
							{
								store(_registers[extractparams(_params[1])],
									  &_variables[extractparams(_params[2])]);
							}
						}
						break;
		}

		case B_SET:
		{
					  int state = 0;
					  if (size == 3)
					  {
						  int pass;

						  pass = checkparams(_params[1], R_V);
						  pass |= checkparams(_params[2], R_N);

						  if (pass == (B_V | B_N))
						  {
							  state = set_var(&_variables[extractparams(_params[1])],
											  extractparams(_params[2]));
						  }
					  }

					  if (state == SET_SUCCESS)
					  {
						  varname[extractparams(_params[1])].location = extractparams(_params[1]);
						  varname[extractparams(_params[1])].name = (char*) malloc(strlen(_params[1]) * sizeof(char));
						  strcpy(varname[extractparams(_params[1])].name, _params[1]);
						  varname[extractparams(_params[1])].val = extractparams(_params[2]);

						  int pos = extractparams(_params[1]);
						  int found = 0;
						  for (int i = 0; i < st.var_count; i++)
						  {
							  if (st.smartvar[i] == pos)
							  {	
								  found = 1;
								  break;
							  }
						  }
						  if (found == 0 || st.var_count == 0)
						  {
							  st.smartvar[st.var_count] = pos;
							  st.var_count++;
						  }
					  }
					  //printf("%s\n", varname[extractparams(_params[1])].name);
					  break;
		}
		case B_RESET:
		{
						int pass;
						pass = checkparams(_params[1], R_R);
						pass |= checkparams(_params[1], R_V);

						if (pass == B_R)
						{
							reset_mem(&_registers[extractparams(_params[1])]);
						}
						else if (pass == B_V)
						{
							reset_mem(&_variables[extractparams(_params[1])]);
						}
						break;
		}

		case B_MULT:
		{
					   if (size == 4)
					   {
						   int pass;
						   pass = checkparams(_params[1], R_R);
						   pass += checkparams(_params[2], R_R);
						   pass += checkparams(_params[3], R_R);
						   pass += checkparams(_params[3], R_N);

						   if (pass == (B_R + B_R + B_R))
						   {
							   mult(&_registers[extractparams(_params[1])],
									_registers[extractparams(_params[2])],
									_registers[extractparams(_params[3])]);
						   }
						   if (pass == (B_R + B_R + (B_N)))
						   {

							   mult(&_registers[extractparams(_params[1])],
									_registers[extractparams(_params[2])],
									extractparams(_params[3]));
						   }
					   }

					   break;
		}

		case B_ADD:
		{
					  if (size == 4)
					  {
						  int pass;
						  pass = checkparams(_params[1], R_R);
						  pass += checkparams(_params[2], R_R);
						  pass += checkparams(_params[3], R_R);
						  pass += checkparams(_params[3], R_N);

						  if (pass == (B_R + B_R + B_R))
						  {
							  add(&_registers[extractparams(_params[1])],
								  _registers[extractparams(_params[2])],
								  _registers[extractparams(_params[3])]);
						  }
						  if (pass == (B_R + B_R + (B_N)))
						  {

							  add(&_registers[extractparams(_params[1])],
								  _registers[extractparams(_params[2])],
								  extractparams(_params[3]));
						  }
					  }
						  break;
		}

		case B_MINUS:
		{
						if (size == 4)
						{
							int pass;
							pass = checkparams(_params[1], R_R);
							pass += checkparams(_params[2], R_R);
							pass += checkparams(_params[3], R_R);
							pass += checkparams(_params[3], R_N);

							if (pass == (B_R + B_R + B_R))
							{
								minus(&_registers[extractparams(_params[1])],
									  _registers[extractparams(_params[2])],
									  _registers[extractparams(_params[3])]);
							}
							if (pass == (B_R + B_R + (B_N)))
							{

								minus(&_registers[extractparams(_params[1])],
									  _registers[extractparams(_params[2])],
									  extractparams(_params[3]));
							}
						}
						break;
		}

		case B_DIVIDE:
		{
						 if (size == 4)
						 {
							 int pass;
							 int err;
							 pass = checkparams(_params[1], R_R);
							 pass += checkparams(_params[2], R_R);
							 pass += checkparams(_params[3], R_R);
							 pass += checkparams(_params[3], R_N);

							 if (pass == (B_R + B_R + B_R))
							 {
								 divi(&_registers[extractparams(_params[1])],
									  _registers[extractparams(_params[2])],
									  _registers[extractparams(_params[3])]);
							 }
							 if (pass == (B_R + B_R + (B_N)))
							 {

								 divi(&_registers[extractparams(_params[1])],
									  _registers[extractparams(_params[2])],
									  extractparams(_params[3]));
							 }
						 }
						break;
		}

		case B_OPEN:
		{
					   asm(_registers, _variables, _params[1]);
					   getch();
					   break;
		}

		case B_SETVAR:
		{
						 int pos = -1;
						 for (int i = 0; i < SIZE; i++)
						 {
							 if (strcmp(_params[1], varname[i].name) == 0)
							 {
								 pos = i;
								 break;
							 }
						 }


						 int pass = checkparams(_params[2], R_N);

						 if (pass == B_N)
						 {
							 if (pos == -1)
							 {
								 int empty;
								 for (int i = 0; i < SIZE; i++)
								 {
									 if (varname[i].location == -1)
									 {
										 empty = i;
										 break;
									 }
								 }

								 set_var(&_variables[empty], extractparams(_params[2]));
								 varname[empty].location = empty;
								 varname[empty].name = (char*) malloc(strlen(_params[1]) * sizeof(char));
								 strcpy(varname[empty].name, _params[1]);
								 varname[empty].val = extractparams(_params[2]);
							 }
							 else
							 {
								 set_var(&_variables[pos], extractparams(_params[2]));
								 varname[pos].val = extractparams(_params[2]);
							 }
						 }

						 break;
		}
		case B_SETJMP:
		{
						 int newjmp = -1;
						 for (int i = 0; i < prog.current_jmp_size; i++)
						 {
							 if (strcmp(_params[i], jmp[i].label_name) == 0)
							 {
								 break;
							 }
							 else
							 {
								 newjmp = 1;
							 }
						 }

						 if (newjmp == 1 || prog.current_jmp_size == 0)
						 {
							 jmp[prog.current_jmp_size].label_line = prog.program_counter + 1;
							 jmp[prog.current_jmp_size].label_name = (char*) malloc(strlen(_params[1]) * sizeof(char));
							 strcpy(jmp[prog.current_jmp_size].label_name, _params[1]);
							 prog.current_jmp_size++;
						 }
						 break;
		}

		case B_JMP:
		{
					  int line = -1;
					  for (int i = 0; i < prog.current_jmp_size; i++)
					  {
						  if (strcmp(jmp[i].label_name, _params[1]) == 0)
						  {
							  line = i;
						  }
					  }
					  
					  if (line > -1)
					  {
						  prog.program_counter = jmp[line].label_line - 1;
					  }

					  break;
		}

		case B_IF:
		{
					 int ret = condition(_registers, _params);

					 int current_line = prog.program_counter;
					 int newif = 1;
					 int ifpos = 0;

					 for (int i = 0; i < prog.if_counter; i++)
					 {
						 if (ifp[i].if_line == current_line)
						 {
							 newif = 0;
							 ifpos = i + i;
							 break;
						 }
						 ifpos = prog.if_counter;
					 }

					 if (newif == 1 || prog.if_counter == 0)
					 {
						 ifp[prog.if_counter].if_line = current_line;
						 int pos = 0;
						 int ifloc = 0;

						 int nestedifs = 1;
						 int nestedendifs = 1;
						 int nestedelse = 0;
						 for (int i = current_line + 1; i < prog.total_lines; i++)
						 {
							 char* t_params[10];
							 int j = 0;
							 while (pos < strlen(prog_lines[i]))
							 {
								 t_params[j] = a_lexer(prog_lines[i], &pos, ' ', ',');
								 j++;
							 }

							 pos = 0;
							 if (_strcmpi(t_params[0], IF) == 0)
							 {
								 nestedifs++;
								 nestedendifs = nestedifs;
							 }
							 if (_strcmpi(t_params[0], ENDIF) == 0)
							 {
								 nestedendifs--;
								 nestedelse--;
								 nestedifs--;
								 if (nestedendifs == 0)
								 {
									 ifp[prog.if_counter].endif_line = i;
									 break;
								 }
							 }
							 if (_strcmpi(t_params[0], ELSE) == 0)
							 {
								 nestedelse++;
								 if (nestedifs - nestedelse == 0)
								 {
									 ifp[prog.if_counter].else_line = i;
									 ifp[prog.if_counter].jump_line = ifp[prog.if_counter].else_line;  
									 nestedelse = -99;
								 }	
							 }
						 }
						 prog.if_counter++;
					 }

					 if (ret)
					 {
						 prog.program_counter = ifp[ifpos].if_line;
						 ifp[ifpos].skip_else = 1;
					 }
					 else if (ifp[ifpos].jump_line != NULL)
					 {
						 prog.program_counter = ifp[ifpos].jump_line;
					 }
					 else
					 {
						 prog.program_counter = ifp[ifpos].endif_line;
					 }

#ifdef DEBUG
					 ret == 1 ? puts("True\n") : puts("False\n");
#endif
					 break;
		}

		case B_ELSE:
		{
					   int elsepos = 0;
					   for (int i = 0; i < prog.if_counter; i++)
					   {
						   if (prog.program_counter == ifp[i].else_line)
						   {
							   elsepos = i;
							   break;
						   }
					   }

					   if (ifp[elsepos].skip_else == 1)
					   {
						   prog.program_counter = ifp[elsepos].endif_line;
						   ifp[elsepos].skip_else = 0;
					   }

					   break;
		}

		case B_ENDIF:
		{
						break;
		}

		case B_DISPLAY:
		{
						  int tab = 0;
						  for (int i = 0; i < (strlen(prog_lines[prog.program_counter])); i++)
						  {
							  if ((prog_lines[prog.program_counter])[i] == '\t')
							  {
								  tab++;
							  }
							  else
							  {
								  break;
							  }
						  }

						  int string_start = strlen(_params[0]);
						  int string_length = strlen(prog_lines[prog.program_counter]) - string_start + 2;

						  char* out = (char*) calloc(string_length ,sizeof(char));
						  for (int i = (string_start + 1 + tab), j = 0; 
							   (i < (strlen(prog_lines[prog.program_counter])) + 1); 
							   i++, j++)
						  {
							  out[j] = (prog_lines[prog.program_counter])[i];
						  }
						  out[strlen(out) + 1] = '\0';
						  display(out);
						  free(out);
						  break;
		}

		case B_PRINT:
		{
						int pass;
						pass = checkparams(_params[1], R_R);
						pass |= checkparams(_params[1], R_V);

						if (pass == B_R)
						{
							print(_registers[extractparams(_params[1])]);
						}
						if (pass == B_V)
						{
							print(_variables[extractparams(_params[1])]);
						}

						break;
		}

		case B_FPRINT:
		{
						 int val_count = size - 3;
						 int* values = (int*) malloc(val_count * sizeof(int));
						 for (int i = 3, j = 0; i < size; i++, j++)
						 {
							 values[j] = _registers[extractparams(_params[i])];
						 }

						 f_print(_params[1], values, val_count);
						 free(values);
						 break;
		}

		case B_READIN:
		{
						 int pass;
						 pass = checkparams(_params[1], R_R);

						 if (pass == B_R)
						 {
							 readinput(&_registers[extractparams(_params[1])]);
						 }
						 break;
		}

		case B_CLEARALL:
		{
						   for (int i = 0; i < SIZE; i++)
						   {
							   reset_mem(&_registers[i]);
							   reset_mem(&_variables[i]);
						   }
						   initprogram();
						   initstructs();
						   break;

		}
		case B_NEWLINE:
		{
						  newline();
						  break;
		}

		case B_DOWHILE:
		{
						  int ret = condition(_registers, _params);

						  int current_line = prog.program_counter;

						  int newwhile = 1;
						  int whilepos = 0;
						  for (int i = 0; i < prog.while_counter; i++)
						  {
							  if (whp[i].dowhile_line == current_line)
							  {
								  newwhile = 0;
								  whilepos = i;
								  break;
							  }
							  whilepos = prog.while_counter;
						  }

						  if (newwhile == 1 || prog.while_counter == 0)
						  {
							  whp[prog.while_counter].dowhile_line = current_line;
							  int pos = 0;

							  int nestedwhile = 1;
							  int nestedenddo = 1;
							  for (int i = current_line + 1; i < prog.total_lines; i++)
							  {
								  char* t_params[10];
								  int j = 0;
								  while (pos < strlen(prog_lines[i]))
								  {
									  t_params[j] = a_lexer(prog_lines[i], &pos, ' ', ',');
									  j++;
								  }

								  pos = 0;
								  if (_strcmpi(t_params[0], DOWHILE) == 0)
								  {
									  nestedwhile++;
									  nestedenddo = nestedwhile;
								  }

								  if (_strcmpi(t_params[0], ENDDO) == 0)
								  {
									  nestedwhile--;
									  nestedenddo--;
									  if (nestedenddo == 0)
									  {
										  whp[whilepos].enddo_line = i;
										  break;
									  }
								  }
							  }
							  prog.while_counter++;
						  }

						  whp[whilepos].loop = ret;
						  if (ret)
						  {
							  prog.program_counter = whp[whilepos].dowhile_line;
						  }
						  else
						  {
							  prog.program_counter = whp[whilepos].enddo_line;
						  }

#ifdef DEBUG
						  ret == 1 ? puts("True\n") : puts("False\n");
#endif
		
						  break;
		}

		case B_ENDDO:
		{
						int current_line = prog.program_counter;
						int looploc = 0;
						int jmpline = current_line;

						for (int i = 0; i < prog.while_counter; i++)
						{
							if (whp[i].enddo_line == current_line)
							{
								jmpline = whp[i].dowhile_line;
								looploc = i;
								break;
							}
						}

						if (whp[looploc].loop == 1)
						{
							prog.program_counter = jmpline - 1;
						}

						break;
		}
		case B_SPACE:
		{
						printf(" ");
						break;
		}
	}
	return ++(prog.program_counter);
}

int switch_inst(const char* inst)
{
	if (_strcmpi(inst, LOAD) == 0) return B_LOAD;
	if (_strcmpi(inst, LOAD_I) == 0) return B_LOAD_I;
	if (_strcmpi(inst, STORE) == 0) return B_STORE;
	if (_strcmpi(inst, STORE_I) == 0) return B_STORE_I;
	if (_strcmpi(inst, ADD) == 0) return B_ADD;
	if (_strcmpi(inst, MINUS) == 0) return B_MINUS;
	if (_strcmpi(inst, MULT) == 0) return B_MULT;
	if (_strcmpi(inst, DIVIDE) == 0) return B_DIVIDE;
	if (_strcmpi(inst, RESET) == 0) return B_RESET;
	if (_strcmpi(inst, SET) == 0) return B_SET;
	if (_strcmpi(inst, OPEN) == 0) return B_OPEN;
	if (_strcmpi(inst, IF) == 0) return B_IF;
	if (_strcmpi(inst, ELSE) == 0) return B_ELSE;
	if (_strcmpi(inst, ENDIF) == 0) return B_ENDIF;
	if (_strcmpi(inst, SETJMP) == 0) return B_SETJMP;
	if (_strcmpi(inst, JMP) == 0) return B_JMP;
	if (_strcmpi(inst, DISPLAY) == 0) return B_DISPLAY;
	if (_strcmpi(inst, PRINT) == 0) return B_PRINT;
	if (_strcmpi(inst, FPRINT) == 0) return B_FPRINT;
	if (_strcmpi(inst, READIN) == 0) return B_READIN;
	if (_strcmpi(inst, NEWLINE) == 0) return B_NEWLINE;
	if (_strcmpi(inst, SETVAR) == 0) return B_SETVAR;
	if (_strcmpi(inst, CLEARALL) == 0) return B_CLEARALL;
	if (_strcmpi(inst, DOWHILE) == 0) return B_DOWHILE;
	if (_strcmpi(inst, ENDDO) == 0) return B_ENDDO;
	if (_strcmpi(inst, SPACE) == 0) return B_SPACE;
}