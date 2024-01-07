#ifndef PROJECT_OIV_STRINGHELPER_H
#define PROJECT_OIV_STRINGHELPER_H

/// @brief Compares two strings if they have deep-equality.
/// @param str1 First string.
/// @param str2 Second string.
/// @return 1 if both strings are deeply equal. Otherwise 0.
int equals(char *str1, char *str2);

/// @brief Cuts a part off from a string, which results in a new string.
/// @param string The string to cut off a part from.
/// @param fromIndex The index where to start cutting (including, so cutting off symbol at 'fromIndex' yet).
/// @param toIndex The index where to stop cutting (excluding, so after 'toIndex' that stop cutting).
/// @return A cutted string, newly allocated (so make sure to call free() later!).
char* cutString(char *string, int fromIndex, int toIndex);

/// @brief Concatenates the string "source" to the string "destination",
///        which results in a newly allocated string.
/// @param destination String used to append the string "source".
/// @param source String obviously used to append to the string "destination".
/// @return Concatenated string (please free later!) or NULL if one of the parameter are NULL.
char *safeStrCat(char *destination, char *source);

/// @brief Searches for a match for the given pattern in a string and returns the index.
/// @param string String to look for the pattern in.
/// @param pattern Pattern to look for in "string".
/// @param beginSearchIndex As the name suggests, the search will start at the given index (if the index is greater than the length of the string, then the pattern will simply be not found => returns -1)
/// @return Index of the match in "string" or -1 if nothing could be found or one of the parameters was NULL or also if the "beginSearchIndex" is out of bounds.
int indexOf(char *string, char *pattern, int beginSearchIndex);

/// @brief Searches for a match for the given pattern in a string and returns the last index.
/// @param string String to look for the pattern in.
/// @param pattern Pattern to look for in "string".
/// @param beginSearchIndex As the name suggests, the search will start at the given index (if the index is greater than the length of the string, then the pattern will simply be not found => returns -1)
/// @return Last index of the match in "string" or -1 if nothing could be found or one of the parameters was NULL or also if the "beginSearchIndex" is out of bounds.
int lastIndexOf(char *string, char *pattern, int beginSearchIndex);

/// @brief Copies the area of a string between two different patterns (including the patterns).
///        E.g., you have the string "<author><displayName>Some name</displayName></author>"
///        but only want everything between "<displayName>" and "</displayName>".
///        startPattern is then "<displayName>" and endPattern is then "</displayName>".
///        The resulting, returned string is then "<displayName>Some name</displayName>".
/// @param string The string you want to search in.
/// @param startPattern The enclosing pattern to be found first.
/// @param endPattern The closing pattern to be found last.
/// @param beginSearchIndex The index in "string" where to start looking for "startPattern".
/// @return The string between "startPattern" and "endPattern", including both patterns.
///         Otherwise NULL if at least one of the two patterns do not exist or if "beginSearchIndex" is out of bounds (invalid).
///         Make sure to free() the returned char* later if not needed anymore!
/// 		NULL is being returned if "string", "startPattern" or "endPattern" is NULL.
char* copyIncludingBetween(char *string, char *startPattern, char *endPattern, int beginSearchIndex);

/// @brief Copies the area of a string between two different patterns (EXcluding the patterns).
///        E.g., you have the string "<author><displayName>Some name</displayName></author>"
///        but only want everything between "<displayName>" and "</displayName>".
///        startPattern is then "<displayName>" and endPattern is then "</displayName>".
///        The resulting, returned string is then "Some name".
/// @param string The string you want to search in.
/// @param startPattern The enclosing pattern to be found first.
/// @param endPattern The closing pattern to be found last.
/// @param beginSearchIndex The index in "string" where to start looking for "startPattern".
/// @return The string between "startPattern" and "endPattern", EXcluding both patterns.
///         Otherwise NULL if at least one of the two patterns do not exist or if "beginSearchIndex" is out of bounds (invalid).
///         Make sure to free() the returned char* later if not needed anymore!
/// 		NULL is being returned if "string", "startPattern" or "endPattern" is NULL.
char* copyExcludingBetween(char *string, char *startPattern, char *endPattern, int beginSearchIndex);

/// @brief Cuts off / removes all appearances of a word in a string.
/// @param string String to remove all appearances from.
/// @param word Word to remove from a string (all).
/// @return New string without all the words or
/// 		the original given parameter string (also copied by-value)
/// 		if there was no word to cut off (simply not found).
char* cutAllWords(char *string, char *word);

#endif