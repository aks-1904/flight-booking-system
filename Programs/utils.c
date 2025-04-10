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

int strToInt(char *str)
{
    int length = len(str), ans = 0;

    for (int i = 0; i < length; i++)
        ans = (ans * 10) + (str[i] - '0');

    return ans;
}

bool checkValidInt(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        if (!(str[i] >= '0' && str[i] <= '9'))
            return false;

    return true;
}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool isValidDate(char *date)
{
    if (len(date) != 10 || date[4] != '-' || date[7] != '-')
        return false;

    char yearStr[5], monthStr[3], dayStr[3];
    for (int i = 0; i < 4; i++)
        yearStr[i] = date[i];
    yearStr[4] = '\0';
    for (int i = 0; i < 2; i++)
        monthStr[i] = date[i + 5];
    monthStr[3] = '\0';
    for (int i = 0; i < 2; i++)
        dayStr[i] = date[i + 8];
    dayStr[2] = '\0';

    if (!checkValidInt(yearStr) || !checkValidInt(monthStr) || !checkValidInt(dayStr))
        return false;

    int year = strToInt(yearStr), month = strToInt(monthStr), day = strToInt(dayStr);

    if (month < 1 || month > 12)
        return false;

    int daysInMonth[] = {31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day < 1 || day > daysInMonth[month - 1])
        return false;

    return true;
}

bool isValidTime(char *time)
{
    if (len(time) != 5 || time[2] != ':')
        return false;

    char hourStr[3], minuteStr[3];
    for (int i = 0; i < 2; i++)
        hourStr[i] = time[i];
    hourStr[2] = '\0';
    for (int i = 0; i < 2; i++)
        minuteStr[i] = time[i + 3];
    minuteStr[2] = '\0';

    if (!checkValidInt(hourStr) || !checkValidInt(minuteStr))
        return false;

    int hour = strToInt(hourStr);
    int minute = strToInt(minuteStr);

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return false;

    return true;
}

void copyStr(char *str1, char *str2)
{
    for (int i = 0; str2[i] != '\0'; i++)
        str1[i] = str2[i];

    str1[len(str2)] = '\0';
}

void trimNewline(char *str)
{
    size_t length = len(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

bool checkValidUsername(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            continue;
        else if (str[i] >= 'A' && str[i] <= 'Z')
            continue;
        else if (str[i] >= '0' && str[i] <= '9')
            continue;
        else if (str[i] == '_' || str[i] == '@' || str[i] == '#')
            continue;
        else
            return false;
    }

    return true;
}

int generateRandomId(int size)
{
    int lower = (int)pow(10, size - 1);
    int upper = (int)pow(10, size) - 1;

    return (rand() % (upper - lower + 1)) + lower;
}

char **splitCSVLine(char *line, int length)
{
    char **arr = (char **)malloc(length * sizeof(char *));
    int idx = 0, i = 0;

    while (line[i] != '\n' && line[i] != '\0')
    {
        int start = i, k = 0;
        while (line[i] != ',' && line[i] != '\n' && line[i] != '\0')
            i++;

        int length = i - start;

        arr[idx] = (char *)malloc((length + 1) * sizeof(char));
        for (int j = start; j < i; j++)
            arr[idx][k++] = line[j];

        arr[idx][k] = '\0';
        idx++;

        if (line[i] == ',')
            i++;
    }

    return arr;
}

int compareFlights(const void *a, const void *b)
{
    Flight *f1 = (Flight *)a;
    Flight *f2 = (Flight *)b;

    int date_cmp = strcmp(f1->departure_date, f2->departure_date);
    if (date_cmp == 0)
    {
        return strcmp(f1->departure_time, f2->departure_time);
    }
    return date_cmp;
}

void sortAccDeparture(Flight *flights, int count)
{
    qsort(flights, count, sizeof(Flight), compareFlights);
}

void printFlights(Flight *flights, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("<------------------------------>\n\n");
        printf("Flight No: %d\n Destination: %s\n Date: %s\n Time: %s\n Price: %d\n Seats: %d\n",
               flights[i].flight_number,
               flights[i].destination,
               flights[i].departure_date,
               flights[i].departure_time,
               flights[i].ticket_price,
               flights[i].available_seats);
        printf("\n\n<------------------------------>\n\n");
    }
}

int compareByPrice(const void *a, const void *b)
{
    Flight *f1 = (Flight *)a;
    Flight *f2 = (Flight *)b;

    return f1->ticket_price - f2->ticket_price;
}

void sortAccPrice(Flight *flights, int count)
{
    qsort(flights, count, sizeof(Flight), compareByPrice);
}