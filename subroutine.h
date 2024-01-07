#ifndef PROJECT_OIV_SUBROUTINE_H
#define PROJECT_OIV_SUBROUTINE_H

/// @brief Welcomes the user on the console and sets the window title.
/// @param programTitle Variable reference from main.c after calling init().
/// @param author Variable reference from main.c after calling init().
void welcomeUser(char *programTitle, char *author);

/// @brief Asserts that all necessary files do exist (if not => error message and exit)
/// @param oivArchiveFilepath Variable reference from main.c after calling init().
/// @param assemblyXmlFilepath Variable reference from main.c after calling init().
void assertExistingFiles(char *oivArchiveFilepath, char *assemblyXmlFilepath);

/// @brief Asserts that the necessary .oiv archive exists (if not => error message and exit)
/// @param destination Variable reference from main.c after calling init().
void assertExistingOIVArchive(char *oivArchiveFilepath);

/// @brief Asserts that the necessary assembly.xml exists after unpacking the .oiv archive 
///        (if it does not exist => error message and exit)
/// @param destination Variable reference from main.c after calling init().
void assertExistingAssemblyXml(char *assemblyXmlFilepath);

/// @brief The same as assertExistingDirectories(), but for the main arguments.
/// @param argc Exact argument from main() function (just pass from main() here without changes).
/// @param argv Exact argument from main() function (just pass from main() here without changes).
void assertCorrectMainArgs(int argc, char **argv);

void processArchiveXml(char *assemblyXmlFileContent, char *destinationRealPath);

void processRemainingAddXml(char *assemblyXmlFileContent, char *destinationRealPath);

/// @brief Processes an XML string of the format "<add ... </add>" to create suitable directories
///		   and GTA IV files for the unpacked .oiv modpack.
/// @param addXmlString A string of the format "<add ... </add>" from an assembly.xml.
/// @param destinationRealPath The destination realpath, determined in the main.c file (or simply the target directory of your unpacked modpack).
/// @param optionalArchivePathValue You specifiy the "path" attribute value here of a
///		   "<archive path="..."><add source="...">...</add></archive>" tag.
///		   This helps / enables the possibility to build the target location of your GTA IV files in "destinationRealPath".
/// 	   String concatentation is then applied as follows: path value string + source value string.
///		   If you only want to process a pure <add ... </add> pattern (without being enclosed in a block <archive>...</archive>),
///		   then you can legally pass NULL here (which will not crash the program or something..).
///		   The function will then ignore the parameter "optionalArchivePathValue".
void processAddTagXmlString(char *addXmlString, char *destinationRealPath, char *optionalArchivePathValue);

#endif