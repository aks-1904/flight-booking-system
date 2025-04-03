#include <stdbool.h>
#include "../Include/utils.h"

int len(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
        ;

    return i;
}

bool isSame(char *str1, char *str2)
{
    int len1 = len(str1), len2 = len(str2);

    if (len1 != len2)
        return false;

    for (int i = 0; i < len1; i++)
        if (str1[i] != str2[i])
            return false;

    return true;
}

void toLowerCase(char *str)
{
    int length = len(str);

    for (int i = 0; i < length; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - 'A' + 'a';
}