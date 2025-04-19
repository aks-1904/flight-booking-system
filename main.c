#include "./Include/main.h"

void main()
{
    srand(time(NULL));
    mainMenu();
}

void mainMenu()
{
    while (true)
    {
        printf("<----------Select a option---------->\n");
        printf("1. Login in as admin\n");
        printf("2. Register as a user\n");
        printf("3. Login as user\n");
        printf("4. Quit\n");

        int choice;
        Admin adminData;
        User userData;
        Response res;

        printf("Choose Option: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            printf("Enter the username: ");
            scanf("%s", adminData.username);
            printf("Enter the password: ");
            scanf("%s", adminData.password);

            bool isAdmin = loginAdmin(adminData);
            if (isAdmin)
                adminMenu();
            else
                printf("Failed to logged in as admin check details, and then continue\n");
            break;

        case 2:
            printf("Enter username (only a-z, A-Z, 1-9, _, @, # allowed): ");
            fgets(userData.username, sizeof(userData.username), stdin);
            trimNewline(userData.username);

            while (!checkValidUsername(userData.username))
            {
                printf("Please enter a valid username: ");
                fgets(userData.username, sizeof(userData.username), stdin);
                trimNewline(userData.username);
            }

            while (isUsernameTaken(userData.username))
            {
                printf("Username already taken, please try another: ");
                fgets(userData.username, sizeof(userData.username), stdin);
                trimNewline(userData.username);
            }

            printf("Enter the password: ");
            fgets(userData.password, sizeof(userData.password), stdin);
            trimNewline(userData.password);

            userData.userId = generateRandomId(USER_ID_SIZE);

            res = registerUser(&userData);
            if (res.success)
            {
                printf("<--------- %s ---------->\n", res.message);
                userMenu(res.user);
            }
            else
            {
                printf("<---------- %s ---------->\n", res.message);
            }
            break;

        case 3:
            printf("Enter username (only a-z, A-Z, 1-9, _, @, # allowed): ");
            fgets(userData.username, sizeof(userData.username), stdin);
            trimNewline(userData.username);

            while (!checkValidUsername(userData.username))
            {
                printf("Please enter a valid username: ");
                fgets(userData.username, sizeof(userData.username), stdin);
                trimNewline(userData.username);
            }

            printf("Enter the password: ");
            fgets(userData.password, sizeof(userData.password), stdin);
            trimNewline(userData.password);

            res = loginUser(&userData);

            if (res.success)
            {
                printf("<--------- %s ---------->\n", res.message);
                userMenu(res.user);
            }
            else
            {
                printf("<---------- %s ---------->\n", res.message);
            }
            break;

        case 4:
            printf("Quiting...\n");
            return;

        default:
            printf("Invalid Option");
            break;
        }
    }
}

void adminMenu()
{
    printf("<----------Logged in as admin (%s)---------->\n", ADMIN_USERNAME);
    while (true)
    {
        printf("Choice amoung following:-\n");
        printf("1. Add a flight\n");
        printf("2. Remove a flight\n");
        printf("3. Check a flight details\n");
        printf("4. Logout\n");

        int choice;
        Flight flight;
        FlightResponse res;

        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        while (getchar() != '\n')
            ; // Clear newline from buffer

        switch (choice)
        {
        case 1:
        {
            char flight_number[10], ticket_price[10], total_seats[10];

            printf("Enter the details of flight to continue:-\n");

            printf("Enter flight number: ");
            fgets(flight_number, sizeof(flight_number), stdin);
            trimNewline(flight_number);
            while (!checkValidInt(flight_number))
            {
                printf("Enter valid flight number: ");
                fgets(flight_number, sizeof(flight_number), stdin);
                trimNewline(flight_number);
            }

            printf("Enter destination: ");
            fgets(flight.destination, sizeof(flight.destination), stdin);
            trimNewline(flight.destination);

            printf("Enter the departure date (Format:- YYYY-MM-DD): ");
            fgets(flight.departure_date, sizeof(flight.departure_date), stdin);
            trimNewline(flight.departure_date);
            while (!isValidDate(flight.departure_date))
            {
                printf("Enter valid Departure date: ");
                fgets(flight.departure_date, sizeof(flight.departure_date), stdin);
                trimNewline(flight.departure_date);
            }

            printf("Enter the departure time (Format:- HH:MM): ");
            fgets(flight.departure_time, sizeof(flight.departure_time), stdin);
            trimNewline(flight.departure_time);
            while (!isValidTime(flight.departure_time))
            {
                printf("Enter valid Departure time: ");
                fgets(flight.departure_time, sizeof(flight.departure_time), stdin);
                trimNewline(flight.departure_time);
            }

            printf("Enter the ticket price: ");
            fgets(ticket_price, sizeof(ticket_price), stdin);
            trimNewline(ticket_price);
            while (!checkValidInt(ticket_price))
            {
                printf("Enter valid Ticket price: ");
                fgets(ticket_price, sizeof(ticket_price), stdin);
                trimNewline(ticket_price);
            }

            printf("Enter total seats: ");
            fgets(total_seats, sizeof(total_seats), stdin);
            trimNewline(total_seats);
            while (!checkValidInt(total_seats))
            {
                printf("Enter valid Total seats: ");
                fgets(total_seats, sizeof(total_seats), stdin);
                trimNewline(total_seats);
            }

            flight.flight_number = strToInt(flight_number);
            flight.ticket_price = strToInt(ticket_price);
            flight.total_seats = strToInt(total_seats);
            flight.available_seats = flight.total_seats;

            res = addFlight(&flight);

            if (res.success)
            {
                printf("\n<---------- Flight created successfully, following are the details ---------->\n");
                printf("Flight Number: %d\nDestination: %s\nDeparture Date: %s\nDeparture Time: %s\nTicket Price: %d\nTotal Seats: %d\n",
                       res.flight.flight_number, res.flight.destination, res.flight.departure_date,
                       res.flight.departure_time, res.flight.ticket_price, res.flight.total_seats);
            }
            else
            {
                printf("\n<---------- %s ---------->\n", res.message);
            }
            break;
        }

        case 4:
            printf("Logging Out...\n");
            return;

        default:
            printf("Invalid Option. Please try again.\n");
            break;
        }
    }
}

void userMenu(User loggedInUser)
{
    printf("<----------Logged in as user (%s)---------->\n", loggedInUser.username);

    int choice;
    Booking bookingData;
    BookingResponse res;
    int returnSize;

    while (true)
    {
        printf("Choice amoung following:-\n");
        printf("1. Book a flight:-\n");
        printf("2. View all bookings:-\n");
        printf("3. Cancel a bookings:-\n");
        printf("4. Logout:-\n");

        printf("Select an option:- ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        while (getchar() != '\n')
            ; // Clear newline from buffer

        switch (choice)
        {
        case 1:
            char destination[50];
            char filter = 0;
            printf("Enter the destination to search for flight: ");
            fgets(destination, 50, stdin);
            trimNewline(destination);

            Flight *flights = displayFlights(destination, &returnSize);

            if (returnSize == 0)
            {
                printf("Soory, no flight available for this destination");
                break;
            }

            printf("Following are the flight details: \n\n");
            printFlights(flights, returnSize);

            printf("Want to filter? (Type y to filter): ");
            scanf("%c", &filter);

            if (filter == 'y' || filter == 'Y')
            {
                int option;
                printf("1. According to departure date and time\n2. According to price\n");
                printf("Choose:- ");
                while (option != 1 && option != 2)
                {
                    scanf("%d", &option);

                    switch (option)
                    {
                    case 1:
                        sortAccDeparture(flights, returnSize);
                        printFlights(flights, returnSize);
                        break;
                    case 2:
                        sortAccPrice(flights, returnSize);
                        printFlights(flights, returnSize);
                        break;

                    default:
                        printf("\n\n<--------- Invalid option -------->\n\n");
                        break;
                    }
                }
            }

            free(flights);
            flights = NULL;

            bookingData.bookingId = generateRandomId(9);
            bookingData.userId = loggedInUser.userId;
            printf("Enter flight id to book: ");
            scanf("%d", &bookingData.flightId);

            printf("Enter total seats you want to book: ");
            scanf("%d", &bookingData.totalSeatsBooked);

            res = bookFlight(bookingData);

            if (res.success)
            {
                printf("<---------- %s ---------->\n", res.message);
                printf("Following are the details of booking: \n");
                printf("Booking Id:- %d\nUser Id- %d\nFlight Id:- %d\n Total Seats Booked:- %d\n", res.booking.bookingId, res.booking.userId, res.booking.flightId, res.booking.totalSeatsBooked);
            }
            else
            {
                printf("<---------- %s ---------->", res.message);
            }
            break;

        case 2:
            printAllBookings(loggedInUser.userId);
            break;

        case 3:
            printAllBookings(loggedInUser.userId);
            int bookingId;

            printf("Enter the booking id to cancel a booking: ");
            scanf("%d", &bookingId);

            res = cancelBooking(bookingId);

            if (res.success)
            {
                printf("<---------- %s ---------->\n", res.message);
                printf("Following are the details of booking that has been deleted: \n");
                printf("Booking Id:- %d\nUser Id- %d\nFlight Id:- %d\n Total Seats Booked:- %d\n", res.booking.bookingId, res.booking.userId, res.booking.flightId, res.booking.totalSeatsBooked);
            }
            else
            {
                printf("<---------- %s ---------->", res.message);
            }
            break;

        case 4:
            printf("Logging out...");
            return;
            break;

        default:
            printf("Invalid option\n");
            break;
        }
    }
}
