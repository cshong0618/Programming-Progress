#include "LEXER.H"


char* a_lexer(char* inst, int* pos, char delim, char delim_2)
{
	char* command;

	int space;
	int string_start = -1;
	int string_end = -1;
	loop:
	for (int i = *pos; i < strlen(inst) + 1; i++)
	{
		if (inst[i] == delim || inst[i] == delim_2 || inst[i] == '\n' || inst[i] == '\0' || inst[i] == '\t')
		{
			space = i;
			break;
		}
		if (inst[i] == '"')
		{
			string_start = i;
			for (int j = i + 1; j < strlen(inst) + 1; j++)
			{
				if (inst[j] == '"')
				{
					string_end = j;
					break;
				}
			}

			if (string_end != -1)
			{
				break;
			}
		}

		space = -1;
	}

	if (string_start > -1 && string_end > -1)
	{
		command = (char*) malloc((string_end - string_start) * sizeof(char));
		int j = 0;
		for (int i = string_start; i < string_end + 1; i++)
		{
			command[j] = inst[i];
			j++;
		}

		command[j] = '\0';
		*pos = string_end;

		if (command[0] == '\0') goto loop;
		return command;
	}

	if (space > -1)
	{
		command = (char*) malloc(space * sizeof(char));
		int j = 0;
		for (int i = *pos; i < space; i++)
		{
			if (inst[i] == ' ' && i < space) continue;
			command[j] = inst[i];
			j++;
		}

		command[j] = '\0';
		*pos = space + 1;

		// TODO: Fix this logic!
		if (command[0] == '\0') goto loop;
		return command;
	}
	else
	{
		return "error";
	}
}


char* c_lexer(char* inst, int* pos, char delim, char delim_2)
{
	char* command;

	int space;
loop:
	for (int i = *pos; i < strlen(inst) + 1; i++)
	{
		if (inst[i] == delim || inst[i] == delim_2 || inst[i] == '\n' || inst[i] == '\0' || inst[i] == '\t')
		{
			space = i;
			break;
		}
		space = -1;
	}

	if (space > -1)
	{
		command = (char*) malloc(space * sizeof(char));
		int j = 0;
		for (int i = *pos; i < space; i++)
		{
			if (inst[i] == ' ' && i < space) continue;
			command[j] = inst[i];
			j++;
		}

		command[j] = '\0';
		*pos = space + 1;

		// TODO: Fix this logic!
		if (command[0] == '\0') goto loop;
		return command;
	}
	else
	{
		return "error";
	}
}