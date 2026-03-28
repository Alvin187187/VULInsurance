#include <iostream>
#include <string>
#include <iomanip> //a library that is useful for making good and clean tables. (came from an online discussion)

//general notice: setw is part of the iomanip library which sets spaces precisely. 
//additional: set precision rounds off numbers to whole part of iomanip also. 

using namespace std;

const int maxyears = 100; //maximum years to simulate

//rates of the investment plan
const double rate1 = 0.04;  // 4% growth plan
const double rate2 = 0.08;  // 8% growth plan
const double rate3 = 0.10; // 10% growth plan

const int max_clients = 100; // Setting maximum number of clients (since it will crash the console)

//Primary variables of the system needed to function. (utilized arrays for the data records)
struct ClientData {
    string name[max_clients];
    int age[max_clients];
    double premium[max_clients];
    double faceamounts[max_clients];
    double charge[max_clients];
    int years[max_clients];
    int count = 0; // Current number of clients
} db;

//Utilizing recursive function to calculate fund value
double calculateFund(double previousFund, double premium, double rate, double chargesPercent, int year) {
    if (year == 0)
        return previousFund;

    double newFund = (previousFund + premium) * (1 + rate) - ((previousFund + premium) * chargesPercent);
    return calculateFund(newFund, premium, rate, chargesPercent, year - 1);
}

// A function to call in order to simulate and print the VUL Projection Fund Table of the client
void simulation(int index) {
    cout << "----------VUL Projection Table for " << db.name[index] << "----------" << endl;
    cout << left << setw(5) << "Year" << setw(5) << "Age" << setw(18) << "Fund Value (4%)" << setw(18) << "Death Benefit (4%)" << setw(18) << "Fund Value (8%)" << setw(18) << "Death Benefit (8%)" << setw(18) << "Fund Value (10%)" << setw(18) << "Death Benefit (10%)" << endl << string(110, '-') << endl;

    //arrays that will store fund values!
    double fund4[maxyears + 1] = { 0 }, fund8[maxyears + 1] = { 0 }, fund10[maxyears + 1] = { 0 };

    //a part of code that will be the counter of the age (looping for year display)
    for (int year = 0; year <= db.years[index]; ++year) { //20
        int currentAge = db.age[index] + year;
        cout << setw(5) << (2025 + year) << setw(5) << currentAge; //starting year is 2025 and will add-up depending on the user's inputted years to simulate.

        double rates[3] = { rate1, rate2, rate3 }; //stored as array
        double* funds[3] = { fund4, fund8, fund10 }; //assigning by pointer wherein it points to each rates

        for (int i = 0; i < 3; ++i) { //loop for printing no growth
            if (year == 0) {
                funds[i][year] = 0.0; //implies if the year is 0, meaning there's no growth.
            }
            else {
                funds[i][year] = calculateFund(funds[i][year - 1], db.premium[index], rates[i], db.charge[index], 1);
            }

            double deathBenefit = (funds[i][year] > db.faceamounts[index] * 2) ? funds[i][year] : db.faceamounts[index] * 2; //calculation of death benefit  
            cout << setw(18) << fixed << setprecision(0) << funds[i][year] << setw(18) << deathBenefit;
        }
        cout << endl; //line break

        if (currentAge > 100) break;
    }
}

// Function for the user to add client
void addClient() {
    if (db.count >= max_clients) { //condition to check if reached maximum
        cout << "Maximum clients reached." << endl;
        return;
    }
    cout << "Enter the client's name: ";
    // FIX: Separated cin >> ws and getline, then added cin.ignore() after each
    // numeric read to flush the leftover newline. Without this, the second word
    // of a spaced name (e.g. "Alvin D") stays in the buffer and feeds straight
    // into the next cin >> causing an infinite error loop.
    cin >> ws;
    getline(cin, db.name[db.count]);

    cout << "Enter the starting age: ";
    while (!(cin >> db.age[db.count])) {
        cout << "Error! Age must be expressed in number. Please try again. "; // FIX: was missing semicolon here
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n'); // FIX: flush newline left after reading age

    cout << "Enter the annual premium: ";
    cin >> db.premium[db.count];
    cin.ignore(1000, '\n'); // FIX: flush after each numeric read

    cout << "Enter the face amount: ";
    cin >> db.faceamounts[db.count];
    cin.ignore(1000, '\n');

    cout << "Enter the company charges percentage (e.g., 0.02 for 2%): ";
    cin >> db.charge[db.count];
    cin.ignore(1000, '\n');

    cout << "Enter years to simulate: ";
    cin >> db.years[db.count];
    cin.ignore(1000, '\n');

    db.count++;
    cout << " " << endl;
    cout << "Client was added successfully with ID: " << db.count << endl;
}

//A function to view all of current clients
void viewAllClients() {
    if (db.count == 0) { //check if the client ID exist
        cout << "No clients in the system." << endl;
        return;
    }
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(5) << "Age" << setw(15) << "Annual Premium" << setw(15) << "Face Amount" << setw(10) << "Charges" << endl;
    cout << string(70, '-') << endl;
    for (int i = 0; i < db.count; ++i) {
        cout << setw(5) << (i + 1) << setw(20) << db.name[i] << setw(5) << db.age[i] << setw(15) << fixed << setprecision(2) << db.premium[i] << setw(15) << db.faceamounts[i] << setw(10) << (db.charge[i] * 100) << "%" << endl;
    }
}

//Function to search client by name or ID
void searchClient() {
    cout << "Enter client ID (number) or name to search: ";
    string query;
    cin >> ws;
    getline(cin, query); // FIX: use getline so spaced names can be searched too

    int id = -1;

    //Check if query is a number (ID)
    bool isNumber = true; //using boolean
    for (char c : query) {
        if (c < '0' || c > '9') {
            isNumber = false;
            break;
        }
    }
    if (isNumber) {
        id = 0;
        for (char c : query) {
            id = id * 10 + (c - '0');
        }
        id--; //Since IDs start from 1
    }

    bool found = false;

    //part that will print the information of client (if the client was found)
    for (int i = 0; i < db.count; ++i) {
        if ((id != -1 && i == id) || db.name[i] == query) {
            cout << "Client Details:" << endl;
            cout << "ID: " << (i + 1) << endl;
            cout << "Name: " << db.name[i] << endl;
            cout << "Starting Age: " << db.age[i] << endl;
            cout << "Annual Premium: " << fixed << setprecision(2) << db.premium[i] << endl;
            cout << "Face Amount: " << db.faceamounts[i] << endl;
            cout << "Charges Percent: " << (db.charge[i] * 100) << "%" << endl;
            cout << "Years to Simulate: " << db.years[i] << endl;
            found = true;
            break;
        }
    }
    if (!found) { //response if the client is not found
        cout << "Client not found." << endl;
    }
}

//Function to simulate the fund growth for the selected client
void simulateFundGrowth() {
    if (db.count == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to simulate: ";
    int id;
    cin >> id;
    cin.ignore(1000, '\n');
    if (id < 1 || id > db.count) {
        cout << "Invalid ID." << endl;
        return;
    }
    simulation(id - 1);
}

//Function to display client summary
void viewClientSummary() {
    if (db.count == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to view summary: ";
    int id;
    cin >> id;
    cin.ignore(1000, '\n');
    if (id < 1 || id > db.count) {
        cout << "Invalid ID." << endl;
        return;
    }

    int index = id - 1;
    double totalContributions = db.premium[index] * db.years[index];
    cout << "Client Summary for " << db.name[index] << ":" << endl;
    cout << "Total Contributions: " << fixed << setprecision(2) << totalContributions << endl;

    //Calculator for final fund values for each rate
    double finalFund4 = calculateFund(0, db.premium[index], rate1, db.charge[index], db.years[index]);
    double finalFund8 = calculateFund(0, db.premium[index], rate2, db.charge[index], db.years[index]);
    double finalFund10 = calculateFund(0, db.premium[index], rate3, db.charge[index], db.years[index]);

    cout << "Final Fund Value (4%): " << fixed << setprecision(0) << finalFund4 << endl;
    cout << "Final Fund Value (8%): " << finalFund8 << endl;
    cout << "Final Fund Value (10%): " << finalFund10 << endl;

    double deathBenefit4 = (finalFund4 > db.faceamounts[index] * 2) ? finalFund4 : db.faceamounts[index] * 2;
    double deathBenefit8 = (finalFund8 > db.faceamounts[index] * 2) ? finalFund8 : db.faceamounts[index] * 2;
    double deathBenefit10 = (finalFund10 > db.faceamounts[index] * 2) ? finalFund10 : db.faceamounts[index] * 2;

    //displaying benefits for death with maturity level
    cout << "Death Benefit at Maturity (4%): " << deathBenefit4 << endl;
    cout << "Death Benefit at Maturity (8%): " << deathBenefit8 << endl;
    cout << "Death Benefit at Maturity (10%): " << deathBenefit10 << endl;
}

//A function to update client's details
void updateClient() { //verifying and query for the selected client
    if (db.count == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to update: ";
    int id;
    cin >> id;
    cin.ignore(1000, '\n'); // FIX: flush before getline for name
    if (id < 1 || id > db.count) {
        cout << "Invalid ID." << endl;
        return;
    }

    int index = id - 1;

    cout << "Enter new name (current: " << db.name[index] << "): ";
    getline(cin, db.name[index]); // FIX: use getline to support spaced names
    cout << "Enter new starting age (current: " << db.age[index] << "): ";
    cin >> db.age[index];
    cin.ignore(1000, '\n');
    cout << "Enter new annual premium (current: " << db.premium[index] << "): ";
    cin >> db.premium[index];
    cin.ignore(1000, '\n');
    cout << "Enter new face amount (current: " << db.faceamounts[index] << "): ";
    cin >> db.faceamounts[index];
    cin.ignore(1000, '\n');
    cout << "Enter new charges percent (current: " << db.charge[index] << "): ";
    cin >> db.charge[index];
    cin.ignore(1000, '\n');
    cout << "Enter new years to simulate (current: " << db.years[index] << "): ";
    cin >> db.years[index];
    cin.ignore(1000, '\n');
    cout << "Client updated successfully." << endl;
}

//Function to delete client record
void deleteClient() {
    if (db.count == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to delete: ";
    int id;
    cin >> id;
    cin.ignore(1000, '\n');
    if (id < 1 || id > db.count) {
        cout << "Invalid ID." << endl;
        return;
    }
    int index = id - 1;
    // Shift elements
    for (int i = index; i < db.count - 1; ++i) {
        db.name[i] = db.name[i + 1];
        db.age[i] = db.age[i + 1];
        db.premium[i] = db.premium[i + 1];
        db.faceamounts[i] = db.faceamounts[i + 1];
        db.charge[i] = db.charge[i + 1];
        db.years[i] = db.years[i + 1];
    }
    db.count--;
    cout << "Client deleted successfully." << endl;
}

//main part of the code
int main() {
    int choice;
    //actual display for possible inquries of the advisor.
    do {
        cout << "----------VUL Client Management System----------" << endl;
        cout << " " << endl;
        cout << "1 -> Add a New Client" << endl;
        cout << "2 -> View All Clients" << endl;
        cout << "3 -> Search Client by Name or ID" << endl;
        cout << "4 -> Simulate Fund Growth" << endl;
        cout << "5 -> View Client Summary" << endl;
        cout << "6 -> Update the Client Details" << endl;
        cout << "7 -> Delete a Client Record" << endl;
        cout << "0 -> Exit the Program" << endl;
        cout << " " << endl;
        cout << "Enter a number of your choice: ";

        //utilizing switch case for the inquries.
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            continue;
        }
        switch (choice) {
        case 1: addClient();
            break;
        case 2: viewAllClients();
            break;
        case 3: searchClient();
            break;
        case 4: simulateFundGrowth();
            break;
        case 5: viewClientSummary();
            break;
        case 6: updateClient();
            break;
        case 7: deleteClient();
            break;

        case 0: cout << "Exiting..." << endl;
            break;
        default: cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
