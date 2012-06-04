#include <iostream>

/*
 * Minor issues, recoverable without significant loss in functionality.
 */
std::ostream& warn();

/*
 * Major issues, unrecoverable or recovering would result in significant loss in functionality.
 *  Program is expected to quit afterwards, or display an error message, then terminate.
 */
std::ostream& error();

// Only checks for 4 most common types of whitespace, don't go putting weird formatting chars in here
bool isWhiteSpace(const char c);

/*
 * Strips everything after the first "//" encountered in the string.
 * Then trims leading and trailing whitespace.
 *
 * Used in parsing data files line by line.
 *
 * The passed string is both modified by this function.
 * Return value is if cleaned line should be parsed (has non-zero length).
 */
bool cleanLine(std::string line);
