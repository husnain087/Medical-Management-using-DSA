#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <sstream>
using namespace std;

struct Patient {
    int id;
    string name;
    string cnic;
    int age;
    string gender;
    string contact;
    string address;
    string admissionDate;
    string doctorAssigned;
    string medicalHistory;
    string currentIllness;
    string treatmentPlan;
    int roomNumber;
    double billingAmount;
    string priority;
    bool emergency;
    string condition;
    Patient* next;
};

class HospitalManagement {
private:
    Patient* head;

public:
    HospitalManagement() : head(nullptr) {
        loadPatients(); // Load patients from file on initialization
    }

    void addPatient(int id, string name, string cnic, int age, string gender, string contact, string address, 
                    string admissionDate, string doctorAssigned, string medicalHistory, string currentIllness,
                    string treatmentPlan, int roomNumber, double billingAmount, string priority, 
                    bool emergency, string condition) {
        // Check if the room is already allocated
        if (isRoomAllocated(roomNumber)) {
            cout << "Room " << roomNumber << " is already booked. Please select an alternate room.\n";
            displayAvailableRooms();
            cout << "Enter alternate Room Number (1-35): ";
            cin >> roomNumber;
        }

        Patient* newPatient = new Patient{id, name, cnic, age, gender, contact, address, admissionDate,
                                            doctorAssigned, medicalHistory, currentIllness, treatmentPlan,
                                            roomNumber, billingAmount, priority, emergency, condition, nullptr};

        newPatient->next = head;
        head = newPatient;

        cout << "Patient added successfully.\n";
        savePatientToFile(newPatient); // Save the new patient to the file
    }

    bool isRoomAllocated(int roomNumber) {
        Patient* temp = head;
        while (temp != nullptr) {
            if (temp->roomNumber == roomNumber) {
                return true; // Room is already allocated
            }
            temp = temp->next;
        }
        return false; // Room is free
    }

    void displayAvailableRooms() {
        cout << "Available rooms:\n";
        for (int i = 1; i <= 35; ++i) {
            if (!isRoomAllocated(i)) {
                cout << "Room " << i << " is available.\n";
            }
        }
    }

    void displayPatientByID(int id) {
        Patient* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                cout << "\n--- Patient Details ---\n";
                cout << "ID: " << temp->id << "\n";
                cout << "Name: " << temp->name << "\n";
                cout << "CNIC: " << temp->cnic << "\n";
                cout << "Age: " << temp->age << "\n";
                cout << "Gender: " << temp->gender << "\n";
                cout << "Contact: " << temp->contact << "\n";
                cout << "Address: " << temp->address << "\n";
                cout << "Admission Date: " << temp->admissionDate << "\n";
                cout << "Doctor Assigned: " << temp->doctorAssigned << "\n";
                cout << "Medical History: " << temp->medicalHistory << "\n";
                cout << "Current Illness: " << temp->currentIllness << "\n";
                cout << "Treatment Plan: " << temp->treatmentPlan << "\n";
                cout << "Room Number: " << temp->roomNumber << "\n";
                cout << "Billing Amount: " << temp->billingAmount << "\n";
                cout << "Priority: " << temp->priority << "\n";
                cout << "Emergency: " << (temp->emergency ? "Yes" : "No") << "\n";
                cout << "Condition: " << temp->condition << "\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Patient with ID " << id << " not found.\n";
    }

    void displayBill(int id) {
        Patient* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                cout << "\n--- Billing Details ---\n";
                cout << "Patient ID: " << temp->id << "\n";
                cout << "Patient Name: " << temp->name << "\n";
                cout << "Room Number: " << temp->roomNumber << "\n";
                cout << "Billing Amount: " << temp->billingAmount << "\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Patient with ID " << id << " not found.\n";
    }

    void findPatientByCNIC(string cnic) {
        Patient* temp = head;
        while (temp != nullptr) {
            if (temp->cnic == cnic) {
                displayPatientByID(temp->id); // Reuse the display function
                return;
            }
            temp = temp->next;
        }
        cout << "No patient found with CNIC " << cnic << ".\n";
    }

    void deletePatientByID(int id) {
        Patient* temp = head;
        Patient* prev = nullptr;

        while (temp != nullptr && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Patient with ID " << id << " not found.\n";
            return;
        }

        if (prev == nullptr) {
            head = temp->next; // Deleting the head
        } else {
            prev->next = temp->next; // Bypass the node to delete it
        }

        delete temp;
        cout << "Patient with ID " << id << " deleted successfully.\n";
        // Optionally, you could implement a function to rewrite the file after deletion
    }

    void displayPatients() {
        Patient* temp = head;
        cout << "\n--- All Patients ---\n";
        if (temp == nullptr) {
            cout << "No patients found.\n";
            return;
        }
        while (temp != nullptr) {
            cout << "ID: " << temp->id << ", Name: " << temp->name << ", Room: " << temp->roomNumber << "\n";
            temp = temp->next;
        }
    }

    void savePatientToFile(Patient* patient) {
        ofstream outFile("patients.txt", ios::app);
        if (outFile) {
            outFile << patient->id << ","
                    << patient->name << ","
                    << patient->cnic << ","
                    << patient->age << ","
                    << patient->gender << ","
                    << patient->contact << ","
                    << patient->address << ","
                    << patient->admissionDate << ","
                    << patient->doctorAssigned << ","
                    << patient->medicalHistory << ","
                    << patient->currentIllness << ","
                    << patient->treatmentPlan << ","
                    << patient->roomNumber << ","
                    << patient->billingAmount << ","
                    << patient->priority << ","
                    << patient->emergency << ","
                    << patient->condition << "\n";
            outFile.close();
        } else {
            cout << "Error opening file for writing.\n";
        }
    }

    void loadPatients() {
        ifstream inFile("patients.txt");
        if (inFile) {
            string line;
            while (getline(inFile, line)) {
                Patient* newPatient = new Patient;
                stringstream ss(line);
                string token;

                getline(ss, token, ','); newPatient->id = stoi(token);
                getline(ss, newPatient->name, ',');
                getline(ss, newPatient->cnic, ',');
                getline(ss, token, ','); newPatient->age = stoi(token);
                getline(ss, newPatient->gender, ',');
                getline(ss, newPatient->contact, ',');
                getline(ss, newPatient->address, ',');
                getline(ss, newPatient->admissionDate, ',');
                getline(ss, newPatient->doctorAssigned, ',');
                getline(ss, newPatient->medicalHistory, ',');
                getline(ss, newPatient->currentIllness, ',');
                getline(ss, newPatient->treatmentPlan, ',');
                getline(ss, token, ','); newPatient->roomNumber = stoi(token);
                getline(ss, token, ','); newPatient->billingAmount = stod(token);
                getline(ss, newPatient->priority, ',');
                getline(ss, token, ','); newPatient->emergency = (token == "1");
                getline(ss, newPatient->condition, ',');

                newPatient->next = head;
                head = newPatient;
            }
            inFile.close();
        } else {
            cout << "Error opening file for reading.\n";
        }
    }

    ~HospitalManagement() {
        while (head) {
            Patient* temp = head;
            head = head->next;
            delete temp;
        }
    }
};


// Display main menu for high-level users
void displayMenuH(int selectedOption) {
    system("cls");
    cout << "\n\n\t\t\tWelcome to Medical Management System\n\n";
    if (selectedOption == 0) {
        cout << "\t> 1. High-Level User\n";
        cout << "\t  2. Local User\n";
    } else {
        cout << "\t  1. High-Level User\n";
        cout << "\t> 2. Local User\n";
    }
    cout << "\n\tUse arrow keys to navigate and press Enter to select.";
}

// Countdown timer function
void countdown() {
    cout << setw(40) << setfill(' ') << "This tab will Open in 10 Seconds...\n\n";
    for (int i = 1; i <= 10; i++) {
        cout << i << "\t";
        Sleep(300);
    }
    system("cls");
}

// User class for managing individual users
class User {
private:
    string username;
    string password;

public:
    User() {}

    User(string& username, string& password) : username(username), password(password) {}

    void saveToFile(const string& filename) {
        ofstream file(filename, ios::out);
        if (file.is_open()) {
            file << username << "\n" << password << "\n";
            file.close();
        }
    }

    bool loadFromFile(const string& filename) {
        ifstream file(filename, ios::in);
        if (file.is_open()) {
            getline(file, username);
            getline(file, password);
            file.close();
            return true;
        }
        return false;
    }

    void createID() {
        cout << "\n\t*Create your user id*" << endl;
        cout << "\tEnter User Name: ";
        cin >> username;

        char ch;
        password.clear();
        cout << "Enter password: ";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b' && !password.empty()) {
                password.pop_back();
                cout << "\b \b";
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        system("cls");
        cout << "\n\tYour ID is creating..........." << endl;
        for (int i = 0; i < 5; ++i) {
            cout << ".";
            Sleep(500);
        }
        system("cls");
    }

    bool validate(const string& inputUsername, const string& inputPassword) {
        return (username == inputUsername && password == inputPassword);
    }

    void setPassword(const string& newPassword) {
        password = newPassword;
    }
};
 HospitalManagement hm;

// Login system class for managing login operations
class LoginSystem {
private:
    User highLevelUser;
    User localUser;

public:
    LoginSystem() {}

    void createIDsForLevel(int levelChoice) {
        if (levelChoice == 1) {
            highLevelUser.createID();
            highLevelUser.saveToFile("high_level_user.txt");
        } else if (levelChoice == 2) {
            localUser.createID();
            localUser.saveToFile("local_user.txt");
        } else {
            cout << "\n\t\tInvalid choice. Please choose either 1 or 2.\n";
        }
    }

    void startLogin(int levelChoice) {
        bool loginSuccess = false;
        User& currentUser = (levelChoice == 1) ? highLevelUser : localUser;
        string filename = (levelChoice == 1) ? "high_level_user.txt" : "local_user.txt";

        if (!currentUser.loadFromFile(filename)) {
            cout << "Error loading user data.\n";
            return;
        }

        do {
        	      	       		cout<<"\n\n";
cout<<setw(20)<<setfill(' ')<<""<<"   _____                _   _                                                 _   \n";
cout<<setw(20)<<setfill(' ')<<""<<"  / ____|              | | (_)                 /\\                            | |  \n";
cout<<setw(20)<<setfill(' ')<<""<<" | |     _ __ ___  __ _| |_ _ _ __   __ _     /  \\   ___ ___ ___  _   _ _ __ | |_ \n";
cout<<setw(20)<<setfill(' ')<<""<<" | |    | '__/ _ \\/ _` | __| | '_ \\ / _` |   / /\\ \\ / __/ __/ _ \\| | | | '_ \\| __|\n";
cout<<setw(20)<<setfill(' ')<<""<<" | |____| | |  __/ (_| | |_| | | | | (_| |  / ____ \\ (_| (_| (_) | |_| | | | | |_ \n";
cout<<setw(20)<<setfill(' ')<<""<<"  \\_____|_|  \\___|\\__,_|\\__|_|_| |_|\\__, | /_/    \\_\\___\\___\\___/ \\__,_|_| |_|\\__|\n";
cout<<setw(20)<<setfill(' ')<<""<<"                                     __/ |                                        \n";
cout<<setw(20)<<setfill(' ')<<"";   

            string username, password;
            cout << "\n\tEnter User Name: ";
            cin >> username;

            char ch;
            password.clear();// it is clearing 
            cout << "Enter password: ";
            while ((ch = _getch()) != '\r') {
                if (ch == '\b' && !password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                } else {
                    password.push_back(ch);
                    cout << '*';
                }
            }
            cout << endl;
            system("cls");

            if (currentUser.validate(username, password)) {
                cout << "\n\t\tLogin Successful\n";
                loginSuccess = true;
            } else {
                cout << "\n\t\tIncorrect Username or Password\n";
                cout << "\n\t\tPress any key to try again or press 'F' to reset password...\n";
                char choice = _getch();
                if (choice == 'F' || choice == 'f') {
                    resetPassword(levelChoice);
                }
            }
        } while (!loginSuccess);

        if (levelChoice == 1) highLevelFunctionality();
        else if (levelChoice == 2) localSystemFunctionality();
    }

    void resetPassword(int levelChoice) {
        User& currentUser = (levelChoice == 1) ? highLevelUser : localUser;
        string filename = (levelChoice == 1) ? "high_level_user.txt" : "local_user.txt";

        string newPass;
        cout << "\n\tEnter new password: ";
        cin >> newPass;
        currentUser.setPassword(newPass);
        currentUser.saveToFile(filename);
        cout << "\n\tPassword reset successfully!\n";
    }

    void highLevelFunctionality() {
        countdown();
        cout << "\n\n\t\t\tWelcome, High-Level User!\n";
         

    // Example usage
    int choice;
    while (true) {
        cout << "\n--- Hospital Patient Management System ---\n";
       
        cout << "2. Display All Patients\n";
        
        cout << "4. Find Patient by CNIC\n";
       
        cout << "6. Display Bill\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
          
        } else if (choice == 2) {
            hm.displayPatients();
        } else if (choice == 3) {
           
        } else if (choice == 4) {
            string cnic;
            cout << "Enter CNIC: ";
            cin >> cnic;
            hm.findPatientByCNIC(cnic);
        } else if (choice == 5) {
           
        } else if (choice == 6) {
            int id;
            cout << "Enter Patient ID for billing: ";
            cin >> id;
            hm.displayBill(id);
        } else if (choice == 7) {
            break; // Exit
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
        // Additional high-level functionality can be implemented here
    }

    void localSystemFunctionality() {
        countdown();
        cout << "\n\n\t\t\tWelcome, Local System User!\n";
        

    // Example usage
    int choice;
    while (true) {
        cout << "\n--- Hospital Patient Management System ---\n";
        cout << "1. Add Patient\n";
        cout << "3. Find Patient by ID\n";
       
        cout << "5. Delete Patient by ID\n";
        
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, age, roomNumber;
            double billingAmount;
            string name, cnic, gender, contact, address, admissionDate, doctorAssigned, medicalHistory,
                   currentIllness, treatmentPlan, priority, condition;
            bool emergency;

            cout << "Enter Patient ID: ";
            cin >> id;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter CNIC: ";
            getline(cin, cnic);
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Gender: ";
            cin >> gender;
            cout << "Enter Contact: ";
            cin >> contact;
            cout << "Enter Address: ";
            cin.ignore();
            getline(cin, address);
            cout << "Enter Admission Date: ";
            getline(cin, admissionDate);
            cout << "Enter Doctor Assigned: ";
            getline(cin, doctorAssigned);
            cout << "Enter Medical History: ";
            getline(cin, medicalHistory);
            cout << "Enter Current Illness: ";
            getline(cin, currentIllness);
            cout << "Enter Treatment Plan: ";
            getline(cin, treatmentPlan);
            cout << "Enter Room Number (1-35): ";
            cin >> roomNumber;
            cout << "Enter Billing Amount: ";
            cin >> billingAmount;
            cout << "Enter Priority: ";
            cin >> priority;
            cout << "Is this an emergency case (1 for Yes, 0 for No): ";
            cin >> emergency;
            cout << "Enter Condition: ";
            cin.ignore();
            getline(cin, condition);

            hm.addPatient(id, name, cnic, age, gender, contact, address, admissionDate, doctorAssigned,
                          medicalHistory, currentIllness, treatmentPlan, roomNumber, billingAmount,
                          priority, emergency, condition);
        } else if (choice == 2) {
           
        } else if (choice == 3) {
            int id;
            cout << "Enter Patient ID: ";
            cin >> id;
            hm.displayPatientByID(id);
        } else if (choice == 4) {
          
        } else if (choice == 5) {
            int id;
            cout << "Enter Patient ID to delete: ";
            cin >> id;
            hm.deletePatientByID(id);
        } else if (choice == 6) {
          
        } else if (choice == 7) {
            break; // Exit
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
        // Additional local system functionality can be implemented here
    }
};

// Loading bar function
void loadingBar() {
    char a = 177, b = 219;
    cout << "\n\n\t\t\t\t\t\t Loading...\n\n";
    cout << "\t\t\t\t\t";
    for (int i = 0; i < 26; i++) cout << a;
    cout << "\r\t\t\t\t\t";
    for (int i = 0; i < 26; i++) {
        cout << b;
        Sleep(50);
    }
    system("cls");
}

// Main function for user interface
int main() {
    LoginSystem loginSystem;
    int levelChoice = 0;
    char key;

    do {
    	 cout << " __          __  _                            " << endl;
    cout<<"\t\t\t\t  "  << " \\ \\        / / | |                           " << endl;
    cout<<"\t\t\t\t  "  << "  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___   " << endl;
    cout<<"\t\t\t\t  "  << "   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\  " << endl;
    cout<<"\t\t\t\t  " << "    \\  /\\  /  __/ | (_| (_) | | | | | |  __/  " << endl;
   	cout<<"\t\t\t\t " << "     \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|  " << endl;
        loadingBar();
        int selectedOption = 0;

        // Navigate options with arrow keys
        while (true) {
            displayMenuH(selectedOption);  // Display the menu with the current selected option
            
            key = _getch();  // Wait for key press
            
            if (key == 72) {  // Up arrow key
                selectedOption = (selectedOption - 1 + 2) % 2;
            } else if (key == 80) {  // Down arrow key
                selectedOption = (selectedOption + 1) % 2;
            } else if (key == 13) {  // Enter key
                levelChoice = selectedOption + 1;
                break;
            }
        }

        system("cls");  // Clear the screen after selection
    } while (levelChoice != 1 && levelChoice != 2);

    // Proceed with selected level
    loginSystem.createIDsForLevel(levelChoice);
    loginSystem.startLogin(levelChoice);

    return 0;
}
