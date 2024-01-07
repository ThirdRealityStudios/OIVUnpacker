#include <stdio.h>
#include <stdlib.h>

void reportIfNULL(void *ptrValue, char *ptrIdentifier, char *fncIdentifier, char *causedBy)
{
	if(ptrValue == NULL)
	{
		if(causedBy != NULL)
			fprintf(stderr, "Variable \"%s\" is NULL in function %s, (probably) caused by %s (Hence, exiting now)",
					ptrIdentifier, fncIdentifier, causedBy);
		else
			fprintf(stderr, "Variable \"%s\" is NULL in function %s (Hence, exiting now)",
					ptrIdentifier, fncIdentifier);

		exit(EXIT_FAILURE);
	}
}