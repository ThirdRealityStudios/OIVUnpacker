#ifndef PROJECT_OIV_FILEHELPER_H
#define PROJECT_OIV_FILEHELPER_H

#include "types.h"

/// @brief Reads all bytes of all file and returns it as a complete string.
///		   (simply like opening notepad.exe and seeing what's inside the file).
///	@param filepath The relative (to the .exe) path or absolute path to the file.
/// @param exceptSymbols A list of symbols to ignore
///						 (not to include in the returned string)
/// 					 while reading the content of the file.
///        			     If you do not want "exceptSymbols", then pass an empty list here.
/// @return The string, contained by the given file.
char* readFile(char *filepath, List *exceptSymbols);

#endif