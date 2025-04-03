#include "./Include/main.h"

void main()
{
    mainMenu();
}

void mainMenu()
{
    printf("<----------Select a option---------->\n");
    printf("1. Login in as admin\n");
    printf("2. Register as a user\n");
    printf("3. Login as user\n");
    printf("4. Quit\n");

    while (true)
    {
        int choice;
        Admin adminData;
        User userData;

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the username: ");
            scanf("%s", adminData.username);
            printf("Enter the password: ");
            scanf("%s", adminData.password);

            bool isAdmin = loginAdmin(adminData);
            if (isAdmin)
                adminMenu();
            else
                printf("Failed to logged in as admin check details, and then continue");
            break;

        default:
            printf("Invalid Option");
            break;
        }
    }
}

void adminMenu()
{
    printf("<----------Logged in as admin (%s)---------->\n", ADMIN_USERNAME);
    printf("1. Add a flight\n");
    printf("2. Remove a flight\n");
    printf("3. Check a flight details\n");
    printf("4. logout\n");
}