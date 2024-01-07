#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEV_DEBUG

int equals(char *str1, char *str2)
{
	if(str1 == NULL && str2 == NULL)
		return 1; // shallow equality
	
	if(str1 == NULL || str2 == NULL)
		return 0; // at least one of them is not equal (shallow)
	
	return strcmp(str1, str2) == 0; // are both strings are equal to each other?.
}

char *safeStrCat(char *destination, char *source)
{
	if(destination == NULL || source == NULL)
		return NULL;
	
	char *concatenated = calloc(strlen(destination) + strlen(source) + 1, sizeof(char));

	memcpy(concatenated, destination, strlen(destination) * sizeof(char));

	strcat(concatenated, source);

	return concatenated;
}

int indexOf(char *string, char *pattern, int beginSearchIndex)
{
	if(beginSearchIndex < 0 || string == NULL || pattern == NULL) // prevent invalid index from programer.
		return -1;
	
	int stringLength = strlen(string);	
	
	for(int i = beginSearchIndex; i < stringLength; i++)
	{
		if(string[i] == pattern[0]) // Find place of possible pattern.
		{
			int patternLength = strlen(pattern);
			
			int brokeInnerLoop = 0;
			
			for(int j = 0; j < patternLength; j++)
			{
				if((i+j) >= stringLength) // pattern exceeds area of string.
				{
					return -1; // pattern not found in the string.
				}
				
				if(string[i+j] != pattern[j])
				{
					j = patternLength; // Current string comparision mismatches pattern, so break inner loop (=> search for new possible match).
					
					brokeInnerLoop = 1;
					continue;
				}
			}
			
			if(!brokeInnerLoop)
			{
				// Return i because the inner loop could completely verify the current possible match with the pattern => simply a match.
				return i;
			}
		}
	}
	
	// Completely ran through the string but found nothing.
	return -1;
}

int lastIndexOf(char *string, char *pattern, int beginSearchIndex)
{
	if(beginSearchIndex < 0 || string == NULL || pattern == NULL) // prevent invalid index from programer.
		return -1;
	
	int stringLength = strlen(string);
	
	int lastIndexFound = -1;
	
	for(int i = beginSearchIndex; i < stringLength; i++)
	{
		if(string[i] == pattern[0]) // Find place of possible pattern.
		{
			int patternLength = strlen(pattern);
			
			int brokeInnerLoop = 0;
			
			for(int j = 0; j < patternLength; j++)
			{
				if((i+j) >= stringLength) // pattern exceeds area of string.
				{
					return -1; // pattern not found in the string.
				}
				
				if(string[i+j] != pattern[j])
				{
					j = patternLength; // Current string comparision mismatches pattern, so break inner loop (=> search for new possible match).
					
					brokeInnerLoop = 1;
					continue;
				}
			}
			
			if(!brokeInnerLoop)
			{
				// Remember i because the inner loop could completely verify the current possible match with the pattern => simply a match.
				lastIndexFound = i;
			}
		}
	}
	
	// Completely ran through the string but found nothing.
	return lastIndexFound;
}

char* copyIncludingBetween(char *string, char *startPattern, char *endPattern, int beginSearchIndex)
{
	if(string == NULL || startPattern == NULL || endPattern == NULL)
		return NULL;
	
	int beginIndex = indexOf(string, startPattern, beginSearchIndex);

	if(beginIndex == -1) // pattern and thus the whole string not found.
	{
		free(startPattern);
		free(endPattern);
		
		return NULL;
	}
	
	// Make sure the end pattern search begins AFTER the first (start) pattern.
	beginSearchIndex = beginIndex + strlen(startPattern);
	
	int endIndex = indexOf(string, endPattern, beginSearchIndex);

	if(endIndex == -1)
	{
		return NULL; // also say "not found" if the tag was not closed correctly.
	}
	
	endIndex += strlen(endPattern); // Add length of closing tag to include everything to end.

	int n = endIndex - beginIndex;

	char *stringArea = calloc(sizeof(char), n + 1);

	memcpy(stringArea, string + beginIndex, n);

	return stringArea;
}

char* cutString(char *string, int fromIndex, int toIndex)
{
	if(string == NULL)
	{
		return NULL;
	}
	
	int stringLength = strlen(string);
	
	if(stringLength == 0)
	{
		// You cannot cut an empty string because it is empty yet.
		// Hence, NULL is returned.
		
		return NULL;
	}
	
	if(fromIndex > toIndex)
	{
		// Argument "fromIndex" was greater than argument "toIndex"
		
		return NULL;
	}
	
	if(fromIndex < 0)
	{
		// Argument "fromIndex" was smaller than 0
		
		return NULL;
	}
	
	if(toIndex < 0)
	{
		// Argument "toIndex" was smaller than 0
		
		return NULL;
	}
	
	if(toIndex >= stringLength)
	{
		// Argument "toIndex" is out of bounds for string argument "string"
		
		return NULL;
	}
	
	// If there is only one symbol to be cut,
	// then return an empty string.
	if(stringLength == 1)
	{
		return calloc(sizeof(char), 1);
	}

	int diffLength = toIndex - fromIndex + 1;
	
	int newStringLength = stringLength - diffLength;
	
	char *cutString = calloc(sizeof(char), newStringLength + 1); // + 1 because of NULL-byte. OK
	
	if(fromIndex == toIndex) // Only 1 symbol to cut off.
	{
		int cutIndex = fromIndex;
		
		if(cutIndex == 0) // If to cut off first symbol.
		{	 			  // This case will only take effect if there is a string of length at least 2!
						  // If you want to imagine the function of this case, take string = "AB" with fromIndex = 0 and toIndex = 0.
			memcpy(cutString, string + 1, newStringLength); // Copy everything from "string" except first symbol.
		}
		else if(cutIndex == (stringLength - 1)) // If to cut off last symbol.
		{	 									// This case will only take effect if there is a string of length at least 2!
												// If you want to imagine the function of this case, take string = "AB" with fromIndex = 1 and toIndex = 1.
			memcpy(cutString, string, newStringLength); // Copy everything from "string" except last symbol.
		}
		else // if to cut off one (!) symbol somewhere in the string (except first and last symbol).
		{	 // This case will only take effect if there is a string of length at least 3!
			 // If you want to imagine the function of this case, take string = "ABC" with fromIndex = 1 and toIndex = 1.
			int leftPartLength = cutIndex;
			int rightPartLength = newStringLength - leftPartLength;
			
			memcpy(cutString, string, leftPartLength);
			memcpy(cutString + leftPartLength, string + leftPartLength + diffLength, rightPartLength);
		}
	}
	else // Larger area to cut off (and fromIndex and toIndex are ALWAYS different here).
	{
		int stringEndIndex = stringLength - 1;
	
		// If to cut off the whole string.
		// This operation will logically only take effect if there is a string of length at least (!) 2.
		// This is because the first if-statement above with "fromIndex == toIndex" will take care for strings,
		// consisting only out of 1 symbol (if you want to cut off that 1 symbol, meaning the whole string).
		// Hence, it is logical, that this part will only operate for strings of at least length 2.
		// If you want to imagine the function of this case, take string = "AB" with fromIndex = 0 and toIndex = 1.
		if(fromIndex == 0 && toIndex == stringEndIndex)
		{
			return cutString; // Return an empty string.
		}
		else if(fromIndex == 0 && toIndex < stringEndIndex) // if to cut off the left part of "string", whereas it does begin at index 0.
		{													// It is very logical in this case that this operation will only take effect if there is a string of a length with at least 3!
															// If you want to imagine the function of this case, take string = "ABC" with fromIndex = 0 and toIndex = 1.
			memcpy(cutString, string + toIndex + 1, newStringLength);
		}
		else if(fromIndex > 0 && toIndex == stringEndIndex) // if to cut off the right (!) part of "string", whereas the part does end at index strlen(string) - 1.
		{													// It is very logical in this case that this operation will only take effect if there is a string of a length with at least 3!
															// If you want to imagine the function of this case, take string = "ABC" with fromIndex = 1 and toIndex = 2.
			memcpy(cutString, string, newStringLength);
		}
		else // if to cut off an area / partial string just somewhere between index 0 and index strlen(string) - 1 (excluding those!).
		{    // In this case it is also very logical that "string" does have at least a length of 4!
			 // If you want to imagine the function of this case, take string = "ABCD" with fromIndex = 1 and toIndex = 2.
			memcpy(cutString, string, fromIndex);

			int leftPartLength = fromIndex;
			int rightPartLength = stringLength - toIndex - 1;
			
			memcpy(cutString + leftPartLength, string + toIndex + 1, rightPartLength);
		}
	}
	
	return cutString;
}

char* cutAllWords(char *string, char *word)
{
	if(string == NULL)
	{
		fprintf(stderr, "Error: argument \"string\" is NULL in function cutAllWords() (Hence, exiting now..)");
		exit(EXIT_FAILURE);
	}
	
	if(word == NULL)
	{
		fprintf(stderr, "Error: argument \"word\" is NULL in function cutAllWords() (Hence, exiting now..)");
		exit(EXIT_FAILURE);
	}
	
	// Copies string first.
	char *shortenedString = safeStrCat("", string);
	
	int matchIndex = 0;
	
	while(1)
	{
		matchIndex = indexOf(shortenedString, word, 0);
		
		if(matchIndex == -1)
		{
			return shortenedString;
		}
		
		char *temp = shortenedString;
		
		shortenedString = cutString(shortenedString, matchIndex, matchIndex + strlen(word) - 1);
		
		free(temp);
	}
}

char* copyExcludingBetween(char *string, char *startPattern, char *endPattern, int beginSearchIndex)
{
	char *stringIncludingPatterns = copyIncludingBetween(string, startPattern, endPattern, beginSearchIndex);
	
	if(stringIncludingPatterns == NULL)
	{
		return NULL; // pattern and thus the whole string not found.
	}
	
	int startPatternIndex = indexOf(stringIncludingPatterns, startPattern, 0);
	
	if(startPatternIndex == -1)
	{
		fprintf(stderr, "Fatal Error: \"startPatternIndex\" is -1 after calling indexOf() in function copyExcludingBetween() (this is logically impossible) (Hence, exiting now..)");
		
		exit(EXIT_FAILURE);
	}
	
	char *temp = cutString(stringIncludingPatterns, startPatternIndex, startPatternIndex + strlen(startPattern) - 1);
	
	if(temp == NULL)
	{
		fprintf(stderr, "Fatal Error: \"temp\" is NULL after calling cutString() in function copyExcludingBetween() (this is logically impossible) (Hence, exiting now..)");
		
		exit(EXIT_FAILURE);
	}
	
	free(stringIncludingPatterns);
	
	int endPatternIndex = indexOf(temp, endPattern, 0);
	
	if(endPatternIndex == -1)
	{
		fprintf(stderr, "Fatal Error: \"endPatternIndex\" is -1 after calling indexOf() in function copyExcludingBetween() (this is logically impossible) (Hence, exiting now..)");
		
		exit(EXIT_FAILURE);
	}
	
	char *stringExcludingPatterns = cutString(temp, endPatternIndex, endPatternIndex + strlen(endPattern) - 1);
	
	free(temp);
	
	return stringExcludingPatterns;
}