#ifndef PROJECT_OIV_LIST_H
#define PROJECT_OIV_LIST_H

#include "types.h"

/* 
 * Note: List functions may straight exit the program with a suitable error message
 * 		 if one of the arguments are invalid
 *		 (e.g., to prevent further damages to data due to further program execution)!
 *
 *		 Also, arguments are always treated as call-by-value.
 */

/// @brief Creates a Node object (newly allocated).
/// @return Node object.
Node* createNode();

/// @brief Creates an empty list for you (newly allocated).
/// @return List object.
List* createEmptyList();

/// @brief Returns the size of a list.
/// @param list The list to return the size from.
/// @return Size of the list.
int sizeOf(List *list);

/// @brief Clears and frees all elements of a list,
/// 	   whereas the list object itself remains alive.
///		   In the end, the list will have a size of 0 again
///        (ready to be filled up again with new values).
/// @param list The list to clear all values of.
void clearList(List *list);

/// @brief Gets a node at a given index in a list.
/// @param list List to get the node from.
/// @param index The index used to get the node.
/// @return Node at the given index.
Node* getNode(List *list, int index);

/// @brief Inserts a value at a specific position,
///        whereas all other elements are being shifted to the right.
///        That means, if you have e.g. the list {"A","C","D"} and execute
///        insert(list, 1, "B"), the list is then {"A","B","C","D"}.
/// @param list The list to insert the value.
/// @param index The place where to insert the value exactly.
/// @param value The value to insert (your value will be copied to realize at least partial encapsulation for the list).
void insertValue(List *list, int index, char *value);

/// @brief Used to append a value at the end (head) of the list.
///        This is the preferred function if you want to add an element
///		   to an empty list.
///        Example use: you execute append(list, "A") for {} (empty list)
///        and will receive {"A"}.
///        Additionally, if you execute append(list, "B"), you will get {"A","B"}.
/// @param list The list to append the value.
/// @param value The value to append (your value will be copied to realize at least partial encapsulation for the list).
void appendValue(List *list, char *value);

/// @brief Returns the value of a list at a given index.
/// @param list The list to look for the value.
/// @param index The index to retrieve and return the value from.
/// @return The value at the given index
///         (only a copied version from list will be returned to protect original values from changes).
///         Make sure you free the returned value later!
char* getValue(List *list, int index);

/// @brief Replaces a value at a given index with a new one in a list.
/// @param list The list where to look in for the value to replace it.
/// @param index Index of replacement.
/// @param newValue Value used to replace the old one at "index" (your value will be copied to realize at least partial encapsulation for the list).
void replaceValue(List *list, int index, char *newValue);

/// @brief Removes a value at a given index.
/// @param list The list where to remove the value from.
/// @param index The remove-index.
void removeValue(List *list, int index);

/// @brief Prints a list on the console.
/// @param list The list to print on the console.
void printList(List *list);

/// @brief Tells you if there is an element with the specified value in the list.
/// @param list List to look in for the value.
/// @param value The value to check if it is contained in the list.
/// @return 1 if it is contained, otherwise 0.
int containsValue(List *list, char *value);

/// @brief Concatenates every value in the list as a string.
/// @brief separatorString Will be added after each string of the list.
///        Possibilities are line-breaks ("\n") or you just pass an empty string here
///        if you don't like to use this feature.
/// @return Concatenated string of every value (remember to free the returned string later!).
char* concatenateValues(List *list, char *separatorString);

#endif