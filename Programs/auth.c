#include <stdio.h>
#include <stdbool.h>
#include "../Include/auth.h"

bool loginAdmin(Admin adminData)
{
    toLowerCase(adminData.username);
    if (!isSame(adminData.username, ADMIN_USERNAME))
        return false;

    if (!isSame(adminData.password, ADMIN_PASSWORD))
        return false;

    return true;
}

Response registerUser(User *userData)
{
    Response res;

    FILE *file = fopen(USER_FILE, "a");

    if (!file)
    {
        copyStr(res.message, "Can't complete your request, please try again later");
        res.success = false;
        return res;
    }

    fputc('\n', file);
    fprintf(file, "%d,%s,%s", userData->userId, userData->username, userData->password);

    res.success = true;
    copyStr(res.message, "Registered Successfully");
    res.user = *userData;

    fclose(file);

    return res;
}

bool isUsernameTaken(char *username)
{
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL)
    {
        printf("Cannot complete your request now");
        return true;
    }

    char line[MAX_LINE_LENGTH];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char **data = splitCSVLine(line);
        if (isSame(data[1], username))
        {
            for (int i = 0; i < 3; i++)
                free(data[i]);

            free(data);
            fclose(file);
            return true;
        }
        for (int i = 0; i < 3; i++)
            free(data[i]);

        free(data);
    }

    fclose(file);
    return false;
}