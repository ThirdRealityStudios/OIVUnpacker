#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <Windows.h>
#include <assert.h>
#include "subroutine.h"
#include "stringHelper.h"
#include "xmlHelper.h"
#include "fileHelper.h"
#include "list.h"  
#include "test/stringHelperTest.h"
#include "test/listTest.h"
#include "test/directoryHelperTest.h"
#include "test/xmlHelperTest.h"

// gcc ./main.c ./list.c ./subroutine.c ./stringHelper.c ./fileHelper.c ./xmlHelper.c ./directoryHelper.c ./reporter.c ./test/stringHelperTest.c ./test/listTest.c ./test/directoryHelperTest.c ./test/xmlHelperTest.c -o main.exe

char *programTitle = "OIV Unpacker for OpenIV";
char *author = "https://www.youtube.com/@diesunddasDE";
char *assemblyXmlFilename = NULL;
char *assemblyXmlFilepath = NULL; // <- free() this variable after use later!
char *contentFolderPath = NULL; // <- same for this variable later!
char *oivArchiveFilepath = NULL;
char *destinationRealPath = NULL;

enum farben { test = -2, testf, foo = -7, nice, geil, kurwa };

void init(int argc, char **argv)
{
	argv++;
	
	oivArchiveFilepath = *argv++;
	destinationRealPath = *argv;
	
	oivArchiveFilepath = _fullpath(NULL, oivArchiveFilepath, 0);
	destinationRealPath = _fullpath(NULL, destinationRealPath, 0);

	// Create path to "content" folder
	// and filepath for file "assembly.xml" inside "content" folder
	contentFolderPath = safeStrCat(destinationRealPath, "\\content\\");
	assemblyXmlFilename = "assembly.xml";
	char *temp = safeStrCat("\\", assemblyXmlFilename);
	assemblyXmlFilepath = safeStrCat(destinationRealPath, temp);
	free(temp);
}

void testModules()
{
	testStringHelper();
	puts("");
	testList();
	puts("");
	testDirectoryHelper();
	puts("");
	testXmlHelper();
}

int main(int argc, char **argv)
{
	// Example usage: ./main.exe "./Setup 1.oiv" "./myModpack/"
	
	// testModules(); // Only uncomment if there are errors.
	
	welcomeUser(programTitle, author);
	assertCorrectMainArgs(argc, argv);
	// From here, it is safe to use main args now as intended.

	init(argc, argv); // do NOT change order! Leave it here after assertCorrectMainArgs(argc, argv) check..

	assertExistingOIVArchive(oivArchiveFilepath);

	// Ignore comment, only relevant for dev
	// ./7z.exe x -y ./trash/test.oiv -o"C:\Users\Konto\Desktop\OIVReader\trash\"
	
	char *temp0 = safeStrCat("7z.exe x -y \"", oivArchiveFilepath);
	char *temp1 = safeStrCat(temp0, "\" -o\"");
	free(temp0);
	temp0 = safeStrCat(temp1, destinationRealPath);
	free(temp1);
	char *command7zip = safeStrCat(temp0, "\" > NUL:");
	free(temp0);

	system(command7zip); // Use 7zip to unpack the .oiv archive.
	free(command7zip);
	
	// Check existence of assembly.xml after extracting the .oiv archive.
	assertExistingAssemblyXml(assemblyXmlFilepath);

	List *exceptSymbols = createEmptyList();
	appendValue(exceptSymbols, "\n"); // Remove line-breaks
	appendValue(exceptSymbols, "\t"); // Remove tab-symbols
	appendValue(exceptSymbols, "\r"); // Remove carriage-returns

	char *assemblyXmlFileContent = readFile(assemblyXmlFilepath, exceptSymbols);

	// Will copy all files of every <archive> tag to their corresponding representative GTA IV folder after unpacking.
	processArchiveXml(assemblyXmlFileContent, destinationRealPath);
	puts("");
	
	// Will copy all files of every remaining <add> tag (after processing the <archive> tags)
	// to their corresponding representative GTA IV folder (so kinda bit like processArchiveXml() before).
	processRemainingAddXml(assemblyXmlFileContent, destinationRealPath);
	puts("");
	
	free(assemblyXmlFilepath);
	assemblyXmlFilepath = NULL;
	
	free(contentFolderPath);
	contentFolderPath = NULL;
	
	free(oivArchiveFilepath);
	oivArchiveFilepath = NULL;

	free(exceptSymbols);
	exceptSymbols = NULL;
	
	free(assemblyXmlFileContent);
	assemblyXmlFileContent = NULL;
	
	free(destinationRealPath);
	destinationRealPath = NULL;

	fputs("Exiting program (done, everything successful, no errors)\n", stdout);

	return EXIT_SUCCESS;
}