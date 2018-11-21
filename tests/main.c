#include <cli.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "test.h"

TEST(argument, Create);
TEST(cli, Create);
TEST(cli, AddArgument);
TEST(cli, Parse_no_args);
TEST(cli, Parse_too_short_arg);
TEST(cli, Parse_too_less_args);
TEST(cli, Parse_with_optional);
TEST(cli, Parse_without_optional);
TEST(cli, Parse_with_required);
TEST(cli, Parse_with_command);
TEST(cli, Parse_with_command_and_without_required);
TEST(cli, Parse_with_everything);
TEST(cli, TryGetArgument_set);
TEST(cli, TryGetArgument_not_set);
TEST(cli, OptionSet_set);
TEST(cli, OptionSet_not_set);

TESTMAIN()
{
	RUNTEST(argument, Create);
	RUNTEST(cli, Create);
	RUNTEST(cli, AddArgument);
	RUNTEST(cli, Parse_no_args);
	RUNTEST(cli, Parse_too_short_arg);
	RUNTEST(cli, Parse_too_less_args);
	RUNTEST(cli, Parse_with_optional);
	RUNTEST(cli, Parse_without_optional);
	RUNTEST(cli, Parse_with_required);
	RUNTEST(cli, Parse_with_command);
	RUNTEST(cli, Parse_with_command_and_without_required);
	RUNTEST(cli, Parse_with_everything);
	RUNTEST(cli, TryGetArgument_set);
	RUNTEST(cli, TryGetArgument_not_set);
	RUNTEST(cli, OptionSet_set);
	RUNTEST(cli, OptionSet_not_set);

	printf("All %d tests executed successfully\n", test_count);
	getchar();
	return EXIT_SUCCESS;
}

TEST(argument, Create)
{
	Argument* arg = Argument_Create();
	assert(arg->name == '\0');
	assert(arg->set == CLI_FALSE);
	assert(arg->type == ARG_TYPE_OPTION);
	assert(arg->value == NULL);
	Argument_Destroy(arg);
}

TEST(cli, Create)
{
	CLI* cli = CLI_Create(1);
	assert(cli->argumentCount == 0);
	assert(cli->arguments != NULL);
	CLI_Destroy(cli);
}

TEST(cli, AddArgument)
{
	CLI* cli = CLI_Create(1);
	
	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	assert(cli->argumentCount == 1);
	
	const Argument* arg = cli->arguments[0];
	assert(arg->name == 'a');
	assert(arg->set == CLI_FALSE);
	assert(arg->type == ARG_TYPE_OPTION);
	assert(arg->value == NULL);

	CLI_Destroy(cli);
}

TEST(cli, Parse_no_args)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	int argc = 1;
	char* argv[] = {
		"executable.exe"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	//because the first argument is always skipped there should be no arguments
	assert(HAS_FLAG(code, ERROR_NO_ARGS));

	CLI_Destroy(cli);
}

TEST(cli, Parse_too_short_arg)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	//the argument has to be atleast 2 chars long, because a command must contain one hyphen and one char
	assert(HAS_FLAG(code, ERROR_ARG_TOO_SHORT));

	CLI_Destroy(cli);
}

TEST(cli, Parse_too_less_args)
{
	CLI* cli = CLI_Create(2);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);
	CLI_AddArgument(cli, 'b', ARG_TYPE_REQUIRED);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	//b is a required command
	assert(HAS_FLAG(code, ERROR_ARGS_MISSING));

	CLI_Destroy(cli);
}

TEST(cli, Parse_with_optional)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	CLI_Destroy(cli);
}

TEST(cli, Parse_without_optional)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	int argc = 1;
	char* argv[] = {
		"executable.exe"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(HAS_FLAG(code, ERROR_NO_ARGS));

	CLI_Destroy(cli);
}

TEST(cli, Parse_with_required)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_REQUIRED);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	CLI_Destroy(cli);
}

TEST(cli, Parse_with_command)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_COMMAND);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	CLI_Destroy(cli);
}

TEST(cli, Parse_with_command_and_without_required)
{
	CLI* cli = CLI_Create(2);

	CLI_AddArgument(cli, 'a', ARG_TYPE_COMMAND);
	CLI_AddArgument(cli, 'b', ARG_TYPE_REQUIRED);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	//because a command is requested there is no need for any other parameter
	assert(code == ERROR_NONE);

	CLI_Destroy(cli);
}

TEST(cli, Parse_with_everything)
{
	CLI* cli = CLI_Create(3);

	CLI_AddArgument(cli, 'a', ARG_TYPE_COMMAND);
	CLI_AddArgument(cli, 'b', ARG_TYPE_REQUIRED);
	CLI_AddArgument(cli, 'b', ARG_TYPE_OPTION);

	int argc = 4;
	char* argv[] = {
		"executable.exe",
		"-a",
		"-b",
		"-c"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	CLI_Destroy(cli);
}

TEST(cli, TryGetArgument_set)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-aValue"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	char* value = NULL;
	assert(CLI_TryGetArgument(cli, 'a', &value));
	assert(streq(value, "Value"));

	CLI_Destroy(cli);
}

TEST(cli, TryGetArgument_not_set)
{
	CLI* cli = CLI_Create(2);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);
	CLI_AddArgument(cli, 'b', ARG_TYPE_OPTION);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-aValue"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	char* value = NULL;
	assert(!CLI_TryGetArgument(cli, 'b', &value));
	assert(value == NULL);

	CLI_Destroy(cli);
}

TEST(cli, OptionSet_set)
{
	CLI* cli = CLI_Create(1);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	assert(CLI_OptionSet(cli, 'a'));

	CLI_Destroy(cli);
}

TEST(cli, OptionSet_not_set)
{
	CLI* cli = CLI_Create(2);

	CLI_AddArgument(cli, 'a', ARG_TYPE_OPTION);
	CLI_AddArgument(cli, 'b', ARG_TYPE_OPTION);

	int argc = 2;
	char* argv[] = {
		"executable.exe",
		"-a"
	};

	cli_errors code = CLI_Parse(cli, (uint32)argc, argv);

	assert(code == ERROR_NONE);

	assert(!CLI_OptionSet(cli, 'b'));

	CLI_Destroy(cli);
}
