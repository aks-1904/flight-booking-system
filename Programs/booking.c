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

AllBookingResponse *showAllBookings(int userId, int *returnSize)
{
    AllBookingResponse *res = NULL;
    *returnSize = 0;

    FILE *file = fopen(BOOKINGS_FILE, "r");
    FILE *flightFile = fopen(FLIGHT_FILE, "r");

    if (!file || !flightFile)
        return NULL;

    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char **fields = splitCSVLine(buffer, BOOKING_DATA_LENGTH);

        if (strToInt(fields[1]) == userId)
        {
            AllBookingResponse *tmp = realloc(res, (*returnSize + 1) * sizeof(AllBookingResponse));

            if (!tmp)
            {
                for (int i = 0; i < BOOKING_DATA_LENGTH; i++)
                    free(fields[i]);
                free(fields);

                fclose(file);
                fclose(flightFile);
                return res;
            }

            res = tmp;

            res[*returnSize].bookingId = strToInt(fields[0]);
            res[*returnSize].totalFare = strToInt(fields[4]);
            res[*returnSize].totalSeatsBooked = strToInt(fields[3]);

            rewind(flightFile);

            char flightBuffer[MAX_LINE_LENGTH];
            while (fgets(flightBuffer, sizeof(flightBuffer), flightFile) != NULL)
            {
                char **flightFields = splitCSVLine(flightBuffer, FLIGHT_DATA_LENGTH);

                if (strToInt(fields[2]) == strToInt(flightFields[0]))
                {
                    copyStr(res[*returnSize].destination, flightFields[1]);
                    copyStr(res[*returnSize].departure_date, flightFields[2]);
                    copyStr(res[*returnSize].departure_time, flightFields[3]);

                    for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                        free(flightFields[i]);
                    free(flightFields);
                    break;
                }

                for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                    free(flightFields[i]);
                free(flightFields);
            }

            (*returnSize)++;
        }

        for (int i = 0; i < BOOKING_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    fclose(file);
    fclose(flightFile);

    return res;
}

BookingResponse cancelBooking(int bookingId)
{
    BookingResponse res;

    FILE *bookingFile = fopen(BOOKINGS_FILE, "r");
    FILE *flightFile = fopen(FLIGHT_FILE, "r");
    FILE *tmpBookingFile = fopen(TEMP_BOOKING_FILE, "w");
    FILE *tmpFlightFile = fopen(TEMP_FLIGHT_FILE, "w");

    if (!bookingFile || !flightFile || !tmpBookingFile || !tmpFlightFile)
    {
        res.success = false;
        copyStr(res.message, "Can't complete your request. Please try again later.");
        return res;
    }

    char buffer[MAX_LINE_LENGTH];
    bool bookingFound = false;
    int seatsBooked = 0, flightId = 0;

    while (fgets(buffer, sizeof(buffer), bookingFile) != NULL)
    {
        char **fields = splitCSVLine(buffer, BOOKING_DATA_LENGTH);

        if (!bookingFound && strToInt(fields[0]) == bookingId)
        {
            bookingFound = true;

            seatsBooked = strToInt(fields[3]);
            flightId = strToInt(fields[2]);

            res.booking.bookingId = bookingId;
            res.booking.flightId = flightId;
            res.booking.total_fare = strToInt(fields[4]);
            res.booking.totalSeatsBooked = seatsBooked;
            res.booking.userId = strToInt(fields[1]);
        }
        else
        {
            fputs(buffer, tmpBookingFile);
        }

        for (int i = 0; i < BOOKING_DATA_LENGTH; i++)
            free(fields[i]);
        free(fields);
    }

    fclose(bookingFile);
    fclose(tmpBookingFile);

    if (bookingFound)
    {
        rename(TEMP_BOOKING_FILE, BOOKINGS_FILE);

        while (fgets(buffer, sizeof(buffer), flightFile) != NULL)
        {
            char **fields = splitCSVLine(buffer, FLIGHT_DATA_LENGTH);
            int currentFlightId = strToInt(fields[0]);

            if (currentFlightId == flightId)
            {
                int updatedSeats = strToInt(fields[5]) + seatsBooked;

                fprintf(tmpFlightFile, "%s,%s,%s,%s,%s,%s,%d\n",
                        fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], updatedSeats);
            }
            else
            {
                fputs(buffer, tmpFlightFile);
            }

            for (int i = 0; i < FLIGHT_DATA_LENGTH; i++)
                free(fields[i]);
            free(fields);
        }

        fclose(flightFile);
        fclose(tmpFlightFile);

        rename(TEMP_FLIGHT_FILE, FLIGHT_FILE);

        copyStr(res.message, "Booking canceled successfully.");
        res.success = true;
    }
    else
    {
        fclose(flightFile);
        fclose(tmpFlightFile);

        remove(TEMP_BOOKING_FILE);
        remove(TEMP_FLIGHT_FILE);

        copyStr(res.message, "Booking ID not found. Please recheck and try again.");
        res.success = false;
    }

    return res;
}