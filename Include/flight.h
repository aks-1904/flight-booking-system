#ifndef FLIGHT_H
#define FLIGHT_H

#define FLIGHT_FILE "./data/flights.csv"
#define FLIGHT_DATA_LENGTH 7
#define MAX_FLIGHTS 100
#define TEMP_FLIGHT_FILE "tmpflight.csv"

typedef struct
{
    int flight_number;
    char destination[50];
    char departure_date[12]; // YYYY-MM-DD
    char departure_time[7];  // HH:MM
    int ticket_price;
    int total_seats;
    int available_seats;
} Flight;

typedef struct
{
    bool success;
    char message[100];
    Flight flight;
} FlightResponse;

FlightResponse addFlight(Flight *flight);
Flight *displayFlights(char *destiantion, int *returnSize);
bool cancelFlight(int flight_number);
FlightResponse findFlight(int flight_number);

#endif