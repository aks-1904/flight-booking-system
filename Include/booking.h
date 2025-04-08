#ifndef BOOKING_H
#define BOOKING_H

#define BOOKING_ID_SIZE 9
#define BOOKINGS_FILE "./data/bookings.csv"
#define BOOKING_DATA_LENGTH 5

typedef struct{
    int bookingId;
    int userId;
    int flightId;
    int totalSeatsBooked;
    int total_fare;
} Booking;

typedef struct{
    bool success;
    char message[100];
    Booking booking;
} BookingResponse;

BookingResponse bookFlight(Booking bookingData);
BookingResponse cancelBooking(int bookingId);

#endif