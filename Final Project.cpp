#include <iostream>  // For input and output functions (cin, cout)
#include <fstream>   // For file handling (ifstream, ofstream)
#include <vector>    // For storing and managing lists of patients
#include <sstream>   // For parsing strings from file input
#include <string>    // For handling string data types
#include <iomanip>	 // for setw() so that the ui looks cooler
#include <cstdlib> // For system() also for making the ui look cooler because system("cls")

using namespace std;

struct Patient { //struct is a datatype that groups together related variables
	int id{};
	string name;
	int age{};
	string gender;
	string contact;
	string medicalHistory;
};

//function prototypes
void menu();                       // Displays main menu and handles user choice
void displayAllPatients();          // Displays all patients from the file
void searchPatient();           // Searches for a patient by their ID
void addPatient();                  // Adds a new patient to the file
void deletePatientById();           // Deletes a patient by their ID
void editPatientById();             // Edits patient information based on ID
void savePatients(const vector<Patient>& patients); // Saves patients to file
vector<Patient> loadPatients();     // Loads patient data from file
int getIntInput(const string& prompt); // Gets integer input with validation
char getGenderInput(const string& prompt); // like the getIntInput but for sextech
string getContactInput(const string& prompt); // getIntInput with extra steps

// Function to clear the screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
}

int main() {
	menu();
	return 0;
}

// Main menu function
void menu() {
    int choice;
    do {
        clearScreen(); // Clear the terminal
        cout << "\n\n---|| Hospital Registration System ||---\n\n";
        cout << "-- 1. Add Patient\n";
        cout << "-- 2. Search Patient\n";
        cout << "-- 3. Display All Patients\n";
        cout << "-- 4. Edit Patient by ID\n";
        cout << "-- 5. Delete Patient by ID\n";
        cout << "-- 6. Exit\n\n";

        choice = getIntInput("Enter your choice (1 - 6): \n");
        
        switch (choice) {
            case 1: addPatient(); break;
            case 2: searchPatient(); break;
            case 3: displayAllPatients(); break;
            case 4: editPatientById(); break;
            case 5: deletePatientById(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
        // Automatically clear the screen when returning to the menu
        if (choice != 6) clearScreen();
    } while (choice != 6);
}

//Input and validation for integers
int getIntInput(const string& prompt) { // & used again here because large strings go brr on memory, const so that no accidental modification
	int value;
	//infinite while loop until user gives valid input
	while (true) {
		cout << prompt; // outputs prompt strng that was given as arg by caller
		if (cin >> value) break; // read input and if statement and its code in one line. Yes you can do that. easy input validation didnt even need cin.fail()
		else {
			cout << "Please enter a valid number." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
	}
	cin.ignore(); // clears any extra input
	return value;
}

// Function to get valid gender input ('M' or 'F')
char getGenderInput(const string& prompt) {
    char gender;
    while (true) {
        cout << prompt; // Outputs the prompt message
        cin >> gender; // Read input from the user
        cin.ignore(); // Clears any extra input left in the buffer
        gender = toupper(gender); // Converts input to uppercase to standardize (M or F)

        if (gender == 'M' || gender == 'F') { // Check if input is valid (M or F)
            break; // Exit the loop if valid input is given
        } else {
            cout << "Please enter a valid gender (M/F)." << endl; // Error message if invalid
        }
    }
    return gender; // Return the valid gender input
}

// Function to get valid contact number input (just digits or numbers with a starting "+")
string getContactInput(const string& prompt) {
    string contact;
    while (true) {
        cout << prompt; // Outputs the prompt message
        getline(cin, contact); // Read input from the user as a string

        bool valid = true;

        // Check length and content based on whether the contact starts with a '+'
        if (contact[0] == '+') { // stuff that starts with +63
            if (contact.length() == 13) {
                for (int i = 1; i < contact.length(); i++) { // Skip '+' and check remaining digits 
                    if (!isdigit(contact[i])) {
                        valid = false;
                        break;
                    }
                }
            } else {
                valid = false;
            }
        } else {
            if (contact.length() == 11) { // for stuff that starts with 09
                for (char ch : contact) {
                    if (!isdigit(ch)) {
                        valid = false;
                        break;
                    }
                }
            } else {
                valid = false;
            }
        }

        if (valid) {
            break; // Exit loop if the contact number is valid
        } else {
            cout << "Please enter a valid contact number (11 digits or 12 with '+')." << endl;
        }
    }
    return contact; // Return the valid contact number
}

// Loads patient data from file
vector<Patient> loadPatients() {
	
	vector<Patient> patients; // defines array of patient structs named patients
	ifstream file("patients.csv");

	if (file.is_open()) { // if file open is successful aka if it exists or not
		string line;	//for holding each line of text
		while (getline(file, line)) { // loop continues until there are no more lines to read
			stringstream ss(line); // defines string streamvariable and sets it to the line
			Patient patient; // patient struct named patient
			string age, id; 


			//reads values from stringstream
			getline(ss, id, ',');	// its like cin but instead of input it reads from whatever string you set the stringstream to
			getline(ss, patient.name, ',');
			getline(ss, age, ',');
			getline(ss, patient.gender, ',');
			getline(ss, patient.contact, ',');
			getline(ss, patient.medicalHistory, ',');

			patient.id = stoi(id); //stoi means string to integer so its static cast but fancy
			patient.age = stoi(age);
			patients.push_back(patient); // puts the patient object into the patient info
		}
		file.close();
	} else { // if it cant find the file or something around that
		cout << "Error opening file for reading." << endl;
	}
	return patients; // returns the patient array to whatever called it
}

// Displays all patients from the csv file
void displayAllPatients() {
    string choice; // Variable to store user's choice to continue or stop
    do {
        clearScreen(); // Clear the screen for a clean UI
        vector<Patient> patients = loadPatients();

        if (patients.empty()) {
            cout << "\nNo patient records found." << endl;
        } else {
            cout << "\n--- All Patients ---\n\n";
            cout << left << setw(5) << "ID" 
                 << setw(20) << "Name" 
                 << setw(5) << "Age" 
                 << setw(10) << "Gender" 
                 << setw(15) << "Contact" 
                 << "Medical History" << endl;

            cout << string(60, '-') << endl; // Print a separator line

            for (const auto& patient : patients) {
                cout << left << setw(5) << patient.id 
                     << setw(20) << patient.name 
                     << setw(5) << patient.age 
                     << setw(10) << patient.gender 
                     << setw(15) << patient.contact 
                     << patient.medicalHistory << endl;
            }
        }

        // Ask if the user wants to view the list again
        cout << "\nDo you want to view the list again? (y/n): ";
        cin >> choice;

    } while (choice == "y" || choice == "Y");

    clearScreen(); // Clear the screen after exiting the loop
}

// Search for a patient by ID
void searchPatient() {
    string choice; // Variable to store user's choice to continue or stop
    do {
        clearScreen(); // Clear the screen for a clean UI
        int searchChoice = getIntInput("\n--- Search Patient ---\n1. By ID\n2. By Name\n\nEnter your choice (1 or 2): ");

        vector<Patient> patients = loadPatients();
        bool found = false;

        if (searchChoice == 1) {
            int id = getIntInput("\n--- Enter the ID of the patient to search: ");
            for (const auto& patient : patients) {
                if (patient.id == id) {
                    found = true;
                    cout << "\n-- Patient Found --\n";
                    cout << "| ID: " << patient.id 
                         << "\n| Name: " << patient.name 
                         << "\n| Age: " << patient.age
                         << "\n| Gender: " << patient.gender 
                         << "\n| Contact: " << patient.contact
                         << "\n| Medical History: " << patient.medicalHistory << endl;
                    break;
                }
            }
        } else if (searchChoice == 2) {
            string nameQuery;
            cout << "\n--- Enter part of the Name to search: ";
            cin.ignore(); // Clear the buffer
            getline(cin, nameQuery);

            cout << "\n-- Matching Patients --\n";

            for (const auto& patient : patients) {
                if (patient.name.find(nameQuery) != string::npos) { // Check if nameQuery is a substring
                    found = true;
                    cout << "| ID: " << patient.id 
                         << "\n| Name: " << patient.name 
                         << "\n| Age: " << patient.age
                         << "\n| Gender: " << patient.gender 
                         << "\n| Contact: " << patient.contact
                         << "\n| Medical History: " << patient.medicalHistory << "\n\n";
                }
            }
        } else {
            cout << "\nInvalid choice. Please try again.\n";
        }

        if (!found) {
            cout << "\n-- No matching patient found." << endl;
        }

        // Ask if the user wants to search for another patient
        cout << "\nDo you want to search for another patient? (y/n): ";
        cin >> choice;

    } while (choice == "y" || choice == "Y");

    clearScreen(); // Clear the screen after exiting the loop
}

// Save all patients to the file
void savePatients(const vector<Patient>& patients) {
	ofstream file("patients.csv");

	if (file.is_open()) {
		for (const auto& patient : patients) { // loop through the object array given in args
			// Write each patient's data to the file in CSV format
			file << patient.id << "," 
				<< patient.name << "," 
				<< patient.age << ","
				<< patient.gender << "," 
				<< patient.contact << "," 
				<< patient.medicalHistory << endl;
		}
		file.close();
	} else {
		cout << "\nError opening file for saving." << endl;
	}
}

// Function to delete a patient by ID
void deletePatientById() {
    string choice; // Variable to store user's choice to continue or stop
    do {
        clearScreen(); // Clear the screen for a clean UI
        int patientIdToDelete = getIntInput("\n---Enter the ID of the patient to delete: ");
        vector<Patient> patientList = loadPatients();
        bool isPatientFound = false;

        // Iterate through the list of patients to find the patient with the given ID
        for (auto patientIterator = patientList.begin(); patientIterator != patientList.end(); ++patientIterator) {
            if (patientIterator->id == patientIdToDelete) {
                isPatientFound = true;
                patientList.erase(patientIterator); // Delete the patient
                cout << "\n--Patient record deleted successfully!" << endl;
                break;
            }
        }

        // If patient was not found, display a message
        if (!isPatientFound) {
            cout << "\n--Patient with ID " << patientIdToDelete << " not found." << endl;
        }

        // Save the updated list of patients back to the file
        savePatients(patientList);

        // Ask if the user wants to delete another patient
        cout << "\nDo you want to delete another patient? (y/n): ";
        cin >> choice;

    } while (choice == "y" || choice == "Y");

    clearScreen(); // Clear the screen after exiting the loop
}

// Edit a patient by ID
void editPatientById() {
    int id = getIntInput("\n---Enter the ID of the patient to edit: ");
    vector<Patient> patients = loadPatients();
    bool found = false;

    for (auto& patient : patients) {
        if (patient.id == id) {
            found = true;
            int editChoice;
            do {
                clearScreen(); // Clear screen for each iteration
                cout << "\n--Editing patient: " << patient.name << "\n--Patient ID: " << patient.id << endl;
                cout << "- 1. Edit Name\n";
                cout << "- 2. Edit Age          | Current: " << patient.age << endl;
                cout << "- 3. Edit Gender       | Current: " << patient.gender << endl;
                cout << "- 4. Edit Contact      | Current: " << patient.contact << endl;
                cout << "- 5. Edit Medical History | Current: " << patient.medicalHistory << endl;
                cout << "- 6. Done/Back\n\n";

                editChoice = getIntInput("Enter your choice (1 - 6): \n");

                switch (editChoice) {
                    case 1: 
                        cout << "| New Name: "; 
                        getline(cin >> ws, patient.name); 
                        break;
                    case 2: 
                        patient.age = getIntInput("| New Age: "); 
                        break;
                    case 3: 
                        patient.gender = getGenderInput("| New Gender (M/F): "); 
                        break;
                    case 4: 
                        patient.contact = getContactInput("| New Contact: "); 
                        break;
                    case 5: 
                        cout << "| New Medical History: ";
                        getline(cin >> ws, patient.medicalHistory); 
                        break;
                    case 6: 
                        cout << "Updating...\n"; 
                        break;
                    default: 
                        cout << "Invalid choice. Please try again.\n";
                }
            } while (editChoice != 6);

            savePatients(patients);
            break;
        }
    }

    if (!found) {
        cout << "\n--Patient with ID " << id << " not found." << endl;
    }
}

// Adds new patient to the system
void addPatient() {
    ofstream file("patients.csv", ios::app); // Open file in append mode
    string choice;

    do {
        clearScreen(); // Clear the terminal for a clean UI
        Patient patient;
        int maxId = 0;
        ifstream infile("patients.csv");
        string line;

        // Find the highest existing patient ID
        while (getline(infile, line)) {
            stringstream ss(line);
            string id;
            getline(ss, id, ','); 
            int currentId = stoi(id);
            if (currentId > maxId) {
                maxId = currentId;
            }
        }
        infile.close();

        // Assign a new ID based on the highest found ID
        patient.id = maxId + 1;

        // Collect patient information
        cout << "--Adding a new patient--\n";
        cout << "| Enter Name: ";
        getline(cin >> ws, patient.name);
        patient.age = getIntInput("| Enter Age: ");
        patient.gender = getGenderInput("| Enter Gender (M/F): ");
        patient.contact = getContactInput("| Enter Contact: ");
        cout << "| Enter Medical History: ";
        getline(cin, patient.medicalHistory);

        // Write the new patient data to the file
        if (file.is_open()) {
            file << patient.id << ","
                 << patient.name << ","
                 << patient.age << ","
                 << patient.gender << ","
                 << patient.contact << ","
                 << patient.medicalHistory << endl;
            cout << "\n--Patient added successfully!\n";
        } else {
            cout << "\n--Error opening file for writing.\n";
        }

        // Ask if the user wants to add another patient
        cout << "\nDo you want to add another patient? (y/n): ";
        cin >> choice;

    } while (choice == "y" || choice == "Y");

    file.close(); // Close the file after all additions
}