#include "./Include/main.h"
#include "./Programs/flight.c"

void main()
{
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

        printf("Choose Option: ");
        scanf("%d", &choice);

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
        printf("Choice amount following:-\n");
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