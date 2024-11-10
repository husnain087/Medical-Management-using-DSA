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

struct Doctor {
    string name;
    string specialization;
    int yearsOfExperience;
    string qualifications;
    string contactInfo;
};

// Function to add a new doctor profile
void addDoctorProfile() {
    Doctor doctor;
    cout << "Enter Doctor's Name: ";
    cin.ignore();
    getline(cin, doctor.name);

    cout << "Enter Specialization: ";
    getline(cin, doctor.specialization);

    cout << "Enter Years of Experience: ";
    cin >> doctor.yearsOfExperience;
    cin.ignore();

    cout << "Enter Qualifications: ";
    getline(cin, doctor.qualifications);

    cout << "Enter Contact Info: ";
    getline(cin, doctor.contactInfo);

    ofstream file("doctors.txt", ios::app);  // Open in append mode
    if (file.is_open()) {
        file << doctor.name << "|" << doctor.specialization << "|" 
             << doctor.yearsOfExperience << "|" << doctor.qualifications 
             << "|" << doctor.contactInfo << "\n";
        file.close();
        cout << "Doctor profile added successfully.\n";
    } else {
        cout << "Error opening file.\n";
    }
}

// Function to view all doctor profiles
void viewDoctorProfiles() {
    ifstream file("doctors.txt");
    if (file.is_open()) {
        string line;
        cout << "\n--- Doctor Profiles ---\n";
        while (getline(file, line)) {
            // Split each line by '|'
            string name, specialization, years, qualifications, contact;
            size_t pos = 0;
            
            pos = line.find('|');
            name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            specialization = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            years = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            qualifications = line.substr(0, pos);
            line.erase(0, pos + 1);

            contact = line;

            // Display doctor profile
            cout << "Name: " << name << "\nSpecialization: " << specialization 
                 << "\nYears of Experience: " << years 
                 << "\nQualifications: " << qualifications 
                 << "\nContact Info: " << contact << "\n\n";
        }
        file.close();
    } else {
        cout << "Error opening file.\n";
    }
}

// Function to update an existing doctor's profile
void updateDoctorProfile() {
    string searchName;
    cout << "Enter the name of the doctor to update: ";
    cin.ignore();
    getline(cin, searchName);

    ifstream file("doctors.txt");
    ofstream tempFile("temp.txt");  // Temporary file to store updated data

    bool found = false;
    if (file.is_open() && tempFile.is_open()) {
        string line;
        while (getline(file, line)) {
            Doctor doctor;
            size_t pos = 0;
            
            pos = line.find('|');
            doctor.name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            doctor.specialization = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            doctor.yearsOfExperience = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find('|');
            doctor.qualifications = line.substr(0, pos);
            line.erase(0, pos + 1);

            doctor.contactInfo = line;

            if (doctor.name == searchName) {
                found = true;
                cout << "\nUpdating profile for " << doctor.name << ":\n";
                cout << "Enter New Specialization: ";
                getline(cin, doctor.specialization);

                cout << "Enter New Years of Experience: ";
                cin >> doctor.yearsOfExperience;
                cin.ignore();

                cout << "Enter New Qualifications: ";
                getline(cin, doctor.qualifications);

                cout << "Enter New Contact Info: ";
                getline(cin, doctor.contactInfo);
            }

            tempFile << doctor.name << "|" << doctor.specialization << "|"
                     << doctor.yearsOfExperience << "|" << doctor.qualifications
                     << "|" << doctor.contactInfo << "\n";
        }

        file.close();
        tempFile.close();

        // Replace original file with updated temp file
        remove("doctors.txt");
        rename("temp.txt", "doctors.txt");

        if (found) {
            cout << "Doctor profile updated successfully.\n";
        } else {
            cout << "Doctor not found.\n";
        }
    } else {
        cout << "Error opening file.\n";
    }
}

// Main function to run the Doctor Profile Management module


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
struct Staff {
    string name;
    string occupation;
    int age;
    string contact;
    Staff* next;  // Pointer to the next staff profile
};

// Initialize the circular linked list
Staff* head = nullptr;

// Function to save staff data to a file
void saveToFile() {
    ofstream file("staff_profiles.txt");
    if (!file) {
        cout << "Error opening file for writing.\n";
        return;
    }
    
    Staff* temp = head;
    if (temp) {
        do {
            file << temp->name << "\n" << temp->occupation << "\n" << temp->age << "\n" << temp->contact << "\n";
            temp = temp->next;
        } while (temp != head);
    }
    file.close();
    cout << "Staff profiles saved to file successfully.\n";
}

// Function to load staff data from a file
void loadFromFile() {
    ifstream file("staff_profiles.txt");
    if (!file) {
        cout << "Error opening file for reading.\n";
        return;
    }

    string name, occupation, contact;
    int age;
    while (getline(file, name) && getline(file, occupation) && file >> age && file.ignore() && getline(file, contact)) {
        Staff* newStaff = new Staff{name, occupation, age, contact, nullptr};
        if (!head) {
            head = newStaff;
            newStaff->next = head;
        } else {
            Staff* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = newStaff;
            newStaff->next = head;
        }
    }
    file.close();
    cout << "Staff profiles loaded from file successfully.\n";
}

// Function to add a new staff profile
void addStaffProfile() {
    Staff* newStaff = new Staff();
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, newStaff->name);
    cout << "Enter occupation (Nurse, Receptionist, Lab Technician, Cleaner, Security): ";
    getline(cin, newStaff->occupation);
    cout << "Enter age: ";
    cin >> newStaff->age;
    cout << "Enter contact number: ";
    cin.ignore();
    getline(cin, newStaff->contact);

    if (!head) { // If list is empty
        head = newStaff;
        newStaff->next = head;
    } else {
        Staff* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        temp->next = newStaff;
        newStaff->next = head;
    }
    cout << "Staff profile added successfully!\n";
    saveToFile();  // Save changes to file
}

// Function to view all staff profiles
void viewStaffProfiles() {
    if (!head) {
        cout << "No staff profiles available.\n";
        return;
    }
    Staff* temp = head;
    cout << "\n--- Hospital Staff Profiles ---\n";
    do {
        cout << "Name: " << temp->name << ", Occupation: " << temp->occupation
             << ", Age: " << temp->age << ", Contact: " << temp->contact << endl;
        temp = temp->next;
    } while (temp != head);
}

// Function to update a staff profile
void updateStaffProfile() {
    if (!head) {
        cout << "No staff profiles to update.\n";
        return;
    }
    string name;
    cout << "Enter the name of the staff to update: ";
    cin.ignore();
    getline(cin, name);

    Staff* temp = head;
    do {
        if (temp->name == name) {
            cout << "Enter new age: ";
            cin >> temp->age;
            cout << "Enter new contact number: ";
            cin.ignore();
            getline(cin, temp->contact);
            cout << "Staff profile updated successfully!\n";
            saveToFile();  // Save changes to file
            return;
        }
        temp = temp->next;
    } while (temp != head);
    cout << "Staff profile not found.\n";
}

// Function to delete a staff profile
void deleteStaffProfile() {
    if (!head) {
        cout << "No staff profiles to delete.\n";
        return;
    }
    string name;
    cout << "Enter the name of the staff to delete: ";
    cin.ignore();
    getline(cin, name);

    Staff* temp = head;
    Staff* prev = nullptr;

    // Check if the node to delete is the head node
    if (head->name == name) {
        if (head->next == head) { // Only one node in the list
            delete head;
            head = nullptr;
        } else {
            while (temp->next != head) { // Find the last node
                temp = temp->next;
            }
            temp->next = head->next;
            delete head;
            head = temp->next;
        }
        cout << "Staff profile deleted successfully!\n";
        saveToFile();  // Save changes to file
        return;
    }

    // For non-head nodes
    do {
        prev = temp;
        temp = temp->next;
        if (temp->name == name) {
            prev->next = temp->next;
            delete temp;
            cout << "Staff profile deleted successfully!\n";
            saveToFile();  // Save changes to file
            return;
        }
    } while (temp != head);

    cout << "Staff profile not found.\n";
}

// Main menu for managing staff profiles
void manageStaffModule() {
    loadFromFile();  // Load data from file at the start
    int choice;
    do {
        cout << "\n--- Hospital Low-Level Staff Management ---\n";
        cout << "1. Add Staff Profile\n";
        cout << "2. View Staff Profiles\n";
        cout << "3. Update Staff Profile\n";
        cout << "4. Delete Staff Profile\n";
        cout << "5. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            addStaffProfile();
        } else if (choice == 2) {
            viewStaffProfiles();
        } else if (choice == 3) {
            updateStaffProfile();
        } else if (choice == 4) {
            deleteStaffProfile();
        } else if (choice == 5) {
            cout << "Exiting to Main Menu...\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (5);
}
// Structure to represent a patient
struct Patients{
    string name;
    int age;
    string issue;
    int priority; // Priority for emergency cases (higher = more urgent)
    Patients* next; // Pointer to the next patient node
};

// Regular queue class for regular patients
class RegularQueue {
private:
    Patients* front; // Front of the queue
    Patients* rear;  // Rear of the queue

public:
    RegularQueue() : front(nullptr), rear(nullptr) {}

    // Add a patient to the regular queue
    void enqueue(const string& name, int age, const string& issue) {
        Patients* newPatient = new Patients{name, age, issue, 0, nullptr};

        // If queue is empty, front and rear are the new patient
        if (rear == nullptr) {
            front = rear = newPatient;
        } else {
            rear->next = newPatient;
            rear = newPatient;
        }

        // Write patient data to file
        ofstream file("que.txt", ios::app);
        file << "Regular," << name << "," << age << "," << issue << "\n";
        file.close();

        cout << "Regular appointment added for " << name << ".\n";
    }

    // Remove and return the patient at the front of the queue
    Patients* dequeue() {
        if (front == nullptr) return nullptr; // Queue is empty

        Patients* patientToServe = front;
        front = front->next;

        // If front becomes null, set rear to null as well
        if (front == nullptr) rear = nullptr;

        return patientToServe;
    }

    // Check if the regular queue is empty
    bool isEmpty() const {
        return front == nullptr;
    }

    // Getter for front to display regular patients
    Patients* getFront() const {
        return front;
    }
};

// Priority queue class for emergency patients
class EmergencyQueue {
private:
    Patients* head; // Head of the list

public:
    EmergencyQueue() : head(nullptr) {}

    // Function to access the head for displaying patients
    Patients* getHead() const {
        return head;
    }

    // Insert patient based on priority (higher priority patients come first)
    void enqueue(const string& name, int age, const string& issue, int priority) {
        Patients* newPatient = new Patients{name, age, issue, priority, nullptr};

        if (head == nullptr || head->priority < priority) {
            // Insert at the beginning if list is empty or new patient has highest priority
            newPatient->next = head;
            head = newPatient;
        } else {
            // Insert based on priority
            Patients* temp = head;
            while (temp->next != nullptr && temp->next->priority >= priority) {
                temp = temp->next;
            }
            newPatient->next = temp->next;
            temp->next = newPatient;
        }

        // Write patient data to file
        ofstream file("que.txt", ios::app);
        file << "Emergency," << name << "," << age << "," << issue << "," << priority << "\n";
        file.close();

        cout << "Emergency appointment added for " << name << " with priority " << priority << ".\n";
    }

    // Remove and return the patient with the highest priority
    Patients* dequeue() {
        if (head == nullptr) return nullptr; // Queue is empty

        Patients* patientToServe = head;
        head = head->next;
        return patientToServe;
    }

    // Check if the emergency queue is empty
    bool isEmpty() const {
        return head == nullptr;
    }
};

// Function to serve the next patient, prioritizing emergency cases
void serveNextPatient(EmergencyQueue& emergencyQueue, RegularQueue& regularQueue) {
    Patients* patientToServe;

    if (!emergencyQueue.isEmpty()) {
        // Serve the emergency patient with the highest priority
        patientToServe = emergencyQueue.dequeue();
        cout << "Serving emergency patient: " << patientToServe->name
             << " (Age: " << patientToServe->age
             << ", Issue: " << patientToServe->issue
             << ", Priority: " << patientToServe->priority << ")\n";
    } else if (!regularQueue.isEmpty()) {
        // Serve the next regular patient if no emergency patients
        patientToServe = regularQueue.dequeue();
        cout << "Serving regular patient: " << patientToServe->name
             << " (Age: " << patientToServe->age
             << ", Issue: " << patientToServe->issue << ")\n";
    } else {
        cout << "No patients in the queue.\n";
        return;
    }

    delete patientToServe; // Free memory after serving the patient
}

// Function to display all patients in both queues
void displayAllPatients(EmergencyQueue& emergencyQueue, RegularQueue& regularQueue) {
    cout << "\n--- Emergency Patients ---\n";
    Patients* temp = emergencyQueue.getHead(); // Use getHead() to access the head of the emergency queue
    while (temp != nullptr) {
        cout << "Name: " << temp->name << ", Age: " << temp->age << ", Issue: " << temp->issue
             << ", Priority: " << temp->priority << endl;
        temp = temp->next;
    }

    cout << "\n--- Regular Patients ---\n";
    temp = regularQueue.getFront();
    while (temp != nullptr) {
        cout << "Name: " << temp->name << ", Age: " << temp->age << ", Issue: " << temp->issue << endl;
        temp = temp->next;
    }
}

// Load patients from file at startup
void loadPatientsFromFile(EmergencyQueue& emergencyQueue, RegularQueue& regularQueue) {
    ifstream file("que.txt");
    if (!file) return; // File doesn't exist, no data to load

    string line, type, name, issue;
    int age, priority;

    while (getline(file, line)) {
        istringstream iss(line);
        getline(iss, type, ',');
        getline(iss, name, ',');
        iss >> age;
        iss.ignore();
        getline(iss, issue, ',');

        if (type == "Emergency") {
            iss >> priority;
            emergencyQueue.enqueue(name, age, issue, priority);
        } else {
            regularQueue.enqueue(name, age, issue);
        }
    }
    file.close();
}

// Main menu function to handle user input
void appointmentSystem() {
    RegularQueue regularQueue;
    EmergencyQueue emergencyQueue;

    // Load existing patients from file
    loadPatientsFromFile(emergencyQueue, regularQueue);

    int choice;
    do {
        cout << "\n--- Hospital Appointment System ---\n";
        cout << "1. Add Regular Patient\n";
        cout << "2. Add Emergency Patient\n";
        cout << "3. Serve Next Patient\n";
        cout << "4. Display All Patients\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, issue;
            int age;
            cout << "Enter patient's name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter patient's age: ";
            cin >> age;
            cout << "Enter patient's issue: ";
            cin.ignore();
            getline(cin, issue);

            // Add patient to the regular queue
            regularQueue.enqueue(name, age, issue);

        } else if (choice == 2) {
            string name, issue;
            int age, priority;
            cout << "Enter emergency patient's name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter patient's age: ";
            cin >> age;
            cout << "Enter patient's issue: ";
            cin.ignore();
            getline(cin, issue);
            cout << "Enter emergency priority level (1-10, 10 = highest): ";
            cin >> priority;

            // Add patient to the emergency queue
            emergencyQueue.enqueue(name, age, issue, priority);

        } else if (choice == 3) {
            // Serve the next patient based on priority
            serveNextPatient(emergencyQueue, regularQueue);

        } else if (choice == 4) {
            // Display all patients in both queues
            displayAllPatients(emergencyQueue, regularQueue);

        } else if (choice == 5) {
            cout << "Exiting the system.\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

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

    int choice = 2; // Start with the first option highlighted
    int maxOption = 7; // Maximum number of options
    char key;

    while (true) {
        system("cls"); // Clear the console
        cout << "\n--- Hospital Patient Management System ---\n";

        // Display menu options with arrow key navigation
        for (int i = 2; i <= maxOption; i++) {
            if (i == choice) {
                cout << "> "; // Highlight the current choice
            } else {
                cout << "  ";
            }
            switch (i) {
                case 2: cout << "Display All Patients\n"; break;
                case 4: cout << "Find Patient by CNIC\n"; break;
                case 6: cout << "Display Bill\n"; break;
                case 7: cout << "Enter In Doctor Section\n"; break;
            }
        }

        key = _getch(); // Get user input for navigation

        if (key == 72) { // Up arrow key
            choice--;
            if (choice < 2) choice = 7; // Loop back to the last option
        } else if (key == 80) { // Down arrow key
            choice++;
            if (choice > 7) choice = 2; // Loop back to the first option
        } else if (key == '\r') { // Enter key
            if (choice == 2) {
                // Display all patients
                hm.displayPatients();
                system("pause"); // Pause to view output before clearing screen
            } else if (choice == 4) {
                // Find patient by CNIC
                string cnic;
                cout << "Enter CNIC: ";
                cin >> cnic;
                hm.findPatientByCNIC(cnic);
                system("pause"); // Pause to view output before clearing screen
            } else if (choice == 6) {
                // Display bill
                int id;
                cout << "Enter Patient ID for billing: ";
                cin >> id;
                hm.displayBill(id);
                system("pause"); // Pause to view output before clearing screen
            } else if (choice == 7) {system("cls");
                
   

        int choice;
do {
    // Display menu options
    cout << "1. Add Doctor Profile\n";
    cout << "2. View Doctor Profiles\n";
    cout << "3. Update Doctor Profile\n";
    cout << "4. To Add in staff management\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        addDoctorProfile();
    } else if (choice == 2) {
        viewDoctorProfiles();
    } else if (choice == 3) {
        updateDoctorProfile();
    } else if (choice == 4) {
    	  manageStaffModule();  // Run the staff management module
      
    }
    else if(choice==5)
    {
    	 cout << "Exiting to Main Menu...\n";
            break;
	}
	 else{
        cout << "Invalid choice. Please try again.\n";
    }
} while (5); // Loop until the user chooses to exit

    }
}
            }
        }
    
    

    void localSystemFunctionality() {
        countdown();
        cout << "\n\n\t\t\tWelcome, Local System User!\n";
        

    // Example usage
    int choice;
    while (8) {
        cout << "\n--- Hospital Patient Management System ---\n";
        cout << "1. Add Patient\n";
        cout << "3. Find Patient by ID\n";
       
        cout << "5. Delete Patient by ID\n";
        
        cout << "7. To Appointement\n";
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
             appointmentSystem();
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
