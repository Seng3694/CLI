#include "argument.h"

Argument* Argument_Create(void)
{
	Argument* arg = malloc(sizeof(Argument));
	arg->value = NULL;
	arg->type = ARG_TYPE_OPTION;
	arg->set = CLI_FALSE;
	arg->name = 0;
	return arg;
}

void Argument_Destroy(Argument* self)
{
	if (self->value)
		free(self->value);

	self->value = NULL;
	free(self);
	self = NULL;
}
