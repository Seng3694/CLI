#ifndef SENG_CLI_H
#define SENG_CLI_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "argument.h"

SENGAPIBEGIN

/**
* \brief CLI errors enum.
* The CLI errors enum consists of flags. These will be concatenated with the binary or operator.
* If you want to know if a specific flag is set use the macro "HAS_FLAG(value, flag_to_check)".
* \sa    HAS_FLAG()
*/
typedef enum cli_errors
{
	ERROR_NONE				= 0,
	ERROR_ARG_TOO_SHORT		= 1 << 0,
	ERROR_NO_ARGS			= 1 << 1,
	ERROR_ARGS_MISSING		= 1 << 2
} cli_errors;

/**
* \brief Command Line Interface parser struct.
* \sa    CLI_Create()
* \sa    CLI_Destroy()
* \sa    CLI_AddArgument()
* \sa    CLI_Parse()
* \sa    CLI_TryGetArgument()
* \sa    CLI_OptionSet()
*/
typedef struct CLI
{
	uint32 argumentCount;
	Argument** arguments;
} CLI;

/**
* \brief          Creates a new CLI parser and reserves enough space for the arguments.
* \param argCount The count of arguments to calculate how much space should be reserved.
* \return         The created CLI struct.
* \sa             CLI_Destroy()
*/
CLI* CLI_Create(const uint32 argCount);
/**
* \brief      Destroys the CLI.
* \param self The CLI to destroy.
* \sa         CLI_Create()
*/
void CLI_Destroy(CLI* self);

/**
* \brief      Adds an argument to the CLI.
* \param self The CLI to add the argument to.
* \param name The key of the argument. e.g. 'h' for "help".
* \param type The argument type. Different types will result in different error results.
*/
void CLI_AddArgument(CLI* self, const char name, const argument_type type);

/**
* \brief      Parses the passed command line arguments.
* \param self The CLI.
* \param argc The count of a arguments provided.
* \param argv The arguments.
* \return     Returns a error value. 0 == (ERROR_NONE) => no error
*/
cli_errors CLI_Parse(CLI* self, int argc, char** argv);

/**
* \brief        Tries to get the value from the argument with the given name.
* \param self   The CLI.
* \param name   The name of the argument (e.g. 'h')
* \param outArg You can pass a char* which will point to the value of the argument. You should copy the value if you want to persist it.
* \return       Returns CLI_TRUE(1) if the value could have been set and CLI_FALSE(0) if not.
*/
boolean CLI_TryGetArgument(CLI* self, const char name, char** outArg);

/**
* \brief        Checks if an option is set (works for commands and required arguments too).
* \param self   The CLI.
* \param name   The name of the argument (e.g. 'h')
* \return       Returns CLI_TRUE(1) if the value is set and CLI_FALSE(0) if not.
*/
boolean CLI_OptionSet(CLI* self, const char name);

SENGAPIEND

#endif
