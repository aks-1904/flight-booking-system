#ifndef AUTH_H
#define AUTH_H

#define ADMIN_USERNAME "aks__y19"
#define ADMIN_PASSWORD "123456789"

#define USER_ID_SIZE 8
#define MAX_LINE_LENGTH 256
#define USERNAME_MAX_LENGTH 100
#define USER_FILE "./data/users.csv"

typedef struct{
    char username[50];
    char password[50];
} Admin;

typedef struct{
    int userId;
    char username[50];
    char password[50];
} User;

typedef struct{
    bool success;
    char message[100];
    User user;
} Response;

bool loginAdmin(Admin admin);
Response registerUser(User *user);
Response loginUser(User user);
bool isUsernameTaken(char* username);

#endif