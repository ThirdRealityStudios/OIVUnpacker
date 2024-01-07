#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringHelper.h"
#include "types.h"
#include "list.h"
#include "reporter.h"

List *getStringsByTagName(char *xmlDoc, char *tagName)
{
	char *xmlDocCopy = safeStrCat(xmlDoc, "");
	
	List *tagList = createEmptyList();
	
	char *beginTag = safeStrCat("<", tagName);
	
	char *temp = safeStrCat("</", tagName);
	char *endTag = safeStrCat(temp, ">");
	free(temp);
	
	while(1)
	{
		char *tagArea = copyIncludingBetween(xmlDocCopy, beginTag, endTag, 0);
		
		if(tagArea != NULL)
		{
			appendValue(tagList, tagArea);
			
			// Remove e.g. <archive ...</archive> pattern from "xmlDocCopy" (will assign an automatically new copy).
			int indexOfPattern = indexOf(xmlDocCopy, tagArea, 0);
			
			if(indexOfPattern == -1)
			{
				fprintf(stderr, "Fatal error: while-loop variable \"indexOfPattern\" is -1 in function getStringsByTagName() (which logically impossible) (Hence, exiting now..)");
				exit(EXIT_FAILURE);
			}
			
			char *temp = cutString(xmlDocCopy, indexOfPattern + 1, indexOfPattern + strlen(tagArea));
			free(xmlDocCopy);
			xmlDocCopy = temp;
			
			free(tagArea);
		}
		else // Break if there are no <archive ...</archive> patterns anymore.
			break;
	}
	
	free(beginTag);
	free(endTag);
	
	return tagList;
}

char* getValueByAttributeName(char *tagString, char *attributeName)
{
	char *attributeAssignmentPattern = safeStrCat(attributeName, "=\"");
	
	reportIfNULL(attributeAssignmentPattern, "attributeAssignmentPattern", "getValueByAttributeName", "safeStrCat()");
	
	char *attributeValue = copyExcludingBetween(tagString , attributeAssignmentPattern, "\"", 0);
	
	free(attributeAssignmentPattern);
	
	return attributeValue; // might return NULL (if attribute not found) or the value.
}

char *getFirstTagNameByXmlString(char *xmlString)
{
	char *possibleTagName0 = copyExcludingBetween(xmlString, "<", " ", 0);
	char *possibleTagName1 = copyExcludingBetween(xmlString, "<", ">", 0);
	
	if(possibleTagName0 == NULL && possibleTagName1 != NULL)
	{
		return possibleTagName1;
	}
	else if(possibleTagName0 != NULL && possibleTagName1 == NULL)
	{
		return possibleTagName0;
	}
	else if(possibleTagName0 != NULL && possibleTagName1 != NULL)
	{
		if(strlen(possibleTagName0) < strlen(possibleTagName1))
		{
			free(possibleTagName1);
			
			return possibleTagName0;
		}
		else if(strlen(possibleTagName0) == strlen(possibleTagName1))
		{
			// It is impossible as said in puts() below because
			// once there should a string beginning with "<" AND ending with " "
			// or the other possibility is that there is a string with "<" AND ending with ">".
			// It can't be both at the same time, e.g. the string
			// "<tag someAttribute="fooValue">" has a different length than
			// "<tag>" without an attribute.
			// If it turns out that both strings are of the equal length,
			// then there must an internal error, most likely resulting from the use
			// of the function copyExcludingBetween().
			fprintf(stderr, "Fatal Error: Found out that string \"possibleTagName0\" has the same length as \"possibleTagName1\" in function getFirstTagNameByXmlString() which is impossible (Hence, exiting now..)");
			
			exit(EXIT_FAILURE);
		}
		
		free(possibleTagName0);
	
		return possibleTagName1;
	}
	else
	{
		return NULL; // there is not a single tag that can be found in the given
	}				 // XML string.
}

char* getContentByTagString(char *tagString)
{
	// Read pure tag name, e.g. if tagString = "<tag>", then this
	// char* variable will point to a string "tag".
	char *tagName = getFirstTagNameByXmlString(tagString);
	
	if(tagName == NULL)
		return NULL; // invalid XML document but NO need here to exit the program as this can happen a little more often.
	
	char *enclosingContentPattern = ">"; // Look e.g. at "<tag(>)Content</tag>"
										 // Of course, you could argue that this could easily lead to misinterpretations
										 // but I assume you provide a valid XML document,
										 // so it's not my problem.
										 // If anyhting goes wrong here in this function,
										 // then NULL is simply returned, so you know it didn't work.
	
	char *preprocessedClosingContentPattern = safeStrCat("</", tagName); // Look e.g. at "<tag>Content(</tag)>"
	
	if(preprocessedClosingContentPattern == NULL)
		return NULL;
	
	char *closingContentPattern = safeStrCat(preprocessedClosingContentPattern, ">");
	
	if(closingContentPattern == NULL)
		return NULL;
	
	free(preprocessedClosingContentPattern); // Not needed anymore.
	preprocessedClosingContentPattern = NULL;
	
	// Copy the content area.
	char *content = copyExcludingBetween(tagString, enclosingContentPattern, closingContentPattern, 0);
	
	if(content == NULL)
		return NULL;
	
	free(tagName);
	tagName = NULL;

	free(closingContentPattern);
	closingContentPattern = NULL;

	return content; // might return NULL (if invalid XML document).
}