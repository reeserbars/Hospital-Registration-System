#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

struct Patient {
	int id;
	string name;
	int age;
	string gender;
	string contact;
	string medicalHistory;
};

//function prototypes
int getIntInput(string prompt);
void addPatient();
vector<Patient> loadPatients();
void savePatients(const vector<Patient>& patients);
void editPatientById();
void deletePatientById();
void searchPatientById();
void displayAllPatients();
void menu();

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

// Function to display all patients
void displayAllPatients() {
	vector<Patient> patients = loadPatients();
	if (patients.empty()) {
		cout << "No patient records found." << endl;
	} else {
		for (const auto& patient : patients) {
			cout << "| ID: " << patient.id << "\n| Name: " << patient.name << "\n| Age: " << patient.age
				 << "\n| Gender: " << patient.gender << "\n| Contact: " << patient.contact
				 << "\n| Medical History: " << patient.medicalHistory << endl << endl;
		}
	}
}

// Function to search for a patient by ID
void searchPatientById() {
	int id = getIntInput("Enter the ID of the patient to search: ");
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
			getline(cin, patient.name);  // Correctly uses getline for name

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

vector<Patient> loadPatients() {
	vector<Patient> patients;
	ifstream file("patients.csv");

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			stringstream ss(line);
			Patient patient;
			string age, id;

			getline(ss, id, ',');
			getline(ss, patient.name, ',');
			getline(ss, age, ',');
			getline(ss, patient.gender, ',');
			getline(ss, patient.contact, ',');
			getline(ss, patient.medicalHistory, ',');

			patient.id = stoi(id);
			patient.age = stoi(age);
			patients.push_back(patient);
		}
		file.close();
	} else {
		cout << "Error opening file for reading." << endl;
	}
	return patients;
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

int getIntInput(string prompt) {
	int value;
	while (true) {
		cout << prompt;
		if (cin >> value) break;
		else {
			cout << "Please enter a valid number." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
	}
	cin.ignore();
	return value;
}