#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

// Structure to hold room information
struct RoomInfo {
    int rows;
    int seatsPerRow;
    bool is3D;
    string roomCode;
    string earlyMovieName;
    string earlyMovieCode;
    string lateMovieName;
    string lateMovieCode;
    vector<vector<int>> seatStatus; // 0 = available, 1 = reserved
};

class Cinema {
private:
    string cinemaName;
    vector<RoomInfo> rooms;
    const double BASE_PRICE = 10.0;
    const double PRICE_3D = 5.0;

public:
    Cinema(string name, int numRooms) : cinemaName(name) {
        rooms.resize(numRooms);
        for (int i = 0; i < numRooms; i++) {
            rooms[i].seatStatus = vector<vector<int>>();
        }
    }

    // Get valid integer input with range checking
    int getValidInt(const string& prompt, int minVal, int maxVal) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            
            if (cin.fail() || value < minVal || value > maxVal) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between " 
                     << minVal << " and " << maxVal << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    // Get valid string input
    string getValidString(const string& prompt) {
        string value;
        while (true) {
            cout << prompt;
            getline(cin, value);
            
            if (value.empty()) {
                cout << "Input cannot be empty! Please try again." << endl;
            } else {
                return value;
            }
        }
    }

    // Initialize room details
    void Processing(int roomIndex) {
        if (roomIndex < 0 || roomIndex >= rooms.size()) {
            cout << "Invalid room number!" << endl;
            return;
        }

        cout << "\n=== Setting up Room " << (roomIndex + 1) << " ===" << endl;

        rooms[roomIndex].rows = getValidInt("How many rows does this room have? ", 1, 50);
        rooms[roomIndex].seatsPerRow = getValidInt("How many seats does every row have? ", 1, 100);
        
        int is3D;
        cout << "Is the room 3D? (1 = Yes, 0 = No): ";
        is3D = getValidInt("", 0, 1);
        rooms[roomIndex].is3D = (is3D == 1);

        rooms[roomIndex].roomCode = getValidString("Give room code: ");
        rooms[roomIndex].earlyMovieName = getValidString("Give the name of the movie for early screening: ");
        rooms[roomIndex].earlyMovieCode = getValidString("Give early screening movie code: ");
        rooms[roomIndex].lateMovieName = getValidString("Give the name of the movie for late screening: ");
        rooms[roomIndex].lateMovieCode = getValidString("Give late screening movie code: ");

        // Initialize seat status array
        rooms[roomIndex].seatStatus.resize(rooms[roomIndex].rows, 
                                           vector<int>(rooms[roomIndex].seatsPerRow, 0));

        cout << "Room " << (roomIndex + 1) << " setup complete!" << endl;
    }

    // Check seat availability and display
    void Availability(int roomIndex) {
        if (roomIndex < 0 || roomIndex >= rooms.size()) {
            cout << "Invalid room number!" << endl;
            return;
        }

        cout << "\n=== Availability for Room " << (roomIndex + 1) << " ===" << endl;
        cout << "Movie (Early): " << rooms[roomIndex].earlyMovieName << endl;
        cout << "Movie (Late): " << rooms[roomIndex].lateMovieName << endl;
        cout << "3D: " << (rooms[roomIndex].is3D ? "Yes" : "No") << endl;
        cout << "\nSeat Map (0 = Available, 1 = Reserved):" << endl;

        int availableCount = 0;
        
        // Display row numbers
        cout << "    ";
        for (int j = 0; j < rooms[roomIndex].seatsPerRow && j < 10; j++) {
            cout << j << " ";
        }
        cout << endl;

        for (int i = 0; i < rooms[roomIndex].rows; i++) {
            cout << "R" << i << ": ";
            for (int j = 0; j < rooms[roomIndex].seatsPerRow; j++) {
                cout << rooms[roomIndex].seatStatus[i][j] << " ";
                if (rooms[roomIndex].seatStatus[i][j] == 0) {
                    availableCount++;
                }
            }
            cout << endl;
        }

        cout << "\nTotal available seats: " << availableCount << endl;
    }

    // Reserve a seat
    bool ReserveSeat(int roomIndex) {
        if (roomIndex < 0 || roomIndex >= rooms.size()) {
            cout << "Invalid room number!" << endl;
            return false;
        }

        int row, seat;
        
        cout << "\n=== Seat Reservation ===" << endl;
        row = getValidInt("Give number of row (0-" + to_string(rooms[roomIndex].rows - 1) + "): ", 
                         0, rooms[roomIndex].rows - 1);
        seat = getValidInt("Give number of seat (0-" + to_string(rooms[roomIndex].seatsPerRow - 1) + "): ", 
                          0, rooms[roomIndex].seatsPerRow - 1);

        if (rooms[roomIndex].seatStatus[row][seat] == 1) {
            cout << "Sorry, this seat is already reserved!" << endl;
            return false;
        }

        rooms[roomIndex].seatStatus[row][seat] = 1;
        cout << "Seat reserved successfully!" << endl;
        return true;
    }

    // Issue ticket with pricing
    void TicketIssue(int roomIndex, int row, int seat) {
        if (roomIndex < 0 || roomIndex >= rooms.size()) {
            cout << "Invalid room number!" << endl;
            return;
        }

        cout << "\n========================================" << endl;
        cout << "           CINEMA TICKET                " << endl;
        cout << "========================================" << endl;
        cout << "Cinema: " << cinemaName << endl;
        cout << "Room Code: " << rooms[roomIndex].roomCode << endl;
        cout << "Row: " << row << " | Seat: " << seat << endl;
        cout << "3D Screening: " << (rooms[roomIndex].is3D ? "Yes" : "No") << endl;
        cout << "----------------------------------------" << endl;
        
        double price = BASE_PRICE;
        if (rooms[roomIndex].is3D) {
            price += PRICE_3D;
        }
        
        cout << fixed << setprecision(2);
        cout << "Ticket Price: $" << price << endl;
        cout << "========================================" << endl;
        cout << "Enjoy your movie!" << endl;
        cout << "========================================\n" << endl;
    }

    // Cancel reservation
    bool CancelReservation(int roomIndex) {
        if (roomIndex < 0 || roomIndex >= rooms.size()) {
            cout << "Invalid room number!" << endl;
            return false;
        }

        int row, seat;
        cout << "\n=== Cancel Reservation ===" << endl;
        row = getValidInt("Give number of row: ", 0, rooms[roomIndex].rows - 1);
        seat = getValidInt("Give number of seat: ", 0, rooms[roomIndex].seatsPerRow - 1);

        if (rooms[roomIndex].seatStatus[row][seat] == 0) {
            cout << "This seat is not reserved!" << endl;
            return false;
        }

        rooms[roomIndex].seatStatus[row][seat] = 0;
        cout << "Reservation cancelled successfully!" << endl;
        return true;
    }

    // Display cinema info
    void DisplayInfo() {
        cout << "\n========================================" << endl;
        cout << "         CINEMA INFORMATION             " << endl;
        cout << "========================================" << endl;
        cout << "Cinema Name: " << cinemaName << endl;
        cout << "Total Rooms: " << rooms.size() << endl;
        cout << "========================================\n" << endl;
    }
};

// Display main menu
void displayMenu() {
    cout << "\n========== CINEMA MANAGEMENT SYSTEM ==========" << endl;
    cout << "1. Setup Room Information" << endl;
    cout << "2. Check Seat Availability" << endl;
    cout << "3. Reserve a Seat" << endl;
    cout << "4. Issue Ticket" << endl;
    cout << "5. Cancel Reservation" << endl;
    cout << "6. Display Cinema Info" << endl;
    cout << "7. Exit" << endl;
    cout << "==============================================" << endl;
    cout << "Choose an option: ";
}

int main() {
    string cinemaName;
    int numRooms;

    cout << "========================================" << endl;
    cout << "   WELCOME TO CINEMA MANAGEMENT SYSTEM  " << endl;
    cout << "========================================" << endl;

    // Get cinema name
    cout << "Give the name of the cinema: ";
    getline(cin, cinemaName);

    // Get number of rooms with validation
    while (true) {
        cout << "Give number of screening rooms in the cinema (1-20): ";
        cin >> numRooms;
        
        if (cin.fail() || numRooms < 1 || numRooms > 20) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 20." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    // Create cinema object
    Cinema cinema(cinemaName, numRooms);

    // Initialize all rooms
    cout << "\nPlease setup all rooms first:" << endl;
    for (int i = 0; i < numRooms; i++) {
        cinema.Processing(i);
    }

    // Main menu loop
    int choice;
    string continueChoice;

    do {
        displayMenu();
        choice = cinema.getValidInt("", 1, 7);

        switch (choice) {
            case 1: {
                int room = cinema.getValidInt("Which room to setup (1-" + to_string(numRooms) + ")? ", 
                                             1, numRooms);
                cinema.Processing(room - 1);
                break;
            }
            case 2: {
                int room = cinema.getValidInt("Which room's availability to check (1-" + to_string(numRooms) + ")? ", 
                                             1, numRooms);
                cinema.Availability(room - 1);
                break;
            }
            case 3: {
                int room = cinema.getValidInt("Which room (1-" + to_string(numRooms) + ")? ", 
                                             1, numRooms);
                cinema.ReserveSeat(room - 1);
                break;
            }
            case 4: {
                int room = cinema.getValidInt("Which room (1-" + to_string(numRooms) + ")? ", 
                                             1, numRooms);
                int row = cinema.getValidInt("Row number: ", 0, 50);
                int seat = cinema.getValidInt("Seat number: ", 0, 100);
                cinema.TicketIssue(room - 1, row, seat);
                break;
            }
            case 5: {
                int room = cinema.getValidInt("Which room (1-" + to_string(numRooms) + ")? ", 
                                             1, numRooms);
                cinema.CancelReservation(room - 1);
                break;
            }
            case 6: {
                cinema.DisplayInfo();
                break;
            }
            case 7: {
                cout << "\nThank you for using Cinema Management System!" << endl;
                cout << "Goodbye!" << endl;
                return 0;
            }
        }

        cout << "\nContinue? (Yes/No): ";
        getline(cin, continueChoice);
        
    } while (continueChoice == "Yes" || continueChoice == "yes" || continueChoice == "Y" || continueChoice == "y");

    cout << "\nThank you for using Cinema Management System!" << endl;
    return 0;
}