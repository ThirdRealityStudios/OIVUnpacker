#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <dirent.h>
#include <errno.h>
#include "list.h"
#include "stringHelper.h"
#include "directoryHelper.h"
#include "xmlHelper.h"
#include "reporter.h"

void assertExistingOIVArchive(char *oivArchiveFilepath)
{
	FILE *oivArchiveFile = fopen(oivArchiveFilepath, "r");

	if(oivArchiveFile == NULL)
	{
		fprintf(stderr, "OpenIV file ending with \".oiv\" not found! (Hence, exiting now)");

		exit(EXIT_FAILURE);
	}

	fclose(oivArchiveFile);
}

void assertExistingAssemblyXml(char *assemblyXmlFilepath)
{
	FILE *assemblyXmlFile = fopen(assemblyXmlFilepath, "r");

	if(assemblyXmlFile == NULL)
	{
		fprintf(stderr, "File \"assembly.xml\" not found in your source folder! (Hence, exiting now)");

		exit(EXIT_FAILURE);
	}
	
	fclose(assemblyXmlFile);
}

void assertCorrectMainArgs(int argc, char **argv)
{
	if(argc == 3)
	{
		argv++;
		
		puts("\nIf there are any errors,\n"
		     "please make sure you restart your computer and have elevated rights.\n");
	}
	else
	{
		if(argc <= 1)
			fprintf(stderr, "Please specifiy a source folder and destination folder!");
		else if(argc == 2)
			fprintf(stderr, "Please also specify a destination folder!");
		else
			fprintf(stderr, "Too many arguments!");
		
		exit(EXIT_FAILURE);
	}
}

void welcomeUser(char *programTitle, char *author)
{
	SetConsoleTitle(programTitle);
	
	printf("\n[%s]\n\n"
		   "Discord: %s\n\n",
	       programTitle, author);
}

// Helps to remember the decision in function processAddTagXmlString()
// whether to replace existing files while moving or not.
int PROJECT_OIV_remember_decicision = 0;

void processAddTagXmlString(char *addXmlString, char *destinationRealPath, char *optionalArchivePathValue)
{
	reportIfNULL(addXmlString, "addXmlString", "processRemainingAddXml()", "function caller");
	reportIfNULL(destinationRealPath, "destinationRealPath", "processRemainingAddXml()", "function caller");
	
	char *temp = NULL; // only (as it says) for temporary calculations (here string concats etc.).

	char *sourceFileLocalOIVPath = NULL;
	char *sourceFilePath = NULL;
	char *gtaIVDirectoryFilePath = NULL;
	char *destinationFilePath = NULL;
	char *destinationDirectory = NULL;

	sourceFileLocalOIVPath = getValueByAttributeName(addXmlString, "source");
	reportIfNULL(sourceFileLocalOIVPath, "sourceFileLocalOIVPath", "processRemainingAddXml()", "getValueByAttributeName()");
	
	if(optionalArchivePathValue == NULL)
	{
		gtaIVDirectoryFilePath = getContentByTagString(addXmlString);
		reportIfNULL(gtaIVDirectoryFilePath, "gtaIVDirectoryFilePath", "processRemainingAddXml()", "getContentByTagString()");
	}
	else // this part here will also consider the "path" attribute value of a archive tag.
	{	 // TODO Test functionality!
		temp = getContentByTagString(addXmlString);
		reportIfNULL(temp, "temp", "processRemainingAddXml()", "getContentByTagString()");
		
		gtaIVDirectoryFilePath = safeStrCat(optionalArchivePathValue, temp);
		reportIfNULL(gtaIVDirectoryFilePath, "gtaIVDirectoryFilePath", "processRemainingAddXml()", "safeStrCat()");
		
		free(temp);
		temp = NULL;
	}
		
	// Building up the source filepath from here, pointing to the files in the .oiv archive..
	temp = safeStrCat(destinationRealPath, "\\content\\");
	reportIfNULL(temp, "temp", "processRemainingAddXml()", "safeStrCat()");
		
	sourceFilePath = safeStrCat(temp, sourceFileLocalOIVPath); // Now the variable sourceFilePath is finally usable and created.
	reportIfNULL(sourceFilePath, "sourceFilePath", "processRemainingAddXml()", "safeStrCat()");
	free(temp);
	temp = NULL;
	
	temp = safeStrCat(destinationRealPath, "\\");
	reportIfNULL(temp, "temp", "processRemainingAddXml()", "safeStrCat()");
		
	destinationFilePath = safeStrCat(temp, gtaIVDirectoryFilePath);
	reportIfNULL(destinationFilePath, "destinationFilePath", "processRemainingAddXml()", "safeStrCat()");
	free(temp);
	temp = NULL;
		
	// Calculating the destination directory of your representative GTA IV folder (so removing the file name inclusive extension here).
	destinationDirectory = cutString(destinationFilePath, lastIndexOf(destinationFilePath, "\\", 0), strlen(destinationFilePath) - 1);
	reportIfNULL(destinationDirectory, "destinationDirectory", "processRemainingAddXml()", "cutString()");

	// Make sure folder does exist before moving.
	int successMkdir = _mkdir_safe(destinationDirectory);
	
	// Moving files from sourceFilePath to destinationFilePath from here..
	int successMoveFile = rename(sourceFilePath, destinationFilePath);

	// If moving was not successful, ask the user what to do?
	if(PROJECT_OIV_remember_decicision == 0 && successMoveFile == -1)
	{
		char *decision = NULL;
		
		do
		{
			decision = calloc(sizeof(char), 3);
			
			fprintf(stdout, "Replace existing file? \"%s\"\n"
						"(\"y\" or \"n\" | \"yr\" or \"nr\" to remember decision for all further files)\n"
						"Your input: ", destinationFilePath);

			fgets(decision, 3, stdin);
			
			fputs("\n", stdout);
			
			// Bugfix:
			// "Remove" line-break which was also read, when entering "y" or "n"
			// (which is in reality then "y\n" or "n\n", but not for "yr" or "nr" because it will get replaced by '\0' in third index).
			if(decision[1] == '\n')
				decision[1] = 0;
			
			fflush(stdin);

			if(equals(decision, "y"))
			{
				remove(destinationFilePath);
				rename(sourceFilePath, destinationFilePath); // if it still doesn't work here, I don't care..

				break;
			}
			else if(equals(decision, "n"))
			{
				break;
			}
			else if(equals(decision, "nr"))
			{
				PROJECT_OIV_remember_decicision = -1;
	
				break;
			}
			else if(equals(decision, "yr"))
			{
				PROJECT_OIV_remember_decicision = 1;
			
				remove(destinationFilePath);
				rename(sourceFilePath, destinationFilePath); // if it still doesn't work here, I don't care..

				break;
			}
			else // wrong input => delete decision and prepare for next input iteration
			{
				free(decision);
				decision = NULL;
				
				fflush(stdin);
			}
		}
		while(1);
		
		free(decision);
		decision = NULL;
	}
	else if(PROJECT_OIV_remember_decicision && !successMoveFile)
	{
		if(PROJECT_OIV_remember_decicision == 1)
		{
			remove(destinationFilePath);
			rename(sourceFilePath, destinationFilePath);
		}
	}

	free(destinationDirectory);
	destinationDirectory = NULL;
		
	free(gtaIVDirectoryFilePath);
	gtaIVDirectoryFilePath = NULL;
		
	free(sourceFileLocalOIVPath);
	sourceFileLocalOIVPath = NULL;
}

void processArchiveXml(char *assemblyXmlFileContent, char *destinationRealPath)
{
	char *assemblyXmlContentArea = copyIncludingBetween(assemblyXmlFileContent, "<content", "</content>", 0);

	List *copyArchiveInstructions = getStringsByTagName(assemblyXmlContentArea, "archive");

	char *archiveXmlString = NULL;
	char *archivePath = NULL;
	char *destinationArchivePath = NULL;
	char *temp = NULL;

	for(int i = 0; i < sizeOf(copyArchiveInstructions); i++)
	{
		archiveXmlString = getValue(copyArchiveInstructions, i);
		archivePath = getValueByAttributeName(archiveXmlString, "path");
		
		temp = safeStrCat(destinationRealPath, "\\");
		destinationArchivePath = safeStrCat(temp, archivePath);
		free(temp);

		_mkdir_safe(destinationArchivePath);
		
		List *copyAddInstructions = getStringsByTagName(archiveXmlString, "add");
		
		for(int j = 0; j < sizeOf(copyAddInstructions); j++)
		{
			char *addXmlString = getValue(copyAddInstructions, j);
			
			// Process <add>...</add> statement to create necessary directories and copy files.
			processAddTagXmlString(addXmlString, destinationRealPath, archivePath);
			
			free(addXmlString);
			addXmlString = NULL;
		}

		clearList(copyAddInstructions);
		free(copyAddInstructions);
		copyAddInstructions = NULL;
		
		free(archivePath);
		free(archiveXmlString);
	}

	clearList(copyArchiveInstructions);
	free(copyArchiveInstructions);

	free(assemblyXmlContentArea);
}

void processRemainingAddXml(char *assemblyXmlFileContent, char *destinationRealPath)
{
	reportIfNULL(assemblyXmlFileContent, "assemblyXmlFileContent", "processRemainingAddXml", "function caller");
	reportIfNULL(destinationRealPath, "destinationRealPath", "processRemainingAddXml", "function caller");
	
	char *assemblyXmlContentArea = copyIncludingBetween(assemblyXmlFileContent, "<content", "</content>", 0);
	
	// Now remove all <archive>...</archive> tags.
	
	// Pattern to remove from "assemblyXmlContentArea".
	char *removeString = NULL;
	
	// Temp string to cut off strings from "assemblyXmlContentArea"
	// before assigning to it.
	char *temp = NULL;
	
	// Preprocessing..
	while(1)
	{
		removeString = copyIncludingBetween(assemblyXmlContentArea, "<archive", "</archive>", 0);
		
		// If there is no <archive ... </archive> pattern left to remove anymore,
		// then break.
		if(removeString == NULL)
			break;
		
		temp = cutAllWords(assemblyXmlContentArea, removeString);
		
		free(removeString);
		removeString = NULL;
		
		free(assemblyXmlContentArea);
		assemblyXmlContentArea = NULL;
		
		// Assign the shortened version / string.
		assemblyXmlContentArea = temp;
		
		temp = NULL;
	}

	// From here, there are no <archive ... </archive> patterns left.

	// Now fetch all <add ... </add> patterns to process them too with creating directories and such.
	List *copyAddInstructions = getStringsByTagName(assemblyXmlContentArea, "add");

	char *addXmlString = NULL;

	for(int i = 0; i < sizeOf(copyAddInstructions); i++)
	{
		addXmlString = getValue(copyAddInstructions, i);
		reportIfNULL(addXmlString, "addXmlString", "processRemainingAddXml()", "getValue()");
		
		// Process <add>...</add> statement to create necessary directories and copy files.
		processAddTagXmlString(addXmlString, destinationRealPath, NULL);
		
		free(addXmlString);
		addXmlString = NULL;
	}
	
	clearList(copyAddInstructions);
	free(copyAddInstructions);
	copyAddInstructions = NULL;

	free(assemblyXmlContentArea);
	assemblyXmlContentArea = NULL;
}