#ifndef UTILS_H
#define UTILS_H

bool isSame(char *str1, char *str2);  // Check if both string are same or not
int len(char *str);                   // Returns length of string (null character not included)
void toLowerCase(char *str);          // Convert all uppercase letter of string to lower case
int strToInt(char *str);              // Convert a string to integer
bool checkValidInt(char *str);        // Takes a string and check if all characters contains only integer or not
bool isValidDate(char *date);         // Check if the given string is valid date or not
bool isValidTime(char *time);         // Check if the given string is valid time or not
bool isLeapYear(int year);            // Check for a leap year
void copyStr(char *str1, char *str2); // Copy string 2 to string 1
void trimNewline(char *str);          // Remove new line character if present

#endif