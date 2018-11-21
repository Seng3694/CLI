#include <cli.h>
#include <iostream>

int main(int argc, char** argv)
{
	CLI* cli = CLI_Create(6);

	CLI_AddArgument(cli, 'h', ARG_TYPE_COMMAND);
	CLI_AddArgument(cli, 'v', ARG_TYPE_COMMAND);
	CLI_AddArgument(cli, 'a', ARG_TYPE_REQUIRED);
	CLI_AddArgument(cli, 'b', ARG_TYPE_REQUIRED);
	CLI_AddArgument(cli, 'c', ARG_TYPE_REQUIRED);
	CLI_AddArgument(cli, 'r', ARG_TYPE_OPTION);

	cli_errors error = CLI_Parse(cli, argc, argv);
	if (error != 0)
	{
		printf("error while parsing cli arguments:\n");
		if (HAS_FLAG(error, ERROR_ARGS_MISSING))
			printf(" One or more required arguments are missing.\n");
		if (HAS_FLAG(error, ERROR_ARG_TOO_SHORT))
			printf(" One or more arguments were too short.\n");
		if (HAS_FLAG(error, ERROR_NO_ARGS))
			printf(" No arguments were provided.\n");
		printf("try -h for help.\n");
		CLI_Destroy(cli);
		return error;
	}

	if (CLI_OptionSet(cli, 'h'))
	{
		printf("\nThis app should serve as a showcase for the light CLI framework.\n"
			"When you provide -a -b and -c it will print these parameters.\n"
			"Optionally you can reverse them with the -r flag.\n"
			"Example: >c_example.exe -a\"Hello\" -b\" \" -c\"World\" -r\n"
			"Example Output: \"World Hello\"\n\n"
			"Available arguments:\n"
			" -h  prints the help text you are looking at (Command)\n"
			" -v  prints the current version number       (Command)\n"
			" -a  first string                            (Required)\n"
			" -b  second string                           (Required)\n"
			" -c  third string                            (Required)\n"
			" -r  reverses order (a,b,c)=>(c,b,a)         (Option)\n\n");
	}

	if (CLI_OptionSet(cli, 'v'))
	{
		printf("CLI example 1.0.0\n");
		CLI_Destroy(cli);
		return 0;
	}

	char* arg = NULL;

	if (CLI_OptionSet(cli, 'r'))
	{
		if (CLI_TryGetArgument(cli, 'c', &arg))
			printf("%s", arg);
		if (CLI_TryGetArgument(cli, 'b', &arg))
			printf("%s", arg);
		if (CLI_TryGetArgument(cli, 'a', &arg))
			printf("%s\n", arg);
	}
	else
	{
		if (CLI_TryGetArgument(cli, 'a', &arg))
			printf("%s", arg);
		if (CLI_TryGetArgument(cli, 'b', &arg))
			printf("%s", arg);
		if (CLI_TryGetArgument(cli, 'c', &arg))
			printf("%s\n", arg);
	}

	CLI_Destroy(cli);
	return 0;
}
