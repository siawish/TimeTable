class Course {
public:
    string courseCode;
    string courseName;
    Teacher* teacher;
    string timeSlot;
    Room* room; // Using raw pointers for Room

    void serialize(ofstream& outFile) const {
        outFile << courseCode << " " << courseName << " " << teacher->name << " " << timeSlot << " " << room->roomNumber << endl;
    }

    void deserialize(ifstream& inFile, vector<Teacher>& teachers, vector<Room>& rooms) {
        string teacherName, roomNumber;
        inFile >> courseCode >> courseName >> teacherName >> timeSlot >> roomNumber;
        for (auto& t : teachers) {
            if (t.name == teacherName) {
                teacher = &t;
                break;
            }
        }
        for (auto& r : rooms) {
            if (r.roomNumber == roomNumber) {
                room = &r;
                break;
            }
        }
    }
};



vector<Course> courses;

// Function declarations
void generateTeacherWiseTimeTable(const vector<Course>& courses);
void generateSectionWiseTimeTable(const vector<Course>& courses);
void generateRoomWiseTimeTable(const vector<Course>& courses);
void displayTimeTableForTime(const vector<Course>& courses, const string& time, const string& day);
void displayTimeTableForDay(const vector<Course>& courses, const string& day);
void displayTimeTable(const vector<Course>& timeTable);
void addStudentsToRoom(Room& room, const vector<Student>& students);


void addCourse(vector<Course>& courses, const string& courseCode, const string& courseName, Teacher& teacher, Room& room, const string& timeSlot);


void generateTeacherWiseTimeTable(const vector<Course>& courses) {
    map<string, vector<Course>> teacherTimeTable;
    for (const auto& course : courses) {
        teacherTimeTable[course.teacher->name].push_back(course);
    }
    for (const auto& entry : teacherTimeTable) {
        cout << "Teacher: " << entry.first << endl;
        displayTimeTable(entry.second);
        cout << endl;
    }
}

void generateSectionWiseTimeTable(const vector<Course>& courses) {
    map<string, vector<Course>> sectionTimeTable;
    for (const auto& course : courses) {
        sectionTimeTable[course.room->roomNumber].push_back(course);
    }
    for (const auto& entry : sectionTimeTable) {
        cout << "Room: " << entry.first << endl;
        displayTimeTable(entry.second);
        cout << endl;
    }
}


void generateRoomWiseTimeTable(const vector<Course>& courses) {
    map<string, vector<Course>> roomTimeTable;
    for (const auto& course : courses) {
        roomTimeTable[course.room->roomNumber].push_back(course);
    }
    for (const auto& entry : roomTimeTable) {
        cout << "Room: " << entry.first << endl;
        displayTimeTable(entry.second);
        cout << endl;
    }
}

void displayTimeTableForTime(const vector<Course>& courses, const string& time, const string& day) {
    cout << "Teachers teaching at " << time << " on " << day << ":" << endl;
    bool found = false;
    for (const auto& course : courses) {
        if (course.timeSlot.substr(0, 5) == time && course.timeSlot.substr(6) == day) {
            cout << "Teacher: " << course.teacher->name << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No courses found at " << time << " on " << day << endl;
    }
}




void displayTimeTableForDay(const vector<Course>& courses, const string& day) {
    cout << "Time table for " << day << ":" << endl;
    for (const auto& course : courses) {
        if (course.timeSlot.find(day) != string::npos) {
            cout << "Course: " << course.courseName << ", Time: " << course.timeSlot << ", Teacher: " << course.teacher->name << endl;
        }
    }
}


void displayTimeTable(const vector<Course>& timeTable) {
    for (const auto& course : timeTable) {
        cout << "Course: " << course.courseName << ", Time: " << course.timeSlot << ", Room: " << course.room->roomNumber << endl;
    }
}

void addStudentsToRoom(Room& room, const vector<Student>& students) {
    for (const auto& student : students) {
        room.students.push_back(make_shared<Student>(student)); // Changed to use make_shared
    }
}





void addCourse(vector<Course>& courses, const string& courseCode, const string& courseName, Teacher& teacher, Room& room, const string& timeSlot) {
    // Check if the room is available at the given time slot
    for (const auto& course : courses) {
        if (course.room->roomNumber == room.roomNumber && course.timeSlot == timeSlot) {
            cout << "Room " << room.roomNumber << " is not available at the given time slot." << endl;
            return; 
        }
    }

    
    Course newCourse;
    newCourse.courseCode = courseCode;
    newCourse.courseName = courseName;
    newCourse.teacher = &teacher;
    newCourse.room = &room;
    newCourse.timeSlot = timeSlot;
    courses.push_back(newCourse);

    cout << "Course " << courseName << " added successfully." << endl;
    return; 
}

void saveTimeTableToFile(const vector<Course>& timeTable, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& course : timeTable) {
            course.serialize(outFile);
        }
        outFile.close();
        cout << "Time table saved to " << filename << " successfully." << endl;
    } else {
        cout << "Error: Unable to open file " << filename << " for writing." << endl;
    }
}

int main() {
    
        // Initialize students 
        students = {
            {"Alice", "A", "001"},
            {"Bob", "A", "002"},
            {"Charlie", "A", "003"},
            {"David", "A", "004"},
            {"Eve", "B", "005"},
            {"Frank", "B", "006"},
            {"Grace", "B", "007"},
            {"Henry", "C", "008"},
            {"Ivy", "C", "009"},
            {"Jack", "C", "010"}
    };

    // Initialize teachers
    teachers = {
        {"Mr. Smith", "Mathematics"},
        {"Mrs. Johnson", "Physics"},
        {"Mr. Williams", "Chemistry"},
        {"Mrs. Brown", "Biology"},
        {"Mr. Jones", "English"}
    };

    // Initialize rooms
    rooms = {
        {"101", 30},
        {"102", 25},
        {"103", 20}
    };

   
    courses = {
        {"MATH101", "Calculus", &teachers[0], "09:00 Monday", &rooms[0]},
        {"PHYS101", "Physics", &teachers[1], "11:00 Monday", &rooms[1]},
        {"CHEM101", "Chemistry", &teachers[2], "13:00 Monday", &rooms[2]},
        {"BIO101", "Biology", &teachers[3], "09:00 Wednesday", &rooms[0]},
        {"ENG101", "English", &teachers[4], "11:00 Wednesday", &rooms[1]},
        {"MATH102", "Algebra", &teachers[0], "13:00 Wednesday", &rooms[2]}
    };
    
    
    // Read data from files
    ifstream studentFile("students.txt");
    if (studentFile.is_open()) {
        string line;
        while (getline(studentFile, line)) {
            Student student;
            student.deserialize(studentFile);
            students.push_back(student);
        }
        studentFile.close();
    }

    ifstream teacherFile("teachers.txt");
    if (teacherFile.is_open()) {
        string line;
        while (getline(teacherFile, line)) {
            Teacher teacher;
            teacher.deserialize(teacherFile);
            teachers.push_back(teacher);
        }
        teacherFile.close();
    }

    ifstream roomFile("rooms.txt");
    if (roomFile.is_open()) {
        string line;
        while (getline(roomFile, line)) {
            Room room;
            room.deserialize(roomFile);
            rooms.push_back(room);
        }
        roomFile.close();
    }

    ifstream courseFile("courses.txt");
    if (courseFile.is_open()) {
        string line;
        while (getline(courseFile, line)) {
            Course course;
            course.deserialize(courseFile, teachers, rooms);
            courses.push_back(course);
        }
        courseFile.close();
    }

    // Login System
    LoginSystem loginSystem;
int loginChoice;
bool loggedIn = false;

do {
    cout << "Login Menu:\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
    loginChoice = validateIntInput();

    switch (loginChoice) {
        case 1:
            loginSystem.registerUser();
            break;
        case 2:
            if (loginSystem.login()) {
                loggedIn = true; // Set loggedIn to true upon successful login
                cout << "Login successful. Proceeding to main menu.\n";
            } else {
                cout << "Login failed. Please try again.\n";
            }
            break;
        case 0:
            cout << "Exiting...\n";
            return 0; // Exit the program
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
    }
} while (!loggedIn); 

// Main Program Menu
int choice;
do {
    // Display menu
    cout << "\nMenu:\n";
    cout << "1. View time table by time and day.\n";
    cout << "2. View time table by day.\n";
    cout << "3. Add students to rooms.\n";
    cout << "4. Remove a student from a room.\n";
    cout << "5. Check room availability for makeup class.\n";
    cout << "6. Switch teacher's room.\n";
    cout << "7. Generate teacher-wise time table.\n";
    cout << "8. Generate section-wise time table.\n";
    cout << "9. Generate room-wise time table.\n";
    cout << "10. Add a new course.\n";
    cout << "11. Save time table to file.\n";
    cout << "0. Exit.\n";
    cout << "Enter your choice: ";
    choice = validateIntInput();

    switch (choice) {
        case 1: {
            string time, day;
            cout << "Enter time (HH:MM): ";
            cin >> time;
            cout << "Enter day: ";
            cin >> day;
            displayTimeTableForTime(courses, time, day);
            break;
        }
        case 2: {
            string day;
            cout << "Enter day: ";
            cin >> day;
            displayTimeTableForDay(courses, day);
            break;
        }
        case 3: {
            // Adding students to rooms
            string studentName, courseCode, roomNumber;
            cout << "Enter student name: ";
            cin >> studentName;
            cout << "Enter course code: ";
            cin >> courseCode;
            cout << "Enter room number: ";
            cin >> roomNumber;

            // Find the specified course
            Course* selectedCourse = nullptr;
            for (auto& course : courses) {
                if (course.courseCode == courseCode && course.room->roomNumber == roomNumber) {
                    selectedCourse = &course;
                    break;
                }
            }

            if (selectedCourse == nullptr) {
                cout << "Course with code " << courseCode << " is not being taught in room " << roomNumber << "." << endl;
            } else {
                // Check if the room has available capacity
                if (selectedCourse->room->students.size() < selectedCourse->room->capacity) {
                    // Add the student to the room
                    Student newStudent;
                    newStudent.name = studentName;
                    cout << "Enter student roll ID: ";
                    cin >> newStudent.rollID;
                    newStudent.section =  ""; // Assuming section information is not entered manually
                    selectedCourse->room->students.push_back(make_shared<Student>(newStudent));
                    cout << "Student " << studentName << " added to room " << roomNumber << " for course " << selectedCourse->courseName << "." << endl;
                } else {
                    cout << "Room " << roomNumber << " is full for course " << selectedCourse->courseName << "." << endl;
                }
            }
            break;
        }
        case 4: {
            string studentName;
            cout << "Enter student name to remove: ";
            cin >> studentName;
            for (auto& room : rooms) {
                removeStudentFromRoom(room, studentName);
            }
            cout << "Student removed from all rooms successfully.\n";
            break;
        }
        case 5: {
            string roomNumber, time;
            cout << "Enter room number: ";
            cin >> roomNumber;
            cout << "Enter time (HH:MM): ";
            cin >> time;
            if (isRoomAvailableForMakeupClass(courses, roomNumber, time)) {
                cout << "Room " << roomNumber << " is available for makeup class at " << time << ".\n";
            } else {
                cout << "Room " << roomNumber << " is not available for makeup class at " << time << ".\n";
            }
            break;
        }
        case 6: {
            string courseCode, newRoomNumber;
            cout << "Enter the course code of the course to switch: ";
            cin >> courseCode;
            cout <<  "Enter the new room number: ";
            cin >> newRoomNumber;
            switchTeacherRoom(courses, courseCode, newRoomNumber, rooms);
            cout << "Teacher's room switched successfully.\n";
            break;
        }
        case 7: {
            generateTeacherWiseTimeTable(courses);
            break;
        }
        case 8: {
            generateSectionWiseTimeTable(courses);
            break;
        }
        case 9: {
            generateRoomWiseTimeTable(courses);
            break;
        }
case 10: {
    string courseCode, courseName, teacherName, roomNumber, timeSlot, teacherSpecialization;
    cout << "Enter course code: ";
    cin >> courseCode;
    cout << "Enter course name: ";
    cin >> courseName;
    cout << "Enter teacher name: ";
    cin >> teacherName;
    cout << "Enter teacher specialization: ";
    cin >> teacherSpecialization;
    cout << "Enter room number: ";
    cin >> roomNumber;
    cout << "Enter time slot (HH:MM Day): ";
    cin >> timeSlot;

    // Create a Teacher object using the provided name and specialization
    Teacher teacher{teacherName, teacherSpecialization};

    // Find the room by room number
    Room* selectedRoom = nullptr;
    for (auto& room : rooms) {
        if (room.roomNumber == roomNumber) {
            selectedRoom = &room;
            break;
        }
    }

    // Check if the room is found
    if (selectedRoom == nullptr) {
        cout << "Room " << roomNumber << " not found." << endl;
    } else {
        // Add the course
        addCourse(courses, courseCode, courseName, teacher, *selectedRoom, timeSlot);
    }
    break;
}

        case 11: {
            string filename;
            cout << "Enter filename to save the time table: ";
            cin >> filename;
            saveTimeTableToFile(courses, filename);
            break;
        }
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
            break;
    }

} while (choice != 0);

    // Write data to files
    ofstream studentOut("students.txt");
    if (studentOut.is_open()) {
        for (const auto& student : students) {
            student.serialize(studentOut);
        }
        studentOut.close();
    }

    ofstream teacherOut("teachers.txt");
    if (teacherOut.is_open()) {
        for (const auto& teacher : teachers) {
            teacher.serialize(teacherOut);
        }
        teacherOut.close();
    }

    ofstream roomOut("rooms.txt");
    if (roomOut.is_open()) {
        for (const auto& room : rooms) {
            room.serialize(roomOut);
        }
        roomOut.close();
    }

    ofstream courseOut("courses.txt");
    if (courseOut.is_open()) {
        for (const auto& course : courses) {
            course.serialize(courseOut);
        }
        courseOut.close();
    }

    return 0;
}