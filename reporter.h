#ifndef PROJECT_OIV_REPORTER_H
#define PROJECT_OIV_REPORTER_H

/// @brief This function will exit your whole program if "ptrValue" is NULL.
///		   Additionally, it can print a brief error message on where, who and why,
///		   required filling the parameters.
/// @param ptrValue Value of the pointer which MUST NOT be NULL.
/// @param ptrIdentifier The name / identifier of your pointer which may not be NULL.
/// @param fncIdentifier The name of the function where NULL error was caused in (!).
/// @param causedBy E.g. the name of the last function which might have caused the NULL value.
///		   			Simply provide NULL for this param if you don't know anything.
void reportIfNULL(void *ptrValue, char *ptrIdentifier, char *fncIdentifier, char *causedBy);

#endif