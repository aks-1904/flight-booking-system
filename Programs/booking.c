#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../Include/main.h"
#include "../Include/flight.h"
#include "../Include/booking.h"

BookingResponse bookFlight(Booking bookingData)
{
    BookingResponse res;

    FILE *file1 = fopen(BOOKINGS_FILE, "a");
    FILE *file2 = fopen(FLIGHT_FILE, "r");
    FILE *tmpFile = fopen(TEMP_FILE, "w");

    if (!file1 || !file2 || !tmpFile)
    {
        copyStr(res.message, "Cannot complete your request, please try again later");
        res.success = false;

        return res;
    }

    bool got = false;
    int ticketPrice;
    char buffer[255];

    while (fgets(buffer, 255, file2) != NULL)
    {
        char **fields = splitCSVLine(buffer, FLIGHT_DATA_LENGTH);
        if (strToInt(fields[0]) == bookingData.flightId)
        {
            ticketPrice = strToInt(fields[4]);
            got = true;
            if (strToInt(fields[6]) < bookingData.totalSeatsBooked)
            {
                res.success = false;
                copyStr(res.message, "Seats not available in this flight as your requirement");

                for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                    free(fields[i]);

                free(fields);

                fclose(file1);
                fclose(file2);
                fclose(tmpFile);

                return res;
            }
            break;
        }
    }

    fclose(file2);

    if (!got)
    {
        copyStr(res.message, "Couldn't able to find the flight, check the flight number");
        res.success = false;

        fclose(file1);
        fclose(tmpFile);

        return res;
    }

    FILE *flightFile = fopen(FLIGHT_FILE, "r");

    fprintf(file1, "%d,%d,%d,%d,%d\n", bookingData.bookingId, bookingData.userId, bookingData.flightId, bookingData.totalSeatsBooked, ticketPrice * bookingData.totalSeatsBooked);

    while (fgets(buffer, 255, flightFile) != NULL)
    {
        char **fields = splitCSVLine(buffer, FLIGHT_DATA_LENGTH);
        if (strToInt(fields[0]) == bookingData.flightId)
        {
            fprintf(tmpFile, "%s,%s,%s,%s,%s,%s,%d\n", fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], strToInt(fields[6]) - bookingData.totalSeatsBooked);

            for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                free(fields[i]);

            free(fields);
        }
        else
        {
            for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                free(fields[i]);

            free(fields);
            fprintf(tmpFile, "%s", buffer);
        }
    }

    fclose(file1);
    fclose(flightFile);
    fclose(tmpFile);

    remove(FLIGHT_FILE);
    rename(TEMP_FILE, FLIGHT_FILE);

    copyStr(res.message, "Booking Successfull");
    res.success = true;
    res.booking = bookingData;

    return res;
}