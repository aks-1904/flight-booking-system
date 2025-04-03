#ifndef AUTH_H
#define AUTH_H

#define ADMIN_USERNAME "aks__y19"
#define ADMIN_PASSWORD "123456789"

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
Response registerUser(User user);
Response loginUser(User user);

#endif