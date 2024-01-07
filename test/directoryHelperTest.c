#include <stdio.h>
#include <stdlib.h>
#include "../directoryHelper.h"
#include "../stringHelper.h"
#include <assert.h>

void testGetSubdirectoryPath()
{
	puts("getSubdirectoryPath()");
	
	char *temp = getSubdirectoryPath("C:\\test\\", 0);
	assert(temp != NULL);
	assert(equals(temp, "C:\\"));
	free(temp);
	
	temp = getSubdirectoryPath("X:\\\\test\\", 0);
	assert(temp != NULL);
	assert(equals(temp, "X:\\"));
	free(temp);
	
	temp = getSubdirectoryPath("X:\\\\test\\", 0);
	assert(temp != NULL);
	assert(equals(temp, "X:\\"));
	free(temp);
	
	temp = getSubdirectoryPath("C:\\test\\", 1);
	assert(temp != NULL);
	assert(equals(temp, "C:\\test\\"));
	free(temp);
	
	temp = getSubdirectoryPath("X:\\\\test\\", 1);
	assert(temp != NULL);
	assert(equals(temp, "X:\\test\\"));
	free(temp);
	
	temp = getSubdirectoryPath("X:\\\\test\\", 1);
	assert(temp != NULL);
	assert(equals(temp, "X:\\test\\"));
	free(temp);

	temp = getSubdirectoryPath("X:\\test\\foo0", 1);
	assert(temp != NULL);
	assert(equals(temp, "X:\\test\\"));
	free(temp);
	
	temp = getSubdirectoryPath("X:\\test\\foo0", 2);
	assert(temp != NULL);
	assert(equals(temp, "X:\\test\\foo0\\"));
	free(temp);

	temp = getSubdirectoryPath("X:\\test\\foos0\\", 1);
	assert(temp != NULL);
	assert(equals(temp, "X:\\test\\"));
	free(temp);
	
	temp = getSubdirectoryPath("X:\\test\\foo0\\", 2);
	assert(temp != NULL);
	assert(equals(temp, "X:\\test\\foo0\\"));
	free(temp);

	temp = getSubdirectoryPath("C:\\Users\\Konto\\Desktop\\OIVReader\\modpack\\pc\\data\\cdimages\\scripttxds.img\\", 7);
	assert(temp != NULL);
	assert(equals(temp, "C:\\Users\\Konto\\Desktop\\OIVReader\\modpack\\pc\\data\\"));
	free(temp);
	
	puts("OK");
}

void testDirectoryHelper()
{
	puts("Testing modules of directoryHelper..");
	puts("");
	
	testGetSubdirectoryPath();
	puts("");
	
	puts("Finished testing.");
}