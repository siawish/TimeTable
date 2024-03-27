#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <list>
#include <algorithm>
#include <memory> 
#include <fstream>
#include <limits>
#include <cctype>

using namespace std;

class Student {
public:
    string name;
    string section;
    string rollID; // Added roll ID for students

    void serialize(ofstream& out) const {
        out << name << "," << section << "," << rollID << "\n";
    }

    void deserialize(ifstream& in) {
        getline(in, name, ',');
        getline(in, section, ',');
        getline(in, rollID);
    }
};

class Teacher {
public:
    string name;
    string specialization;

    void serialize(ofstream& out) const {
        out << name << "," << specialization << "\n";
    }

    void deserialize(ifstream& in) {
        getline(in, name, ',');
        getline(in, specialization);
    }
};

class Room {
public:
    string roomNumber;
    int capacity;
    list<shared_ptr<Student>> students; // Change here to use shared_ptr

    void serialize(ofstream& out) const {
        out << roomNumber << "," << capacity << "\n";
    }

    void deserialize(ifstream& in) {
        getline(in, roomNumber, ',');
        string capacityStr;
        getline(in, capacityStr);
        capacity = stoi(capacityStr);
    }
};

class User {
private:
    string username;
    string password;

public:
    User(string uname, string pass) : username(uname), password(pass) {}

    bool authenticate(string uname, string pass) {
        return (username == uname && password == pass);
    }

    string getUsername() {
        return username;
    }
};

class LoginSystem {
private:
    User* currentUser;

public:
    LoginSystem() : currentUser(nullptr) {}

    void registerUser() {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pass;
        currentUser = new User(uname, pass);
        cout << "Account created successfully!\n";
    }

    bool login() {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pass;

        if (currentUser != nullptr) {
            if (currentUser->authenticate(uname, pass)) {
                cout << "Login successful. Welcome, " << currentUser->getUsername() << "!\n";
                return true;
            }
        }
        cout << "Invalid username or password!\n";
        return false;
    }

    ~LoginSystem() {
        delete currentUser;
    }
};

vector<Student> students;
vector<Teacher> teachers;
vector<Room> rooms;


// Function declarations 

void removeStudentFromRoom(Room & room, const string & studentName);
bool isRoomAvailableForMakeupClass(const vector<Course>& courses, const string& roomNumber, const string& time);
void switchTeacherRoom(vector<Course>& courses, const string& courseCode, const string& newRoomNumber, vector<Room>& rooms);
void addStudentsToRoom(shared_ptr<Room>& room, const vector<shared_ptr<Student>>& students);


int validateIntInput() {
    int input;
    while (!(cin >> input)) {
        cout << "Invalid input. Please enter a valid integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return input;
}

char validateCharInput() {
    char input;
    while (!(cin >> input) || (input != '\n' && !isprint(input)) || input == '\0' || (input == 32)) {
        cout << "Invalid input. Please enter a valid character: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return input;
}


#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower

void removeStudentFromRoom(Room& room, const string& studentName) {
    bool studentRemoved = false;
    string lowercaseStudentName = studentName;
    transform(lowercaseStudentName.begin(), lowercaseStudentName.end(), lowercaseStudentName.begin(), ::tolower);

    auto it = room.students.begin();
    while (it != room.students.end()) {
        string lowercaseRoomStudentName = (*it)->name;
        transform(lowercaseRoomStudentName.begin(), lowercaseRoomStudentName.end(), lowercaseRoomStudentName.begin(), ::tolower);

        if (lowercaseRoomStudentName == lowercaseStudentName) {
            it = room.students.erase(it);
            studentRemoved = true;
            break; // Student found and removed, exit the loop
        }
        else {
            ++it;
        }
    }
    if (studentRemoved) {
        cout << "Student " << studentName << " removed from room " << room.roomNumber << " successfully.\n";
    }
    else {
        cout << "Student " << studentName << " not found in room " << room.roomNumber << ".\n";
    }
}




bool isRoomAvailableForMakeupClass(const vector<Course>& courses, const string& roomNumber, const string& time) {
    for (const auto& course : courses) {
        if (course.room->roomNumber == roomNumber && course.timeSlot == time) {
            return false;
        }
    }
    return true;
}

void switchTeacherRoom(vector<Course>& courses, const string& courseCode, const string& newRoomNumber, vector<Room>& rooms) {
    for (auto& course : courses) {
        if (course.courseCode == courseCode) {
            course.room = nullptr;
            for (auto& room : rooms) {
                if (room.roomNumber == newRoomNumber) {
                    course.room = &room;
                    break;
                }
            }
            if (course.room == nullptr) {
                cout << "Room " << newRoomNumber << " not found!" << endl;
            }
            break;
        }
    }
}


