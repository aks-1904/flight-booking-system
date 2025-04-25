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

bool cancelFlight(int flight_number)
{
    FILE *flightFile = fopen(FLIGHT_FILE, "r");
    FILE *bookingsFile = fopen(BOOKINGS_FILE, "r");
    FILE *tempFlightFile = fopen(TEMP_FLIGHT_FILE, "w");
    FILE *tempBookingFile = fopen(TEMP_BOOKING_FILE, "w");

    int found = false;

    if (!flightFile || !bookingsFile || !tempBookingFile || !tempFlightFile)
        return false;

    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), flightFile) != NULL)
    {
        char **fields = splitCSVLine(buffer, FLIGHT_DATA_LENGTH);

        if (strToInt(fields[0]) == flight_number)
        {
            found = true;
        }
        else
        {
            fputs(buffer, tempFlightFile);
        }

        for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    while (fgets(buffer, sizeof(buffer), bookingsFile) != NULL)
    {
        char **fields = splitCSVLine(buffer, BOOKING_DATA_LENGTH);

        if (strToInt(fields[2]) != flight_number)
            fputs(buffer, tempBookingFile);

        for (int i = 0; i < BOOKING_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    fclose(flightFile);
    fclose(bookingsFile);
    fclose(tempFlightFile);
    fclose(tempBookingFile);

    if (found)
    {
        rename(TEMP_FLIGHT_FILE, FLIGHT_FILE);
        rename(TEMP_BOOKING_FILE, BOOKINGS_FILE);
    }
    else
    {
        remove(TEMP_FLIGHT_FILE);
        remove(TEMP_BOOKING_FILE);
    }

    return found;
}

void printReport()
{
    FILE *flightFile = fopen(FLIGHT_FILE, "r");
    FILE *bookingFile = fopen(BOOKINGS_FILE, "r");

    if (!flightFile || !bookingFile)
    {
        printf("Some error occuered while generating report");
        return;
    }

    char line[MAX_LINE_LENGTH];

    fgets(line, sizeof(line), flightFile);
    fgets(line, sizeof(line), bookingFile);

    int totalBookings = 0;
    int totalAvailableSeats = 0;
    float totalRevenue = 0.0;

    while (fgets(line, sizeof(line), flightFile))
    {
        char **fields = splitCSVLine(line, FLIGHT_DATA_LENGTH);
        if (fields == NULL)
            continue;
        totalAvailableSeats += strToInt(fields[6]);

        for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    rewind(bookingFile);
    fgets(line, sizeof(line), bookingFile);

    while (fgets(line, sizeof(line), bookingFile))
    {
        char **fields = splitCSVLine(line, BOOKING_DATA_LENGTH);
        if (fields == NULL)
            continue;
        totalBookings += strToInt(fields[3]);
        totalRevenue += strToInt(fields[4]);

        for (int i = 0; i < BOOKING_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    fclose(flightFile);
    fclose(bookingFile);

    printf("===== Flight Booking Report =====\n");
    printf("Total Bookings       : %d\n", totalBookings);
    printf("Total Available Seats: %d\n", totalAvailableSeats);
    printf("Total Revenue (INR)  : %.2f\n", totalRevenue);
    printf("<================================>");
}

FlightResponse findFlight(int flight_number)
{
    FlightResponse res;
    FILE *file = fopen(FLIGHT_FILE, "r");

    if (!file)
    {
        copyStr(res.message, "Cannot complete you request, try again later");
        res.success = false;

        return res;
    }

    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file))
    {
        char **fields = splitCSVLine(buffer, FLIGHT_DATA_LENGTH);

        if (strToInt(fields[0]) == flight_number)
        {
            res.success = true;
            res.flight.flight_number = strToInt(fields[0]);
            copyStr(res.flight.destination, fields[1]);
            copyStr(res.flight.departure_date, fields[2]);
            copyStr(res.flight.departure_time, fields[3]);
            res.flight.ticket_price = strToInt(fields[4]);
            res.flight.total_seats = strToInt(fields[5]);
            res.flight.available_seats = strToInt(fields[6]);

            return res;
        }

        for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    res.success = false;
    copyStr(res.message, "Flight Id didn't match to any existing flight");

    return res;
}