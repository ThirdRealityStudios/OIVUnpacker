#include <stdio.h>
#include <stdlib.h>
#include "../list.h"
#include "../stringHelper.h"
#include <assert.h>

void testCreateEmptyList()
{
	puts("testCreateEmptyList()");
	
	List *list = createEmptyList();
	
	assert(list != NULL);
	
	assert(list -> tail == NULL);
	assert(list -> head == NULL);
	
	assert(list -> size == 0);
	
	free(list);
	
	puts("OK");
}

void testClearList()
{
	puts("testClearList()");
	
	List *list = createEmptyList();
	
	clearList(list);
	assert(sizeOf(list) == 0);
	
	appendValue(list, "A");
	
	clearList(list);
	assert(sizeOf(list) == 0);
	
	appendValue(list, "A");
	appendValue(list, "B");
	
	clearList(list);
	assert(sizeOf(list) == 0);
	
	appendValue(list, "A");
	appendValue(list, "B");
	appendValue(list, "C");
	
	clearList(list);
	assert(sizeOf(list) == 0);
	
	free(list);
	
	puts("OK");
}

void testGetNode()
{
	puts("testGetNode()");
	
	List *list = createEmptyList();
	
	appendValue(list, "A");
	assert(list -> size == 1);
	assert(list -> tail != NULL);
	assert(list -> tail -> value != NULL);
	assert(list -> tail -> value[0] == 'A');

	Node *nodeA = getNode(list, 0);

	assert(nodeA != NULL);

	assert(nodeA -> value[0] == 'A');

	free(list -> tail);
	free(list);
	
	puts("OK");
}

void testInsertValue()
{
	puts("testInsertValue()");
	
	List *list = createEmptyList();
	
	appendValue(list, "A");
	
	insertValue(list, 0, "B");
	
	assert(getValue(list, 0)[0] == 'B');
	assert(getValue(list, 1)[0] == 'A');
	
	insertValue(list, 1, "C");
	
	assert(getValue(list, 0)[0] == 'B');
	assert(getValue(list, 1)[0] == 'C');
	assert(getValue(list, 2)[0] == 'A');
	
	insertValue(list, 2, "D");
	
	assert(getValue(list, 0)[0] == 'B');
	assert(getValue(list, 1)[0] == 'C');
	assert(getValue(list, 2)[0] == 'D');
	assert(getValue(list, 3)[0] == 'A');
	
	clearList(list);
	
	free(list);
	
	puts("OK");
}

void testAppendValue()
{
	puts("testAppendValue()");
	
	List *list = createEmptyList();
	
	assert(sizeOf(list) == 0);

	appendValue(list, "A");

	assert(sizeOf(list) == 1);

	assert(getValue(list, 0)[0] == 'A');

	appendValue(list, "B");
	assert(sizeOf(list) == 2);
	assert(getValue(list, 0)[0] == 'A');
	assert(getValue(list, 1)[0] == 'B');
	
	appendValue(list, "C");
	assert(sizeOf(list) == 3);
	assert(getValue(list, 0)[0] == 'A');
	assert(getValue(list, 1)[0] == 'B');
	assert(getValue(list, 2)[0] == 'C');
	
	clearList(list);
	free(list);
	
	puts("OK");
}

void testGetValue()
{
	puts("testGetValue()");
	
	List *list = createEmptyList();
	
	appendValue(list, "A");
	assert(list -> tail -> value[0] == 'A');
	
	appendValue(list, "B");
	assert(list -> tail -> value[0] == 'A');
	assert(list -> head -> value[0] == 'B');
	assert(list -> tail -> value[0] == getValue(list, 0)[0]);
	assert(list -> head -> value[0] == getValue(list, 1)[0]);
	assert(list -> tail -> next -> value[0] == 'B');
	
	appendValue(list, "C");
	assert(list -> tail -> value[0] == 'A');
	assert(list -> tail -> next -> value[0] == 'B');
	assert(list -> tail -> next -> next -> value[0] == 'C');
	assert(list -> head -> value[0] == 'C');
	
	clearList(list);
	free(list);
	
	puts("OK");
}

void testReplaceValue()
{
	puts("testReplaceValue()");
	
	List *list = createEmptyList();
	
	appendValue(list, "A");
	
	replaceValue(list, 0, "B");
	assert(getValue(list, 0)[0] == 'B');
	
	appendValue(list, "A");
	
	replaceValue(list, 1, "C");
	assert(getValue(list, 0)[0] == 'B');
	assert(getValue(list, 1)[0] == 'C');
	
	appendValue(list, "A");
	
	replaceValue(list, 0, "X");
	assert(getValue(list, 0)[0] == 'X');
	assert(getValue(list, 1)[0] == 'C');
	assert(getValue(list, 2)[0] == 'A');
	
	clearList(list);
	free(list);
	
	puts("OK");
}

void testRemoveValue()
{
	puts("testRemoveValue()");
	
	List *list = createEmptyList();
	
	appendValue(list, "A"); // => {"A"}
	assert(list -> size == 1);
	assert(list -> tail -> value[0] == 'A');
	assert(list -> head -> value[0] == 'A');
	assert(list -> head == list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> head -> before == NULL);
	assert(list -> tail -> next == NULL);
	assert(list -> head -> next == NULL);
	
	removeValue(list, 0); // => {} (empty list)
	assert(list -> size == 0);
	assert(list -> head == NULL);
	assert(list -> tail == NULL);
	
	appendValue(list, "A"); // => {"A"}
	assert(list -> tail -> value[0] == 'A');
	assert(list -> head -> value[0] == 'A');
	assert(list -> head == list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> head -> before == NULL);
	assert(list -> tail -> next == NULL);
	assert(list -> head -> next == NULL);
	
	appendValue(list, "B"); // => {"A","B"}
	assert(list -> tail -> value[0] == 'A');
	assert(list -> head -> value[0] == 'B');
	assert(list -> head != list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> head -> before == list -> tail);
	assert(list -> tail -> next == list -> head);
	assert(list -> head -> next == NULL);
	
	removeValue(list, 0); // => {"B"}
	assert(list -> size == 1);
	assert(list -> tail -> value[0] == 'B');
	assert(list -> head -> value[0] == 'B');
	assert(list -> head == list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> head -> before == NULL);
	assert(list -> tail -> next == NULL);
	assert(list -> head -> next == NULL);
	
	removeValue(list, 0);  // => {} (empty list)
	assert(list -> size == 0);
	
	appendValue(list, "A"); // => {"A"}
	assert(list -> tail -> value[0] == 'A');
	assert(list -> head -> value[0] == 'A');
	assert(list -> head == list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> head -> before == NULL);
	assert(list -> tail -> next == NULL);
	assert(list -> head -> next == NULL);
	
	appendValue(list, "B"); // => {"A","B"}
	assert(list -> tail -> value[0] == 'A');
	assert(list -> head -> value[0] == 'B');
	assert(list -> head != list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> head -> before == list -> tail);
	assert(list -> tail -> next == list -> head);
	assert(list -> head -> next == NULL);
	
	appendValue(list, "C"); // => {"A","B","C"}
	assert(list -> tail != NULL);
	assert(list -> tail -> next != NULL);
	assert(list -> tail -> next -> next != NULL);
	assert(list -> head != NULL);
	assert(list -> tail -> next -> next == list -> head);
	assert(list -> tail -> value[0] == 'A');
	assert(list -> tail -> next -> value[0] == 'B');
	assert(list -> head -> value[0] == 'C');
	assert(list -> tail -> next -> next -> value[0] == 'C');
	assert(list -> head != list -> tail);
	assert(list -> tail -> before == NULL);
	assert(list -> tail -> next -> before == list -> tail);
	assert(list -> head -> before == list -> tail -> next);
	assert(list -> head -> next == NULL);
	
	appendValue(list, "D"); // => {"A","B","C","D"}
	
	removeValue(list, 3); // => {"A", "B", "C"}
	assert(list -> size == 3);
	assert(getValue(list, 0)[0] == 'A');
	assert(getValue(list, 1)[0] == 'B');
	assert(getValue(list, 2)[0] == 'C');
	
	removeValue(list, 2); // => {"A", "B"}
	assert(list -> size == 2);
	assert(getValue(list, 0)[0] == 'A');
	assert(getValue(list, 1)[0] == 'B');
	
	removeValue(list, 1); // => {"A"}
	assert(list -> size == 1);
	assert(getValue(list, 0)[0] == 'A');
	
	appendValue(list, "D"); // => {"A", "D"}
	
	removeValue(list, 0); // => {"D"}
	assert(list -> size == 1);
	assert(getValue(list, 0)[0] == 'D');
	
	removeValue(list, 0); // => {} (empty list)
	assert(list -> size == 0);
	
	appendValue(list, "A");
	appendValue(list, "B");
	appendValue(list, "C");
	appendValue(list, "D");
	assert(list -> size == 4);
	
	removeValue(list, 2);
	assert(list -> size == 3);
	
	removeValue(list, 1);
	assert(list -> size == 2);
	
	removeValue(list, 1);
	assert(list -> size == 1);
	
	removeValue(list, 0);
	assert(list -> size == 0);
	
	free(list);
	
	puts("OK");
}

void testSizeOf()
{
	puts("testSizeOf()");
	
	List *list = createEmptyList();
	
	assert(sizeOf(list) == 0);
	
	appendValue(list, "A");
	assert(sizeOf(list) == 1);
	
	appendValue(list, "A");
	assert(sizeOf(list) == 2);
	
	appendValue(list, "A");
	assert(sizeOf(list) == 3);
	
	removeValue(list, 0);
	assert(sizeOf(list) == 2);
	
	removeValue(list, 0);
	assert(sizeOf(list) == 1);
	
	removeValue(list, 0);
	assert(sizeOf(list) == 0);
	
	appendValue(list, "A");
	assert(sizeOf(list) == 1);
	
	appendValue(list, "A");
	assert(sizeOf(list) == 2);
	
	appendValue(list, "A");
	assert(sizeOf(list) == 3);
	
	removeValue(list, 0);
	assert(sizeOf(list) == 2);
	
	removeValue(list, 0);
	assert(sizeOf(list) == 1);
	
	removeValue(list, 0);
	assert(sizeOf(list) == 0);
	
	clearList(list);
	free(list);
	
	puts("OK");
}

void testContainsValue()
{
	// int containsValue(List *list, char *value)
	
	puts("testContainsValue()");
	
	List *list = createEmptyList(); // => {} (empty list)
	
	assert(containsValue(list, "x") == 0);
	assert(containsValue(list, "Z") == 0);
	assert(containsValue(list, "-12312 asd") == 0);
	
	appendValue(list, "x"); // => {"x"}
	assert(containsValue(list, "x") == 1);
	assert(containsValue(list, "Z") == 0);
	assert(containsValue(list, "-12312 asd") == 0);
	
	appendValue(list, "Z"); // => {"x", "Z"}
	assert(containsValue(list, "x") == 1);
	assert(containsValue(list, "Z") == 1);
	assert(containsValue(list, "-12312 asd") == 0);
	
	appendValue(list, "asd"); // => {"x", "Z", "asd"}
	assert(containsValue(list, "x") == 1);
	assert(containsValue(list, "Z") == 1);
	assert(containsValue(list, "asd") == 1);
	assert(containsValue(list, "-12312 asd") == 0);
	
	appendValue(list, "-12312 asd"); // => {"x", "Z", "asd", "-12312 asd"}
	assert(containsValue(list, "x") == 1);
	assert(containsValue(list, "Z") == 1);
	assert(containsValue(list, "asd") == 1);
	assert(containsValue(list, "-12312 asd") == 1);
	
	removeValue(list, 1); // => {"x", "asd", "-12312 asd"}
	assert(containsValue(list, "x") == 1);
	assert(containsValue(list, "Z") == 0);
	assert(containsValue(list, "asd") == 1);
	assert(containsValue(list, "-12312 asd") == 1);

	removeValue(list, 1); // => {"x", "-12312 asd"}
	assert(containsValue(list, "x") == 1);
	assert(containsValue(list, "Z") == 0);
	assert(containsValue(list, "asd") == 0);
	assert(containsValue(list, "-12312 asd") == 1);

	removeValue(list, 0); // => {"-12312 asd"}
	assert(containsValue(list, "x") == 0);
	assert(containsValue(list, "Z") == 0);
	assert(containsValue(list, "asd") == 0);
	assert(containsValue(list, "-12312 asd") == 1);

	// removeValue({"-12312 asd"}, 0);
	removeValue(list, 0); // => {} (empty list)
	assert(containsValue(list, "x") == 0);
	assert(containsValue(list, "Z") == 0);
	assert(containsValue(list, "asd") == 0);
	assert(containsValue(list, "-12312 asd") == 0);

	free(list);

	puts("OK");
}

void testConcatenateValues()
{
	puts("testConcatenateValues()");

	List *list = createEmptyList();
	
	char *concatenatedString = NULL;
	
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, ""));
	free(concatenatedString);
	
	appendValue(list, "A");
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, "A"));
	free(concatenatedString);
	
	appendValue(list, "ttention");
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, "Attention"));
	free(concatenatedString);
	
	appendValue(list, "!");
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, "Attention!"));
	free(concatenatedString);
	
	// Replacing once here the exclamation mark with a question mark
	replaceValue(list, 2, "?");
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, "Attention?"));
	free(concatenatedString);
	
	removeValue(list, 1);
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, "A?"));
	free(concatenatedString);
	
	removeValue(list, 0);
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, "?"));
	free(concatenatedString);
	
	removeValue(list, 0);
	concatenatedString = concatenateValues(list, "");
	assert(equals(concatenatedString, ""));
	free(concatenatedString);
	
	clearList(list);
	free(list);
	
	puts("OK");
}

void testList()
{
	puts("Testing modules of list..");
	puts("");
	
	testGetNode();
	puts("");
	
	testGetValue();
	puts("");
	
	testAppendValue();
	puts("");
	
	testSizeOf();
	puts("");
	
	testInsertValue();
	puts("");
	
	testReplaceValue();
	puts("");
	
	testRemoveValue();
	puts("");
	
	testContainsValue();
	puts("");
	
	testConcatenateValues();
	puts("");
	
	puts("Finished testing.");
}