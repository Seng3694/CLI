#ifndef SENG_ARGUMENT_H
#define SENG_ARGUMENT_H

#include <stdlib.h>
#include "common.h"

SENGAPIBEGIN

/**
* \brief Argument Type enum.
*/
typedef enum argument_type
{
	ARG_TYPE_OPTION,
	ARG_TYPE_REQUIRED,
	ARG_TYPE_COMMAND
} argument_type;

/**
* \brief Argument struct.
* \sa    Argument_Create()
* \sa    Argument_Destroy()
*/
typedef struct Argument
{
	argument_type type;
	boolean set;
	char name;
	char* value;
} Argument;

/**
* \brief  Creates a new argument.
* \return The created argument.
* \sa     Argument_Destroy()
*/
Argument* Argument_Create(void);

/**
* \brief      Destroys the argument.
* \param self The Argument to destroy.
* \sa         Argument_Create()
*/
void Argument_Destroy(Argument* self);

SENGAPIEND

#endif
