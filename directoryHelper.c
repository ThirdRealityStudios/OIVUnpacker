#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <ctype.h>
#include "stringHelper.h"
#include "list.h"
#include <assert.h>

char* getSubdirectoryPath(char *directoryPath, int level)
{
	char *realpath = _fullpath(NULL, directoryPath, 0); // remove double slashes and so on..

	if(realpath == NULL)
	{
		fprintf(stderr, "Error: Variable \"realpath\" is NULL in function getSubdirectoryPath() (probably is the given path format invalid from argument \"directoryPath\") (Hence, exiting now..)");
		
		exit(EXIT_FAILURE);
	}

	if(level < 0)
	{
		fprintf(stderr, "Error: Argument \"level\" is negative in function getSubdirectory() (Hence, exiting now..)");
		free(realpath);
		realpath = NULL;
		exit(EXIT_FAILURE);
	}

	char *driveName = calloc(sizeof(char), 4);
	memcpy(driveName, realpath, 3 * sizeof(char));

	if(level == 0)
	{
		free(realpath);
		realpath = NULL;
		
		return driveName; // Return drive letter folder (because level is 0, e.g. then C:\ or D:\ but not C:\subdirectory)
	}

	// Remove slash from drive name
	char *driveNameWithoutSlash = calloc(sizeof(char), 3);
	memcpy(driveNameWithoutSlash, driveName, sizeof(char) * 2);
	free(driveName);
	driveName = NULL;
	
	List *subdirectories = createEmptyList();
	
	// Copy drive name, e.g. the string "C:" or "D:" (without slash as you can read)
	appendValue(subdirectories, driveNameWithoutSlash);
	free(driveNameWithoutSlash);
	driveNameWithoutSlash = NULL;
	
	char *temp = cutString(realpath, 0, 2); // Remove drive name from "realpath"
	free(realpath);					        // for further processing.
	realpath = temp; // overwrite realpath with version without drive name.

	int subdirectoriesAdded = 0;
	int slashIndex = 0;
	
	for(; subdirectoriesAdded < level;)
	{
		slashIndex = indexOf(realpath, "\\", 0); // get first slash appearing.
		
		if(slashIndex == -1)
			break; // No more subdirectory names found to add to the list (Hence, also the argument "level" was chosen way too high).
			
		int subdirectoryNameLength = slashIndex; // e.g. the case "test\" or "folder\anotherfolder\".
			
		char *subdirectoryName = calloc(sizeof(char), subdirectoryNameLength + 1);
		memcpy(subdirectoryName, realpath, subdirectoryNameLength); // Copy folder name.

		// Append subdirectory name to list.
		appendValue(subdirectories, subdirectoryName);
		subdirectoriesAdded++;
			
		free(subdirectoryName);
		subdirectoryName = NULL;
			
		temp = cutString(realpath, 0, slashIndex); // Remove current directory inclusive slash at the end								// to prepare the next directory to read.
			
		free(realpath);
		realpath = temp; // Now realpath without current main directory (removed)
	}
		
	if(subdirectoriesAdded < level && !equals(realpath, ""))
	{
		appendValue(subdirectories, realpath);
		
		subdirectoriesAdded++;
	}
	
	free(realpath);
	realpath = NULL;

	char *subdirectoryPath = concatenateValues(subdirectories, "\\");
	
	clearList(subdirectories);
	free(subdirectories);
	subdirectories = NULL;

	return subdirectoryPath;
}

int _mkdir_safe(char *directoryPath)
{
	if(directoryPath == NULL)
	{
		fprintf(stderr, "Error: argument \"directoryPath\" is NULL in function _mkdir_safe()");
		
		exit(EXIT_FAILURE);
	}
	
	if(equals(directoryPath, "")) // Return suitable errno value for empty path string.
	{
		errno = 0;
		
		_mkdir(directoryPath);
		
		return errno;
	}

	int level = 1;

	char *lastDirPath = NULL;
	char *currentDirPath = getSubdirectoryPath(directoryPath, level);

	int deepestLastDirErrno = 0;

	while(1)
	{
		if(level > 1) // do not free when just initialized for first loop iteration.
		{
			if(!equals(lastDirPath, currentDirPath)) // after first iteration only create folders if their paths are different to each other.
			{
				errno = 0;

				_mkdir(currentDirPath);
				
				deepestLastDirErrno = errno;
			}
			else // current and last directory is equal to each other, so there is no other subdirectory left to calculate (break)..
			{
				break;
			}
			
			free(lastDirPath);
			lastDirPath = currentDirPath;
			
			currentDirPath = getSubdirectoryPath(directoryPath, ++level);
		}
		else // may create directory because this is still the first loop-iteration.
		{
			errno = 0;
			
			_mkdir(currentDirPath);
			
			deepestLastDirErrno = errno;

			currentDirPath = getSubdirectoryPath(directoryPath, ++level);
		}
	}

	free(lastDirPath);
	lastDirPath = NULL;
	free(currentDirPath);
	currentDirPath = NULL;

	return deepestLastDirErrno;
}