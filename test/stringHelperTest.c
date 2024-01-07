#include <assert.h>
#include "../stringHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testEquals()
{
	puts("testEquals()");

	assert(equals(NULL, NULL) == 1);
	assert(equals(NULL, "\0") == 0);
	assert(equals("\0", NULL) == 0);
	assert(equals(NULL, "") == 0);
	assert(equals("", NULL) == 0);
	
	assert(equals("", "") == 1);
	assert(equals("", "A") == 0);
	assert(equals("A", "") == 0);

	assert(equals("A", "A") == 1);
	assert(equals("A", "AB") == 0);
	assert(equals("AB", "A") == 0);
	
	assert(equals("AB", "AB") == 1);
	assert(equals(NULL, "AB") == 0);
	assert(equals("AB", NULL) == 0);
	
	assert(equals("Hello World", NULL) == 0);
	assert(equals(NULL, "Hello World") == 0);
	assert(equals("Hello World", "Hello World") == 1);
	assert(equals("", "Hello World") == 0);
	assert(equals("Hello World", "") == 0);
	assert(equals("\0", "Hello World") == 0);
	assert(equals("Hello World", "\0") == 0);
	
	assert(equals("Hello World", "Hi\nHow are you?") == 0);
	assert(equals("Hi\nHow are you?", "Hello World") == 0);
	assert(equals("Hi\nHow are you?", "Hi\nHow are you?") == 1);
	
	assert(equals("y", "y") == 1);
	assert(equals("y", "yr") == 0);
	assert(equals("yr", "y") == 0);
	
	puts("OK");
}

void testCutString() // TODO hier auch equals(..) einsetzen!
{
	puts("testCutString()");
	
	char *temp = NULL;

	// char* cutString(char *string, int fromIndex, int toIndex)
	
	// Check for NULL as argument.
	temp = cutString(NULL, 0, 0);
	assert(equals(temp, NULL));
	free(temp);
	
	temp = cutString(NULL, 1, 0);
	assert(equals(temp, NULL));
	free(temp);
	
	temp = cutString(NULL, 0, 1);
	assert(equals(temp, NULL));
	free(temp);
	
	temp = cutString(NULL, 1, 1);
	assert(equals(temp, NULL));
	free(temp);
	
	// Check for "" (empty string) as argument.
	temp = cutString("", 0, 0);
	assert(equals(temp, NULL)); // You cannot cut an empty string (no matter what the other arguments are).
	free(temp);			        // Hence, NULL is returned.
	
	temp = cutString("", 1, 0);
	assert(equals(temp, NULL));
	free(temp);
	
	temp = cutString("", 0, 1);
	assert(equals(temp, NULL));
	free(temp);
	
	temp = cutString("", 1, 1);
	assert(equals(temp, NULL));
	free(temp);
	
	// Check for "\0" as argument.
	temp = cutString("\0", 0, 0);
	assert(equals(temp, NULL)); // The argument "\0" will be detected as an empty string,
	free(temp);			        // because it works internally with NULL-terminated strings.
								// Hence, NULL is returned.
	
	temp = cutString("\0", 1, 0); // Same reason like in the last test but also here:
	assert(equals(temp, NULL));   // NULL because fromIndex > toIndex.
	free(temp);
	
	temp = cutString("\0", 0, 1); // Same reason like in the last test but also here:
	assert(equals(temp, NULL));   // NULL because toIndex is out of bounds.
	free(temp);
	
	temp = cutString("\0", 1, 1); // Same reason like in the last test but also here:
	assert(equals(temp, NULL));   // NULL because both (!) fromIndex and toIndex are out of bounds.
	free(temp);
	
	temp = cutString("\0", -1, 0); // Same reason like in the last test but also here:
	assert(equals(temp, NULL));    // NULL because fromIndex is negative.
	free(temp);
	
	temp = cutString("\0", 0, -1); // Same reason like in the last test but also here:
	assert(equals(temp, NULL));   // NULL because toIndex is smaller than fromIndex AND toIndex is negative.
	free(temp);
	
	// Check for "A" as argument.
	temp = cutString("A", 0, 0); // You cut off 'A' in this case which results
	assert(equals(temp, ""));    // in an empty string.
	free(temp);
	
	temp = cutString("A", 1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex > toIndex.
	free(temp);
	
	temp = cutString("A", 0, 1);
	assert(equals(temp, NULL)); // NULL because toIndex is out of bounds.
	free(temp);
	
	temp = cutString("A", 1, 1);
	assert(equals(temp, NULL)); // NULL because both (!) fromIndex and toIndex are out of bounds.
	free(temp);
	
	temp = cutString("A", -1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex is negative.
	free(temp);
	
	temp = cutString("A", 0, -1);
	assert(equals(temp, NULL)); // NULL because toIndex is smaller than fromIndex AND toIndex is negative.
	free(temp);
	
	
	// Check for "AB" as argument.
	temp = cutString("AB", 0, 0); // You cut off 'A' in this case which results
	assert(equals(temp, "B"));    // in "B" only.
	free(temp);
	
	temp = cutString("AB", 1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex > toIndex.
	free(temp);
	
	temp = cutString("AB", 0, 1); // You cut off 'A' and 'B'
	assert(equals(temp, ""));     // which results in an empty string.
	free(temp);
	
	temp = cutString("AB", 1, 1); // You cut off 'B' which results in "A" only.
	assert(equals(temp, "A"));
	free(temp);
	
	temp = cutString("AB", -1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex is negative.
	free(temp);
	
	temp = cutString("AB", 0, -1);
	assert(equals(temp, NULL)); // NULL because toIndex is smaller than fromIndex AND toIndex is negative.
	free(temp);
	
	// Check for "ABC" as argument.
	temp = cutString("ABC", 0, 0); // You cut off 'A' in this case which results
	assert(equals(temp, "BC"));    // in "BC" only.
	free(temp);
	
	temp = cutString("ABC", 2, 2); // You cut off 'C' in this case which results
	assert(equals(temp, "AB"));    // in "AB" only.
	free(temp);
	
	temp = cutString("ABC", 1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex > toIndex.
	free(temp);
	
	temp = cutString("ABC", 0, 1); // You cut off 'A' and 'B'
	assert(equals(temp, "C"));     // which results in "C" only.
	free(temp);
	
	temp = cutString("ABC", 1, 2); // You cut off 'B' and 'C' in this case which results
	assert(equals(temp, "A"));    // in "A" only.
	free(temp);
	
	temp = cutString("ABC", 0, 2); // You cut off 'A', 'B' and 'C' in this case which results
	assert(equals(temp, ""));    // in an empty string.
	free(temp);
	
	temp = cutString("ABC", 1, 1); // You cut off 'B' which results in "AC" only.
	assert(equals(temp, "AC"));
	free(temp);
	
	temp = cutString("ABC", -1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex is negative.
	free(temp);
	
	temp = cutString("ABC", 0, -1);
	assert(equals(temp, NULL)); // NULL because toIndex is smaller than fromIndex AND toIndex is negative.
	free(temp);
	
	// Check for "Hello World" as argument.
	temp = cutString("Hello World", 0, 0); // You cut off 'H' in this case which results
	assert(equals(temp, "ello World"));    // in "allo World" only.
	free(temp);
	
	temp = cutString("Hello World", 2, 2); // You cut off 'l' in this case which results
	assert(equals(temp, "Helo World"));   // in "Helo World" only.
	free(temp);
	
	temp = cutString("Hello World", 1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex > toIndex.
	free(temp);
	
	temp = cutString("Hello World", 0, 1); // You cut off 'H and 'a'
	assert(equals(temp, "llo World"));     // which results in "llo World" only.
	free(temp);
	
	temp = cutString("Hello World", 1, 2); // You cut off 'e' and 'l' in this case which results
	assert(equals(temp, "Hlo World"));     // in "Hlo World" only.
	free(temp);
	
	temp = cutString("Hello World", 0, 2); // You cut off 'H', 'e' and 'l' in this case which results
	assert(equals(temp, "lo World"));      // in "lo World".
	free(temp);
	
	temp = cutString("Hello World", 1, 1); // You cut off 'a' which results in
	assert(equals(temp, "Hllo World"));	   // "Hllo World" only.
	free(temp);
	
	temp = cutString("Hello World", 1, 9); // You cut off everything (including) between 'e' and last 'l' which results in
	assert(equals(temp, "Hd"));	           // "Hd" only.
	free(temp);
	
	temp = cutString("Hello World", 0, 9); // You cut off everything (including) between 'H' and last 'l' which results in
	assert(equals(temp, "d"));	           // "d" only.
	free(temp);
	
	temp = cutString("Hello World", 1, 10); // You cut off everything (including) between 'e' and 'd' which results in
	assert(equals(temp, "H"));	            // "H" only.
	free(temp);
	
	temp = cutString("Hello World", 0, 5); // You cut off everything (including) between 'H' and ' ' which results in
	assert(equals(temp, "World"));	       // "World" only.
	free(temp);
	
	temp = cutString("Hello World", 5, 10); // You cut off everything (including) between ' ' and 'd' which results in
	assert(equals(temp, "Hello"));	        // "Hello" only.
	free(temp);
	
	temp = cutString("Hello World", -1, 0);
	assert(equals(temp, NULL)); // NULL because fromIndex is negative.
	free(temp);
	
	temp = cutString("Hello World", 0, -1);
	assert(equals(temp, NULL)); // NULL because toIndex is smaller than fromIndex AND toIndex is negative.
	free(temp);
	
	// Check for "Hi,\nhow are you?" as argument because it also contains the line-break as special character.
	temp = cutString("Hi,\nhow are you?", 2, 3); // You cut off ',' and '\n' in this case which results
	assert(equals(temp, "Hihow are you?"));      // in "Hihow are you?" only
	free(temp);									 // Hence, you can also remove line-breaks.
	
	puts("OK");
}

void testSafeStrCat()
{
	puts("testSafeStrCat()");
	
	char *temp;
	
	// Concatenate the value NULL to NULL, which must result in NULL for temp.
	temp = safeStrCat(NULL, NULL);
	assert(equals(temp, NULL));
	free(temp);
	
	// Concatenate the value NULL to "" (empty string), which must result in NULL for temp.
	temp = safeStrCat("", NULL);
	assert(equals(temp, NULL));
	free(temp);
	
	// Concatenate the value "" (empty string) to NULL, which must result in NULL for temp.
	temp = safeStrCat(NULL, "");
	assert(equals(temp, NULL));
	free(temp);
	
	// Concatenate the value "" (empty string) again to an empty string "",
	// which must result in an empty string (nothing changed).
	temp = safeStrCat("", "");
	assert(equals(temp, ""));
	free(temp);
	
	// Concatenate the value "" (empty string) to "A",
	// which results in "A" again.
	temp = safeStrCat("A", "");
	assert(equals(temp, "A"));
	free(temp);
	
	// TODO safeStrCat() crashes the program before message "OK".
	
	puts("OK");
}

void testIndexOf()
{
	puts("testIndexOf()");
	
	// int indexOf(char *string, char *pattern, int beginSearchIndex);
	
	assert(indexOf("Hello world,\nthis is some text.\n", NULL, 0) == -1); // You pass NULL for the "pattern" => returns -1
	assert(indexOf("Hello world,\nthis is some text.\n", "", 0) == -1);   // Empty string is like '\0' => so indexOf will not find symbol => returns -1

	assert(indexOf(NULL, "Some text", 0) == -1); // You pass NULL for param "string" => returns -1
	
	assert(indexOf("Hello world,\nthis is some text.\n", "H", 0) == 0);
	assert(indexOf("Hello world,\nthis is some text.\n", "H", 0) == 0);
	assert(indexOf("Hello world,\nthis is some text.\n", "H", 0) == 0);
	assert(indexOf("Hello world,\nthis is some text.\n", "He", 0) == 0);
	assert(indexOf("Hello world,\nthis is some text.\n", "Hello", 0) == 0);

	assert(indexOf("Hello world,\nthis is some text.\n", ",", 0) == 11);
	assert(indexOf("Hello world,\nthis is some text.\n", "\n", 0) == 12);
	assert(indexOf("Hello world,\nthis is some text.\n", "t", 0) == 13);
	
	assert(indexOf("Hello world,\nthis is some text.\n", "\n", 12) == 12);
	assert(indexOf("Hello world,\nthis is some text.\n", "\n", 13) == 31);

	assert(indexOf("Hello world,\nthis is some text.\n", "H", 13) == -1);
	assert(indexOf("Hello world,\nthis is some text.\n", "x", 0) == 28);
	assert(indexOf("Hello world,\nthis is some text.\n", "X", 0) == -1);
	assert(indexOf("Hello world,\nthis is some text.\n", "text,", 0) == -1);

	assert(indexOf("Hello world,\nthis is some text.\n", "H", 13) == -1);
	assert(indexOf("Hello world,\nthis is some text.\n", "H", 13) == -1);
	
	puts("OK");
}

void testCopyIncludingBetween()
{
	puts("testCopyIncludingBetween()");
	
	// char* copyIncludingBetween(char *string, char *startPattern, char *endPattern, int beginSearchIndex)
	
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , NULL, NULL, 0), NULL));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "", NULL, 0), NULL));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , NULL, "", 0), NULL));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , NULL, NULL, -1), NULL));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "", "", 0), NULL));
	
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "H", "", 0), NULL));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "", "H", 0), NULL));
	
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "H", ".", 0), "Hello world,\nthis is some text."));
	
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "H", ".", 1), NULL));
	
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "Hello", "world", 0), "Hello world"));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "world", "this", 0), "world,\nthis"));
	assert(equals(copyIncludingBetween("Hello world,\nthis is some text.\n" , "text", "\n", 0), "text.\n"));
	
	puts("OK");
}

void testCopyExcludingBetween()
{
	puts("testCopyExcludingBetween()");
	
	// char* copyExcludingBetween(char *string, char *startPattern, char *endPattern, int beginSearchIndex)
	
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , NULL, NULL, 0), NULL));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "", NULL, 0), NULL));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , NULL, "", 0), NULL));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , NULL, NULL, -1), NULL));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "", "", 0), NULL));
	
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "H", "", 0), NULL));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "", "H", 0), NULL));
	
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "H", ".", 0), "ello world,\nthis is some text"));
	
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "H", ".", 1), NULL));
	
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "Hello", "world", 0), " "));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "world", "this", 0), ",\n"));
	assert(equals(copyExcludingBetween("Hello world,\nthis is some text.\n" , "text", "\n", 0), "."));
	
	puts("OK");
}

void testCutAllWords()
{
	puts("testCutAllWords()");
	
	char *temp = cutAllWords("", "");
	assert(equals(temp, ""));
	free(temp);
	
	temp = cutAllWords("A", "");
	assert(equals(temp, "A"));
	free(temp);
	
	temp = cutAllWords("A", "A");
	assert(equals(temp, ""));
	free(temp);
	
	temp = cutAllWords("AA", "A");
	assert(equals(temp, ""));
	free(temp);
	
	temp = cutAllWords("A A", "A");
	assert(equals(temp, " "));
	free(temp);
	
	temp = cutAllWords("Hello World", "l");
	assert(equals(temp, "Heo Word"));
	free(temp);
	
	temp = cutAllWords("C:\\\\", "\\");
	assert(equals(temp, "C:"));
	free(temp);
	
	temp = cutAllWords("\\C:\\testLOL \\nice", "\\");
	assert(equals(temp, "C:testLOL nice"));
	free(temp);
	
	puts("OK");
}

void testStringHelper()
{
	puts("Testing modules of stringHelper..");
	puts("");
	
	testEquals();
	puts("");
	
	testCutString();
	puts("");
	
	testSafeStrCat();
	puts("");
	
	testIndexOf();
	puts("");
	
	testCopyIncludingBetween();
	puts("");
	
	testCopyExcludingBetween();
	puts("");
	
	testCutAllWords();
	puts("");
	
	puts("Finished testing.");
}