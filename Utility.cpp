#include "Utility.hpp"

bool showGuiWarnings = false; // also show warnings in a gui, use if console is disabled
bool showGuiErrors = false; // also show errors in a gui, use if console is disabled
/// TODO implement showing warnings/errors in gui

/*
 * Minor issues, recoverable without significant loss in functionality.
 */
std::ostream& warn()
{
    return std::cerr << "[WAR] ";
}

/*
 * Major issues, unrecoverable or recovering would result in significant loss in functionality.
 *  Program is expected to quit afterwards, or display an error message, then terminate.
 */
std::ostream& error()
{
    return std::cerr << "[ERR] ";
}

// Only checks for 4 most common types of whitespace, don't go putting weird formatting chars in here
bool isWhiteSpace(const char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/*
 * Strips everything after the first "//" encountered in the string.
 * Then trims leading and trailing whitespace.
 *
 * Used in parsing data files line by line.
 *
 * The passed string is both modified by this function.
 * Return value is if cleaned line should be parsed (has non-zero length).
 */
bool cleanLine(std::string line)
{
    // TODO: check if substr requires freeing the unused remainder of the string
    unsigned int length = line.length();
    //
    unsigned int commentPos = line.find("//");
    if (commentPos != std::string::npos)
    {
        length = commentPos; // consider beginning of comment to be end of string
    }
    //
    unsigned int startPos; // position of first non-whitespace character
    unsigned int endPos; // position of last non-whitespace character
    for (startPos = 0; startPos < length && isWhiteSpace(line[startPos]); ++startPos);
    for (endPos = length - 1; endPos > startPos && isWhiteSpace(line[endPos]); --endPos);
    line = line.substr(startPos, endPos - startPos + 1);
    //
    return line.length() > 0;
}


// printf("Error on line %d of file %s\n", __LINE__, __FILE__);
