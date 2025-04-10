#include <stdio.h>
#include <stdbool.h>
#include "../Include/flight.h"

FlightResponse addFlight(Flight *flight)
{
    FlightResponse res;
    FILE *file = fopen(FLIGHT_FILE, "a"); // Open in append mode
    if (file == NULL)
    {
        res.success = false;
        copyStr(res.message, "Cannot complete your request, please try again later");
        return res;
    }

    // Write the flight data to the file
    fputc('\n', file);
    fprintf(file, "%d,%s,%s,%s,%d,%d,%d",
            flight->flight_number,
            flight->destination,
            flight->departure_date,
            flight->departure_time,
            flight->ticket_price,
            flight->total_seats,
            flight->available_seats);

    res.success = true;
    copyStr(res.message, "Flight has been created successfully");
    res.flight = *flight;

    fclose(file);

    return res;
}

Flight *displayFlights(char *destination, int *returnSize)
{
    Flight *flights = NULL;
    *returnSize = 0;

    FILE *file = fopen(FLIGHT_FILE, "r");
    if (!file)
    {
        return NULL;
    }

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char **fields = splitCSVLine(buffer, FLIGHT_DATA_LENGTH);

        if (isSame(fields[1], destination))
        {
            Flight *temp = realloc(flights, (*returnSize + 1) * sizeof(Flight));
            if (!temp)
            {
                for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                    free(fields[i]);
                free(fields);
                fclose(file);
                return flights;
            }

            flights = temp;

            flights[*returnSize].flight_number = strToInt(fields[0]);
            copyStr(flights[*returnSize].destination, fields[1]);
            copyStr(flights[*returnSize].departure_date, fields[2]);
            copyStr(flights[*returnSize].departure_time, fields[3]);
            flights[*returnSize].ticket_price = strToInt(fields[4]);
            flights[*returnSize].total_seats = strToInt(fields[5]);
            flights[*returnSize].available_seats = strToInt(fields[6]);

            (*returnSize)++;
        }

        for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    fclose(file);
    return flights;
}
