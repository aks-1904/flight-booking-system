#ifndef BOOKING_H
#define BOOKING_H

#define BOOKING_ID_SIZE 9
#define BOOKINGS_FILE "./data/bookings.csv"
#define BOOKING_DATA_LENGTH 5
#define TEMP_BOOKING_FILE "tmpbooking.csv"

typedef struct
{
    int bookingId;
    int userId;
    int flightId;
    int totalSeatsBooked;
    int total_fare;
} Booking;

typedef struct
{
    bool success;
    char message[100];
    Booking booking;
} BookingResponse;

typedef struct
{
    int bookingId;
    char destination[50];
    char departure_date[12]; // YYYY-MM-DD
    char departure_time[7];  // HH:MM
    int totalSeatsBooked;
    int totalFare;
} AllBookingResponse;

BookingResponse bookFlight(Booking bookingData);
BookingResponse cancelBooking(int bookingId);
AllBookingResponse *showAllBookings(int userId, int *returnSize);

#endif