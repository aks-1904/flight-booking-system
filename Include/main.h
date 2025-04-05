#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "../Programs/utils.c"
#include "../Programs/auth.c"

void mainMenu();
void adminMenu();
void userMenu(User user);

#endif