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