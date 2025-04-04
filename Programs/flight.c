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