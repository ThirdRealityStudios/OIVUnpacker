#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>

char* readFile(char *filepath, List *exceptSymbols)
{
	if(filepath == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"filepath\" in function readFile()");
		exit(EXIT_FAILURE);
	}
	
	if(exceptSymbols == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"exceptSymbols\" in function readFile() - "
						"if you do not want to use \"exceptSymbols\", then please pass an empty list instead!");
		exit(EXIT_FAILURE);
	}
	
	FILE *file = fopen(filepath, "r");

	if(file == NULL)
	{
		fprintf(stderr, "Fatal Error: Got NULL from variable \"file\" while opening file in function readFile()");
		
		exit(EXIT_FAILURE);
	}

	char *fileContent = NULL;

	List *charsRead = createEmptyList();

	int charRead = 0;
	char *charReadNullTerminated = NULL;

	do
	{
		charRead = fgetc(file);
		
		charReadNullTerminated = calloc(sizeof(char), 2);
		memcpy(charReadNullTerminated, &charRead, sizeof(char));

		if(charRead != -1 && !containsValue(exceptSymbols, charReadNullTerminated))
			appendValue(charsRead, charReadNullTerminated);
		
		free(charReadNullTerminated);
	}
	while(charRead != -1);
	
	fclose(file);
	
	fileContent = concatenateValues(charsRead, "");
	
	clearList(charsRead);
	free(charsRead);
	
	return fileContent;
}