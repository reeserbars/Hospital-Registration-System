#include <iostream>  // For input and output functions (cin, cout)
#include <fstream>   // For file handling (ifstream, ofstream)
#include <vector>    // For storing and managing lists of patients
#include <sstream>   // For parsing strings from file input
#include <string>    // For handling string data types

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
void searchPatientById();           // Searches for a patient by their ID
void addPatient();                  // Adds a new patient to the file
void deletePatientById();           // Deletes a patient by their ID
void editPatientById();             // Edits patient information based on ID
void savePatients(const vector<Patient>& patients); // Saves patients to file
vector<Patient> loadPatients();     // Loads patient data from file
int getIntInput(const string& prompt); // Gets integer input with validation

int main() {
	menu();
	return 0;
}

// Main menu function
void menu() {
	int choice;
	do {
		cout << "\nHospital Registration System\n";
		cout << "1. Add Patient\n";
		cout << "2. Search Patient by ID\n";
		cout << "3. Display All Patients\n";
		cout << "4. Edit Patient by ID\n";
		cout << "5. Delete Patient by ID\n";
		cout << "6. Exit\n";
		choice = getIntInput("Enter your choice: \n");

		switch (choice) {
			case 1: addPatient(); break;
			case 2: searchPatientById(); break;
			case 3: displayAllPatients(); break;
			case 4: editPatientById(); break;
			case 5: deletePatientById(); break;
			case 6: cout << "Exiting...\n"; break;
			default: cout << "Invalid choice. Please try again.\n";
		}
	} while (choice != 6);
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
	vector<Patient> patients = loadPatients();
	if (patients.empty()) {
		cout << "No patient records found." << endl;
	} else {
	
	// this is the equivalent its just fancier because i used a range based for loop
	// 	for (int i = 0; i < patients.size(); ++i) {
    // 		const Patient& patient = patients[i];  
	//
	// syntax: for (variable-that-holds-every-value-in-the-a-container : container-where-each-element-is-put-into-the-variable-every-iteration)
	//
	// auto is there so you dont have to specify the type for every variable since the patients array stores different datatypes 
	// 
	//& copies the value of the array to the current var, const so that we dont accidentally modify it, because using & directly affects the variable since it IS the variable,
	//its essentially another name for the variable. using it saves on memory because it doesnt make a copy of the original variable which would be pretty memory intensive
		for (const auto& patient : patients) { 
			cout << "| ID: " << patient.id 
				<< "\n| Name: " << patient.name 
				<< "\n| Age: " << patient.age
				 << "\n| Gender: " << patient.gender 
				 << "\n| Contact: " << patient.contact
				 << "\n| Medical History: " << patient.medicalHistory << endl << endl;
		}
	}
}

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

// Function to search for a patient by ID
void searchPatientById() {
	int id = getIntInput("Enter the ID of the patient to search: "); //calls get input
	vector<Patient> patients = loadPatients();
	bool found = false;

	for (const auto& patient : patients) {
		if (patient.id == id) {
			found = true;
			cout << "ID: " << patient.id << "\nName: " << patient.name << "\nAge: " << patient.age
				 << "\nGender: " << patient.gender << "\nContact: " << patient.contact
				 << "\nMedical History: " << patient.medicalHistory << endl;
			break;
		}
	}
	if (!found) {
		cout << "Patient with ID " << id << " not found." << endl;
	}
}

// Function to delete a patient by ID
void deletePatientById() {
	int id = getIntInput("Enter the ID of the patient to delete: ");
	vector<Patient> patients = loadPatients();
	bool found = false;

	for (auto it = patients.begin(); it != patients.end(); ++it) {
		if (it->id == id) {
			found = true;
			patients.erase(it);
			cout << "Patient record deleted successfully!" << endl;
			break;
		}
	}
	if (!found) {
		cout << "Patient with ID " << id << " not found." << endl;
	}
	savePatients(patients);
}

// Function to edit a patient by ID
void editPatientById() {
	int id = getIntInput("Enter the ID of the patient to edit: ");
	vector<Patient> patients = loadPatients();
	bool found = false;

	for (auto& patient : patients) {
		if (patient.id == id) {
			found = true;
			cout << "Editing patient " << patient.name << endl;

			cout << "New Name: ";
			getline(cin, patient.name);  // Correctly uses get line() for name

			patient.age = getIntInput("New Age: ");  // Uses getIntInput for age

			cout << "New Gender (M/F): ";
			getline(cin, patient.gender);

			cout << "New Contact: ";
			getline(cin, patient.contact);

			cout << "New Medical History: ";
			getline(cin, patient.medicalHistory);

			cout << "Patient record updated successfully!" << endl;
			break;
		}
	}

	if (!found) {
		cout << "Patient with ID " << id << " not found." << endl;
	}

	savePatients(patients);
}

// Function to save all patients to the file
void savePatients(const vector<Patient>& patients) {
	ofstream file("patients.csv");

	if (file.is_open()) {
		for (const auto& patient : patients) {
			file << patient.id << "," 
				<< patient.name << "," 
				<< patient.age << ","
				<< patient.gender << "," 
				<< patient.contact << "," 
				<< patient.medicalHistory << endl;
		}
		file.close();
	} else {
		cout << "Error opening file for saving." << endl;
	}
}

void addPatient() {
	ofstream file("patients.csv", ios::app);
	Patient patient;

	patient.id = getIntInput("Enter ID: ");
	cout << "Enter Name: ";
	getline(cin, patient.name);
	patient.age = getIntInput("Enter Age: ");
	cout << "Enter Gender (M/F): ";
	getline(cin, patient.gender);
	cout << "Enter Contact: ";
	getline(cin, patient.contact);
	cout << "Enter Medical History: ";
	getline(cin, patient.medicalHistory);

	if (file.is_open()) {
		file << patient.id << "," << patient.name << "," << patient.age << ","
			 << patient.gender << "," << patient.contact << "," << patient.medicalHistory << endl;
		cout << "Patient added successfully!" << endl;
	} else {
		cout << "Error opening file for writing." << endl;
	}
	file.close();
}

