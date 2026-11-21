#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <limits>
using namespace std;

// Base class
class Student {
protected:
    int rollNo;
    string name;

public:
    Student(int r = 0, const string& n = "") : rollNo(r), name(n) {}

    virtual void display() const {
        cout << "Roll Number: " << rollNo << "\nName: " << name << '\n';
    }

    virtual ~Student() = default;
};

// Derived class
class Result : public Student {
private:
    double mark1, mark2, mark3;

    void validateMarks() const {
        if (mark1 < 0 || mark1 > 100 || mark2 < 0 || mark2 > 100 ||
            mark3 < 0 || mark3 > 100) {
            throw invalid_argument("Marks must be between 0 and 100.");
        }
    }

public:
    Result(int r, const string& n, double m1, double m2, double m3)
        : Student(r, n), mark1(m1), mark2(m2), mark3(m3) {
        validateMarks();
    }

    double total() const {
        return mark1 + mark2 + mark3;
    }

    double percentage() const {
        return total() / 3.0;
    }

    double getPercentage() const {
        return percentage();
    }

    string resultStatus() const {
        return (mark1 >= 35 && mark2 >= 35 && mark3 >= 35) ? "PASS" : "FAIL"; 
}

    void display() const override {
        Student::display();
        cout << fixed << setprecision(2);
        cout << "Marks: " << mark1 << ", " << mark2 << ", " << mark3 << '\n';
        cout << "Total: " << total() << " / 300\n";
        cout << "Percentage: " << percentage() << "%\n";
        cout << "Result: " << resultStatus() << "\n";
        cout << "-----------------------------\n";
    }

    string toFileLine() const {
        ostringstream line;
        line << rollNo << '|' << name << '|' << mark1 << '|'
             << mark2 << '|' << mark3;
        return line.str();
    }
};

void addStudent(vector<Result>& students) {
    int roll;
    string name;
    double m1, m2, m3;

    cout << "Enter roll number: ";
    cin >> roll;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter student name: ";  
getline(cin, name);

    cout << "Enter marks in three subjects: ";
    if (!(cin >> m1 >> m2 >> m3)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Please enter numeric marks.");
    }

    students.emplace_back(roll, name, m1, m2, m3);
    cout << "Student record added successfully.\n";
}

void displayStudents(const vector<Result>& students) {
    if (students.empty()) {
        cout << "No student records available.\n";
        return;
    }
    for (const Result& student : students) {
        student.display();
    }
}

void saveRecords(const vector<Result>& students) {
    ofstream outFile("student_records.txt");
    if (!outFile) {
        throw runtime_error("Unable to open file for writing.");
    }

    for (const Result& student : students) {
        outFile << student.toFileLine() << '\n';
    }
    cout << "Student records saved to student_records.txt.\n";
}

void loadRecords(vector<Result>& students) {
    ifstream inFile("student_records.txt");
    if (!inFile) {
        throw runtime_error("Unable to open student_records.txt for reading.");  
  }

    vector<Result> loadedStudents;
    string line;
    int skipped = 0;

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        try {
            stringstream ss(line);
            string rollText, name, m1Text, m2Text, m3Text;
            if (!getline(ss, rollText, '|') || !getline(ss, name, '|') ||
                !getline(ss, m1Text, '|') || !getline(ss, m2Text, '|') ||
                !getline(ss, m3Text)) {
                throw invalid_argument("Invalid record format.");
            }

            int roll = stoi(rollText);
            double m1 = stod(m1Text);
            double m2 = stod(m2Text);
            double m3 = stod(m3Text);
            loadedStudents.emplace_back(roll, name, m1, m2, m3);
        } catch (const exception&) {
            ++skipped;
        }
    }

    students = loadedStudents;
    cout << students.size() << " record(s) loaded successfully.\n";
    if (skipped > 0) {
        cout << skipped << " invalid record(s) were skipped.\n";
    }
}

void findAbovePercentage(const vector<Result>& students) {
    double limit;
    cout << "Enter percentage to search above: ";
    if (!(cin >> limit)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Please enter a valid percentage.");
    }

    auto it = find_if(students.begin(), students.end(),
                      [limit](const Result& student) {
                          return student.getPercentage() > limit;
                      });

    if (it == students.end()) {
        cout << "No students scored above " << limit << "% .\n";
        return;
    }

    cout << "Students scoring above " << limit << "%:\n";
    for (; it != students.end(); ++it) {
        if (it->getPercentage() > limit) {
            it->display();
        }
    }
}

int main() {
    vector<Result> students;
    int choice;

    do {
        cout << "\n===== STUDENT RESULT MANAGEMENT SYSTEM =====\n";
        cout << "1. Add student record\n";
        cout << "2. Display all records\n";
        cout << "3. Save records to file\n";
        cout << "4. Load records from file\n";
        cout << "5. Find students above a percentage\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid menu choice.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1:
                    addStudent(students);
                    break;
                case 2:
                    displayStudents(students);
                    break;
                case 3:
                    saveRecords(students);
                    break;
                case 4:
                    loadRecords(students);
                    break;
                case 5:
                    findAbovePercentage(students);
                    break;
                case 0:
                    cout << "Exiting program. Thank you!\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    } while (choice != 0);

    return 0;
}
