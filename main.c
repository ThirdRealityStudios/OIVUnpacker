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
#include <winbase.h>
#include <Lmcons.h>
#include "internetHelper.h"
#include "directoryHelper.h"

// gcc ./main.c ./list.c ./subroutine.c ./stringHelper.c ./fileHelper.c ./xmlHelper.c ./directoryHelper.c ./reporter.c ./internetHelper.c ./test/stringHelperTest.c ./test/listTest.c ./test/directoryHelperTest.c ./test/xmlHelperTest.c -o main.exe -lws2_32

char *programTitle = "OIV Unpacker for OpenIV";
char *author = "https://www.youtube.com/@diesunddasDE";
char *assemblyXmlFilename = NULL;
char *assemblyXmlFilepath = NULL; // <- free() this variable after use later!
char *contentFolderPath = NULL; // <- same for this variable later!
char *oivArchiveFilepath = NULL;
char *destinationRealPath = NULL;

char *userName = NULL; // The current user name running this program.
char *appdataDirectoryPath = NULL; // The %appdata% folder, assumed on C: drive.
char *localDirectoryPath = NULL; // The directory path of %appdata%/Local/

char *temp = NULL; // for random, unrelated string operations.

/// @brief Checks whether the hardcoded product key is still valid via internet.
/// @param localDirectoryPath Path to your Local folder in %appdata% (a program folder will be created to save the entered key).
void activateProduct(char *localDirectoryPath)
{
	char *oivtoolsDirectory = safeStrCat(localDirectoryPath, "OIVTools\\");
	
	_mkdir_safe(oivtoolsDirectory);
	
	char *oivtoolsKeyFilePath = safeStrCat(oivtoolsDirectory, "activation.dat");
	
	char productKeyHardcoded[] = "CD87T-HFP4C-V7X7H-8VY68-W7D7M";

	FILE *keyFile = fopen(oivtoolsKeyFilePath, "r");
	
	// Product is valid for 1 year.
	struct tm *productActivationLossDateTm = calloc(sizeof(struct tm), 1);
	productActivationLossDateTm -> tm_mday = 30;
	productActivationLossDateTm -> tm_mon = 1; // Feb
	productActivationLossDateTm -> tm_year = 2025 - 1900; // Always subtract 1900 from the actual year.
	productActivationLossDateTm -> tm_hour = 23;
	productActivationLossDateTm -> tm_min = 59;
	
	time_t productActivationLossDate = mktime(productActivationLossDateTm);
	time_t liveTime = getLiveTime();
	
	if(liveTime == -1) // could not fetch the current internet live time (most likely user has no internet or NTP server is down?)
	{
		fputs("Activation failed. Please check your internet connection! (Hence, exiting now..)\n", stdout);

		exit(EXIT_FAILURE);
	}
	
	// See if the product key is still valid (no deep comparison here for the key itself, just checking for expiration which is also hardcoded).
	// If invalid, the customer needs to buy a new software with the key together.
	if(liveTime > productActivationLossDate)
	{
		fputs("Activation failed.\n"
			  "Your product key has expired (1 year is over).\n"
			  "Please get a new licensed software, e.g. write a simple message via Discord.\n"
			  "The Discord server is: https://discord.gg/tMkHnXVT", stdout);
	}
	
	if(keyFile == NULL) // Software was not activated yet.
	{
		char *productKeyEntered = calloc(sizeof(productKeyHardcoded), 1);
		
		do
		{
			fputs("Please enter your product key below for activation (internet required):\n> ", stdout);
			fgets(productKeyEntered, sizeof(productKeyHardcoded), stdin);
			fflush(stdin);
			
			fputs("\n", stdout);
			
			if(equals(productKeyEntered, productKeyHardcoded))
			{
				fputs("Activation successful. Thank you for choosing OIVUnpacker!\n", stdout);
					
				fprintf(stdout, "Key filepath: %s\n", oivtoolsKeyFilePath);
					
				keyFile = fopen(oivtoolsKeyFilePath, "wt"); // Save the key to a file (uncritical as it serves only to remember the entered key => key manipulation in the file will cause corrupt software on startup => delete file in such a case).
					
				fputs(productKeyHardcoded, keyFile);
					
				fclose(keyFile);
				
				free(productKeyEntered);
				productKeyEntered = NULL;
				
				free(oivtoolsDirectory);
				oivtoolsDirectory = NULL;

				break;
			}
			
			fputs("Your product key entered was invalid.\n", stdout);
		}
		while(1);
	}
	else // Software was activated yet (does not mean the key in the file is valid! So check that here..).
	{
		char keyReadFromFile[sizeof(productKeyHardcoded)] = {'\0'};
		fscanf(keyFile, "%s", keyReadFromFile);
		fclose(keyFile);
		
		printf("Could read the following key: %s, length: %d\n", keyReadFromFile, sizeof(productKeyHardcoded));
		
		// Abort the program if the key was manipulated or for whatever reasons..
		if(!equals(keyReadFromFile, productKeyHardcoded))
		{
			// This issue might appear maybe because of an old installation of the OIVTools,
			// so delete "activation.dat" in %appdata%/Local/OIVTools/
			remove(oivtoolsKeyFilePath);
			
			fputs("Your product key is invalid.\n"
				  "If you received a new license and this error appears,\n"
				  "just restart the program (tried to delete files from old installations).\n"
				  "If it still doesn't work then anymore,\n"
				  "do not hesitate to write a message on Discord here in a support channel:\n"
				  "https://discord.gg/tMkHnXVT\n"
				  "In such a case we will most likely find a simple solution.\n"
				  "When writing to the support channel,\n"
				  "please make sure to provide sufficient screenshots that show the problem briefly.\n"
				  "Otherwise it might be necessary to start a screenshare in a separate Discord call if screenshots\n"
				  "do not describe the problem sufficiently (depends on if you want it then, but unnecessary mess can be avoided definitively).\n", stdout);
			
			exit(EXIT_FAILURE);
		}
		
		puts("Your program is activated!");
	}
	
	free(oivtoolsKeyFilePath);
	oivtoolsKeyFilePath = NULL;
	

	char productActivationLossDateFormatted[100];
	
	strftime(productActivationLossDateFormatted, sizeof(productActivationLossDateFormatted), "%d.%m.%Y %H:%M", productActivationLossDateTm);
	
	fprintf(stdout, "Your product key is valid until: %s\n", productActivationLossDateFormatted);
	
	free(productActivationLossDateTm);
	productActivationLossDateTm = NULL;
	
	
}

void preInit()
{
	DWORD maxUserNameLength = UNLEN+1; // inclusive NULL-byte.
	
	userName = calloc(sizeof(char), maxUserNameLength);
	
	int userNameRetrievalSuccess = GetUserNameA(userName, &maxUserNameLength) != 0;
	
	// The user name is important for the runtime.
	// Hence, without, the program will exit.
	if(!userNameRetrievalSuccess)
	{
		fputs("Could not retrieve local user name (Hence, exiting now..)\n", stdout);
		
		exit(EXIT_FAILURE);
	}
	
	// Only used for temporary string calculations to determine the %appdata% directory path..
	temp = safeStrCat("C:\\Users\\", userName);
	
	// This is the usual %appdata% directory path now (on 99.99% of all Windows systems).
	appdataDirectoryPath = safeStrCat(temp, "\\AppData\\");
	
	localDirectoryPath = safeStrCat(appdataDirectoryPath, "Local\\");
	
	free(temp);
	temp = NULL;
}

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
	temp = safeStrCat("\\", assemblyXmlFilename);
	assemblyXmlFilepath = safeStrCat(destinationRealPath, temp);
	free(temp);
	temp = NULL;

	fprintf(stdout, "User name: %s\n", userName);
	fprintf(stdout, "AppData directory path: %s\n", appdataDirectoryPath);
	fprintf(stdout, "Local directory path: %s\n", localDirectoryPath);
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

/// @brief The entry point of this program.
///		   The program assumes the user account is located on the C: drive.
///		   If not, the program will not work.
int main(int argc, char **argv) // TODO Save activation file in %appdata%/Local/ThirdRealityStudios/OIVPacker
{
	preInit();
	
	// Example usage: ./main.exe "./Setup 1.oiv" "./myModpack/"
	
	activateProduct(localDirectoryPath); // Activation check efore anything else.

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
	
	// Will copy all files of every remaining <add> tag (after processing the <archive> tags)
	// to their corresponding representative GTA IV folder (so kinda bit like processArchiveXml() before).
	processRemainingAddXml(assemblyXmlFileContent, destinationRealPath);
	
	free(localDirectoryPath);
	localDirectoryPath = NULL;
	
	free(appdataDirectoryPath);
	appdataDirectoryPath = NULL;
	
	free(userName);
	userName = NULL;
	
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