#include "cli.h"

CLI* CLI_Create(const uint32 commandCount)
{
	CLI* cli = malloc(sizeof(CLI));
	cli->argumentCount = 0;
	cli->arguments = malloc(commandCount * sizeof(Argument*));
	return cli;
}

void CLI_Destroy(CLI* self)
{
	uint32 i;
	uint32 c = self->argumentCount;
	for (i = 0; i < c; ++i)
		Argument_Destroy(self->arguments[i]);

	free(self->arguments);
	self->arguments = NULL;
	free(self);
	self = NULL;
}

void CLI_AddArgument(CLI* self, const char name, const argument_type type)
{
	Argument* argument = Argument_Create();
	argument->type = type;
	argument->name = name;
	self->arguments[self->argumentCount++] = argument;
}

cli_errors CLI_Parse(CLI* self, int argc, char** argv)
{
	uint32 uargc = (uint32)argc;
	//first arg is executable
	if (uargc <= 1)
		return ERROR_NO_ARGS;

	cli_errors code = ERROR_NONE;
	uint32 i, j;
	const char* current;
	uint32 currentLength;
	Argument* currentArgument;
	const uint32 argumentCount = self->argumentCount;
	boolean argumentFound = CLI_FALSE;

	//skip executable
	for (i = 1; i < uargc; ++i)
	{
		argumentFound = CLI_FALSE;
		current = argv[i];
		currentLength = strlen(current);

		if (currentLength < 2)
		{
			code |= ERROR_ARG_TOO_SHORT;
			continue;
		}
		else if (current[0] == '-') //if it actually could be a command
		{
			for (j = 0; j < argumentCount; ++j)
			{
				currentArgument = self->arguments[j];

				if (currentArgument->name == current[1])
				{
					if (currentArgument->set)
						continue;

					argumentFound = CLI_TRUE;
					currentArgument->set = CLI_TRUE;

					//if there is any value
					if (currentLength - 2 != 0)
					{
						//example:  "-dParameter" strlen will return 11. we want to skip the hyphen and command name so -2
						//but we still need space for the \0 so we allocate "length" - 1
						currentArgument->value = malloc((currentLength - 1) * sizeof(char));
						//copy from source with offset 2 (skip hyphen and command name) and copy "length" - 1 which includes the \0
						memcpy(currentArgument->value, current + 2, currentLength - 1);
					}
					break;
				}
			}
		}
		//if no argument is found, just set it as positional argument
		if (argumentFound == CLI_FALSE)
		{
			currentArgument = self->arguments[i - 1];
			if (!currentArgument->set)
			{
				currentArgument->set = CLI_TRUE;
				//no offset in this one. so length + 1
				currentArgument->value = malloc((currentLength + 1) * sizeof(char));
				memcpy(currentArgument->value, current, currentLength + 1);
			}
		}
	}

	//validate if either command is set or any required option is not set
	cli_errors code2 = ERROR_NONE;

	for (i = 0; i < argumentCount; ++i)
	{
		currentArgument = self->arguments[i];
		if (currentArgument->type == ARG_TYPE_COMMAND && currentArgument->set)
		{
			code2 = ERROR_NONE;
			break;
		}
		else if(currentArgument->type == ARG_TYPE_REQUIRED && !currentArgument->set)
		{
			code2 |= ERROR_ARGS_MISSING;
		}
	}

	return code | code2;
}

boolean CLI_TryGetArgument(CLI* self, const char name, char** outArg)
{
	const uint32 argumentCount = self->argumentCount;
	Argument* currentArgument;
	uint32 i;

	for (i = 0; i < argumentCount; ++i)
	{
		currentArgument = self->arguments[i];
		if (currentArgument->name == name)
		{
			if (!currentArgument->set)
				return CLI_FALSE;

			*outArg = strdup(currentArgument->value);

			return CLI_TRUE;
		}
	}

	return CLI_FALSE;
}

boolean CLI_OptionSet(CLI* self, const char name)
{
	const uint32 argumentCount = self->argumentCount;
	Argument* currentArgument;
	uint32 i;

	for (i = 0; i < argumentCount; ++i)
	{
		currentArgument = self->arguments[i];
		if (currentArgument->name == name)
		{
			if (!currentArgument->set)
				return CLI_FALSE;
			else
				return CLI_TRUE;
		}
	}

	return CLI_FALSE;
}
