#ifndef FLIGHT_H
#define FLIGHT_H

#define FLIGHT_FILE "./data/flights.csv"

typedef struct{
    int flight_number;
    char destination[50];
    char departure_date[12]; // YYYY-MM-DD
    char departure_time[7]; // HH:MM
    int ticket_price;
    int total_seats;
    int available_seats;
} Flight;

typedef struct{
    bool success;
    char message[100];
    Flight flight;
} FlightResponse;

FlightResponse addFlight(Flight *flight);

#endif