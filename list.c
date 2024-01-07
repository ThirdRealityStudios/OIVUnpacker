#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "stringHelper.h"

Node* createNode()
{
	return calloc(sizeof(Node), 1);
}

List* createEmptyList()
{
	return calloc(sizeof(List), 1);
}

int sizeOf(List *list)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function sizeOf()");
		exit(EXIT_FAILURE);
	}
	
	if(list -> size < 0)
	{
		fprintf(stderr, "Error: Got negative size for argument \"list\" in function sizeOf()");
		exit(EXIT_FAILURE);
	}
	
	return list -> size;
}

void printList(List *list)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function printList()");
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
		fprintf(stdout, "Nothing to show. List is empty!");
	
	Node *current = NULL;
	
	for(int i = 0; i < sizeOf(list); i++)
	{
		if(i == 0)
			current = list -> tail;
		else
			current = current -> next;
		
		printf("(%s)", current -> value);
		
		if(i < sizeOf(list) - 1)
		{
			printf(" -> ");
		}
	}
}

Node* getNode(List *list, int index)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function getNode()");
		exit(EXIT_FAILURE);
	}
	
	if(index < 0)
	{
		fprintf(stderr, "Error: Passed negative value for argument \"index\" in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	if(index >= sizeOf(list))
	{
		fprintf(stderr, "Error: Passed value for argument \"index\" is out of bounds in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	Node *current = NULL;
	
	for(int i = 0; i <= index; i++)
	{
		if(i == 0)
			current = list -> tail;
		else
			current = current -> next;
	}
	
	return current;
}


void removeValue(List *list, int index)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function removeValue()");
		exit(EXIT_FAILURE);
	}

	if(index < 0)
	{
		fprintf(stderr, "Error: Passed negative value for argument \"index\" in function removeValue()");
		exit(EXIT_FAILURE);
	}
	
	if(index >= sizeOf(list))
	{
		fprintf(stderr, "Error: Passed value for argument \"index\" is out of bounds in function removeValue()");
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
	{
		fprintf(stderr, "Error: Function removeValue() can not be used for empty lists");
		exit(EXIT_FAILURE);
	}
	
	Node *node = getNode(list, index);
	
	if(node == NULL)
	{
		fprintf(stderr, "Fatal Error: Retrieved NULL instead of a removable node object in function removeValue()");
		exit(EXIT_FAILURE);
	}

	// Delete old value correctly.
	free(node -> value);
	node -> value = NULL;

	Node *before = node -> before;
	Node *next = node -> next;

	if(index == 0)
	{
		if(sizeOf(list) == 1)
		{
			// Simply make the list look empty here.
			//
			// Note:
			// simply calling here clearList(list) would make an endless circular call
			// because clearList() relies on removeValue() yet!
			
			// removeValue({"-12312 asd"}, 0);
			
			free(list -> tail);
			
			list -> tail = NULL;
			list -> head = NULL;
			
			list -> size = 0;
		}
		else if(sizeOf(list) == 2)
		{			
			free(list -> tail);
			
			next -> before = NULL;
			
			list -> tail = next;
			list -> head = next;
			
			list -> size = 1;
		}
		else // at least 3 elements
		{
			free(list -> tail);
			
			next -> before = NULL;
			
			list -> tail = next;
			
			list -> size--;
		}
	}
	else if(index == sizeOf(list) - 1)
	{
		if(sizeOf(list) == 2) // index is in this case exactly 1.
		{
			free(node);

			list -> tail = before;
			list -> head = before;
			
			before -> next = NULL;

			list -> size = 1;
		}
		else // List size is at least 3.
		{ 	 // e.g. for list size of 3 => index = 2 in this case!
			free(node);

			list -> head = before;

			before -> next = NULL;

			list -> size--;
		}
	}
	else // value / node somewhere in between the list.
	{	 // Also note that for this case the list size is logically at least 3.		
		free(node);
		
		before -> next = next;
		next -> before = before;
		
		list -> size--;
	}
}

void clearList(List *list)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function clearList()");
		exit(EXIT_FAILURE);
	}

	while(sizeOf(list) != 0)
	{
		removeValue(list, 0);
	}
}

void insertValue(List *list, int index, char *value)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	if(value == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"value\" in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	if(strlen(value) == 0)
	{
		fprintf(stderr, "Error: Passed empty string for argument \"value\" in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
	{
		fprintf(stderr, "Error: Function insertValue() may not be used for empty lists (use appendValue() instead)");
		exit(EXIT_FAILURE);
	}
	
	if(index < 0)
	{
		fprintf(stderr, "Error: Passed negative value for argument \"index\" in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	
	if(index >= sizeOf(list))
	{
		fprintf(stderr, "Error: Passed value for argument \"index\" is out of bounds in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"value\" in function insertValue()");
		exit(EXIT_FAILURE);
	}
	
	Node *node = createNode();
	
	int valueLength = strlen(value);
	
	// Copy "value" to realize the concept call-by-value.
	node -> value = calloc(sizeof(char), valueLength + 1);
	memcpy(node -> value, value, valueLength);
	
	if(index == 0)
	{		
		/*
		Job of function appendValue() yet, so no need!
		
		if(sizeOf(list) == 1)
		{
			list -> head = list -> tail;
		}
		*/
		
		node -> next = list -> tail;
		
		list -> tail -> before = node;
		
		list -> tail = node;
	}
	else if(index == sizeOf(list) - 1)
	{
		node -> next = list -> head;
		node -> before = list -> head -> before;
		
		node -> before -> next = node;
		node -> next -> before = node;
	}
	else
	{
		Node *next = getNode(list, index + 1);
		Node *before = getNode(list, index - 1);
		
		node -> next = next;
		node -> before = before;
		
		node -> next -> before = node;
		node -> before -> next = node;
	}
	
	list -> size++;
}

void appendValue(List *list, char *value)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function appendValue()");
		exit(EXIT_FAILURE);
	}
	
	if(value == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"value\" in function appendValue()");
		exit(EXIT_FAILURE);
	}
	
	if(strlen(value) == 0)
	{
		fprintf(stderr, "Error: Passed empty string for argument \"value\" in function appendValue()");
		exit(EXIT_FAILURE);
	}
	
	Node *node = createNode();
	
	int valueLength = strlen(value);
	
	// Realizing here the concept call-by-value.
	node -> value = calloc(sizeof(char), valueLength + 1);
	memcpy(node -> value, value, valueLength);
	
	if(sizeOf(list) == 0)
	{
		list -> tail = node;
		list -> head = node;
		
		list -> size = 1;
	}
	else
	{
		list -> head -> next = node;

		node -> before = list -> head;

		list -> head = node;
		
		list -> size++;
	}
}

char* getValue(List *list, int index)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function getValue()");
		exit(EXIT_FAILURE);
	}
	
	if(index < 0)
	{
		fprintf(stderr, "Error: Passed negative value for argument \"index\" in function getValue()");
		exit(EXIT_FAILURE);
	}
	
	if(index >= sizeOf(list))
	{
		fprintf(stderr, "Error: Passed value for argument \"index\" is out of bounds in function getValue()");
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
	{
		fprintf(stderr, "Error: Function getValue() can not be used for empty lists");
		exit(EXIT_FAILURE);
	}
	
	char *listValueReference = getNode(list, index) -> value;
	
	int valueLength = strlen(listValueReference);
	
	// Realize the concept call-by-value.
	char *valueCopied = calloc(sizeof(char), valueLength + 1);
	memcpy(valueCopied, listValueReference, valueLength);
	
	return valueCopied;
}

void replaceValue(List *list, int index, char *newValue)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function replaceValue()");
		exit(EXIT_FAILURE);
	}
	
	if(newValue == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"newValue\" in function replaceValue()");
		exit(EXIT_FAILURE);
	}
	
	if(strlen(newValue) == 0)
	{
		fprintf(stderr, "Error: Passed empty string for argument \"newValue\" in function replaceValue()");
		exit(EXIT_FAILURE);
	}
	
	if(index < 0)
	{
		fprintf(stderr, "Error: Passed negative value for argument \"index\" in function replaceValue()");
		exit(EXIT_FAILURE);
	}
	
	if(index >= sizeOf(list))
	{
		fprintf(stderr, "Error: Passed value for argument \"index\" is out of bounds in function replaceValue()");
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
	{
		fprintf(stderr, "Error: Function replaceValue() can not be used for empty lists");
		exit(EXIT_FAILURE);
	}
	
	Node *node = getNode(list, index);
	
	if(node == NULL)
	{
		fprintf(stderr, "Fatal Error: Retrieved NULL instead of a node object in function replaceValue()");
		exit(EXIT_FAILURE);
	}

	int valueLength = strlen(newValue);

	// Delete old value correctly.
	free(node -> value);
	node -> value = NULL;

	// Also realize in this function for the argument "newValue"
	// the concept call-by-value.
	node -> value = calloc(sizeof(char), valueLength + 1);
	memcpy(node -> value, newValue, valueLength);
}

int containsValue(List *list, char *value)
{
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function containsValue()");
		
		exit(EXIT_FAILURE);
	}
	
	if(value == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"value\" in function containsValue()");
		
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
		return 0;
	
	if(sizeOf(list) == 1)
		return equals(list -> tail -> value, value);
	
	Node *current = list -> tail;
	
	while(current != NULL)
	{
		if(equals(current -> value, value))
			return 1;
		
		current = current -> next;
	}
	
	return 0;
}

char* concatenateValues(List *list, char *separatorString)
{	
	if(list == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"list\" in function concatenate()");
		
		exit(EXIT_FAILURE);
	}
	
	if(separatorString == NULL)
	{
		fprintf(stderr, "Error: Passed NULL for argument \"separatorString\" in function concatenate()");
		
		exit(EXIT_FAILURE);
	}
	
	if(sizeOf(list) == 0)
		return calloc(sizeof(char), 1);
	
	int totalBytes = 0;
	
	for(int i = 0; i < sizeOf(list); i++)
	{
		char *currentValue = getValue(list, i);
		
		totalBytes += strlen(currentValue) + strlen(separatorString);
	}
	
	int separatorStringLength = strlen(separatorString);
	
	char *concatenatedString = calloc(sizeof(char), totalBytes + 1);
	
	int index = 0;
	
	Node *current = NULL;
	
	for(int i = 0; i < sizeOf(list); i++)
	{
		if(i == 0)
			current = list -> tail;
		else
			current = current -> next;
		
		int valueLength = strlen(current -> value); // 5
		
		memcpy(concatenatedString + index, current -> value, valueLength);
		
		if(separatorStringLength > 0)
			memcpy(concatenatedString + index + valueLength, separatorString, separatorStringLength);
		
		index += valueLength + separatorStringLength;
	}
	
	return concatenatedString;
}