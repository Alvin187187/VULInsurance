#include <iostream>
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
string client_name[max_clients];
int client_age[max_clients];
double premium_client[max_clients];
double client_faceamounts[max_clients];
double company_charge[max_clients];
int client_years[max_clients]; 
int clientCount = 0; // Current number of clients

//Utilizing recursive function to calculate fund value
double calculateFund(double previousFund, double premium, double rate, double chargesPercent, int year) {
    if (year == 0)
        return previousFund;

    double newFund = (previousFund + premium) * (1 + rate) - ((previousFund + premium) * chargesPercent);
    return calculateFund(newFund, premium, rate, chargesPercent, year - 1);
}

// A function to call in order to simulate and print the VUL Projection Fund Table of the client
void simulation(int index) {
    cout << "----------VUL Projection Table for " << client_name[index] << "----------" << endl;
    cout << left << setw(5) << "Year" << setw(5) << "Age";
    cout << setw(18) << "Fund Value (4%)" << setw(18) << "Death Benefit (4%)";
    cout << setw(18) << "Fund Value (8%)" << setw(18) << "Death Benefit (8%)";
    cout << setw(18) << "Fund Value (10%)" << setw(18) << "Death Benefit (10%)";
    cout << endl << string(80, '-') << endl;

    //arrays that will store fund values!
    double fund4[maxyears + 1] = { 0 };
    double fund8[maxyears + 1] = { 0 };
    double fund10[maxyears + 1] = { 0 };


    //a part of code that will be the counter of the age (looping for year display)
    for (int year = 0; year <= client_years[index]; ++year) { //20
        int currentAge = client_age[index] + year;
        cout << setw(5) << (2025 + year) << setw(5) << currentAge; //starting year is 2025 and will add-up depending on the user's inputted years to simulate.

        double rates[3] = { rate1, rate2, rate3 }; //stored as array
        double* funds[3] = { fund4, fund8, fund10 }; //assigning by pointer wherein it points to each rates

        for (int i = 0; i < 3; ++i) { //loop for printing no growth
            if (year == 0) {
                funds[i][year] = 0.0; //implies if the year is 0, meaning there's no growth.
            }
            else {
                funds[i][year] = calculateFund(funds[i][year - 1], premium_client[index], rates[i], company_charge[index], 1);
            }

            double deathBenefit = (funds[i][year] > client_faceamounts[index] * 2) ? funds[i][year] : client_faceamounts[index] * 2; //calculation of death benefit  
            cout << setw(18) << fixed << setprecision(0) << funds[i][year];
            cout << setw(18) << deathBenefit;
        }
        cout << endl; //line break

        if (currentAge > 100) break;
    }
}

// Function for the user to add client
void addClient() {
    if (clientCount >= max_clients) { //condition to check if reached maximum
        cout << "Maximum clients reached." << endl;
        return;
    }
    cout << "Enter the client's name (single word): ";
    cin >> client_name[clientCount];
    cout << "Enter the starting age: ";
    cin >> client_age[clientCount];
    cout << "Enter the annual premium: ";
    cin >> premium_client[clientCount];
    cout << "Enter the face amount: ";
    cin >> client_faceamounts[clientCount];
    cout << "Enter the company charges percentage (e.g., 0.02 for 2%): ";
    cin >> company_charge[clientCount];
    cout << "Enter years to simulate: ";
    cin >> client_years[clientCount];
    clientCount++;
    cout << " " << endl;
    cout << "Client was added successfully with ID: " << clientCount << endl;
}

//A function to view all of current clients
void viewAllClients() {
    if (clientCount == 0) { //check if the client ID exist
        cout << "No clients in the system." << endl;
        return;
    }
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(5) << "Age" << setw(15) << "Annual Premium" << setw(15) << "Face Amount" << setw(10) << "Charges" << endl;
    cout << string(70, '-') << endl;
    for (int i = 0; i < clientCount; ++i) {
        cout << setw(5) << (i + 1) << setw(20) << client_name[i] << setw(5) << client_age[i];
        cout << setw(15) << fixed << setprecision(2) << premium_client[i];
        cout << setw(15) << client_faceamounts[i] << setw(10) << (company_charge[i] * 100) << "%" << endl;
    }
}

//Function to search client by name or ID
void searchClient() {
    cout << "Enter client ID (number) or name (single word) to search: ";
    string query;
    cin >> query; //I utilized query in order to search.

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
    for (int i = 0; i < clientCount; ++i) {
        if ((id != -1 && i == id) || client_name[i] == query) {
            cout << "Client Details:" << endl;
            cout << "ID: " << (i + 1) << endl;
            cout << "Name: " << client_name[i] << endl;
            cout << "Starting Age: " << client_age[i] << endl;
            cout << "Annual Premium: " << fixed << setprecision(2) << premium_client[i] << endl;
            cout << "Face Amount: " << client_faceamounts[i] << endl;
            cout << "Charges Percent: " << (company_charge[i] * 100) << "%" << endl;
            cout << "Years to Simulate: " << client_years[i] << endl;
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
    if (clientCount == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to simulate: ";
    int id;
    cin >> id;
    if (id < 1 || id > clientCount) {
        cout << "Invalid ID." << endl;
        return;
    }
    simulation(id - 1);
}

//Function to display client summary
void viewClientSummary() {
    if (clientCount == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to view summary: ";
    int id;
    cin >> id;
    if (id < 1 || id > clientCount) {
        cout << "Invalid ID." << endl;
        return;
    }

    int index = id - 1;
    double totalContributions = premium_client[index] * client_years[index];
    cout << "Client Summary for " << client_name[index] << ":" << endl;
    cout << "Total Contributions: " << fixed << setprecision(2) << totalContributions << endl;

    //Calculator for final fund values for each rate
    double finalFund4 = calculateFund(0, premium_client[index], rate1, company_charge[index], client_years[index]);
    double finalFund8 = calculateFund(0, premium_client[index], rate2, company_charge[index], client_years[index]);
    double finalFund10 = calculateFund(0, premium_client[index], rate3, company_charge[index], client_years[index]);

    cout << "Final Fund Value (4%): " << fixed << setprecision(0) << finalFund4 << endl;
    cout << "Final Fund Value (8%): " << finalFund8 << endl;
    cout << "Final Fund Value (10%): " << finalFund10 << endl;

    double deathBenefit4 = (finalFund4 > client_faceamounts[index] * 2) ? finalFund4 : client_faceamounts[index] * 2;
    double deathBenefit8 = (finalFund8 > client_faceamounts[index] * 2) ? finalFund8 : client_faceamounts[index] * 2;
    double deathBenefit10 = (finalFund10 > client_faceamounts[index] * 2) ? finalFund10 : client_faceamounts[index] * 2;

    //displaying benefits for death with maturity level
    cout << "Death Benefit at Maturity (4%): " << deathBenefit4 << endl;
    cout << "Death Benefit at Maturity (8%): " << deathBenefit8 << endl;
    cout << "Death Benefit at Maturity (10%): " << deathBenefit10 << endl;
}

//A function to update client's details
void updateClient() { //verifying and query for the selected client
    if (clientCount == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to update: ";
    int id;
    cin >> id;
    if (id < 1 || id > clientCount) {
        cout << "Invalid ID." << endl;
        return;
    }

    int index = id - 1;

    //actual dislay
    cout << "Enter new name (current: " << client_name[index] << "): ";
    cin >> client_name[index];
    cout << "Enter new starting age (current: " << client_age[index] << "): ";
    cin >> client_age[index];
    cout << "Enter new annual premium (current: " << premium_client[index] << "): ";
    cin >> premium_client[index];
    cout << "Enter new face amount (current: " << client_faceamounts[index] << "): ";
    cin >> client_faceamounts[index];
    cout << "Enter new charges percent (current: " << company_charge[index] << "): ";
    cin >> company_charge[index];
    cout << "Enter new years to simulate (current: " << client_years[index] << "): ";
    cin >> client_years[index];
    cout << "Client updated successfully." << endl;
}

//Function to delete client record
void deleteClient() {
    if (clientCount == 0) {
        cout << "No clients available." << endl;
        return;
    }
    cout << "Enter client ID to delete: ";
    int id;
    cin >> id;
    if (id < 1 || id > clientCount) {
        cout << "Invalid ID." << endl;
        return;
    }
    int index = id - 1;
    // Shift elements
    for (int i = index; i < clientCount - 1; ++i) {
        client_name[i] = client_name[i + 1];
        client_age[i] = client_age[i + 1];
        premium_client[i] = premium_client[i + 1];
        client_faceamounts[i] = client_faceamounts[i + 1];
        company_charge[i] = company_charge[i + 1];
        client_years[i] = client_years[i + 1];
    }
    clientCount--;
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
        cin >> choice;

        //utilizing switch case for the inquries.
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
