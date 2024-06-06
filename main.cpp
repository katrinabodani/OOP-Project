#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <time.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <limits> //exception handling

#ifdef _WIN32 // Check if compiling on Windows
#include <conio.h> // for displaying * when password
#else // Linux platform
#include <termios.h> // for tcgetattr(), tcsetattr(), etc.
#include <unistd.h> // for STDIN_FILENO
#endif

#define MAX_QUESTION_OPTIONS 10

using namespace std;

class Subject {
private:
    string name;
    string code;

public:
    // Default constructor
    Subject() {
        name = "";
        code = "";
    }

    // Parameterized constructor
    Subject(string name, string code) {
        this->name = name;
        this->code = code;
    }

    // Getter for name
    string getName() const {
        return name;
    }

    // Setter for name
    void setName(string name) {
        this->name = name;
    }

    // Getter for code
    string getCode() const {
        return code;
    }

    // Setter for code
    void setCode(string code) {
        this->code = code;
    }
};

class User {
protected:
    string name;
    string username;
    string password;
    bool isTeacher;

public:
    // Default constructor
    User() {
        name = username = password = "\0";
        isTeacher = false;
    }

    // Parameterized constructor
    User(string name, string username, bool teacher) {
        this->name = name;
        this->username = username;
        this->isTeacher = teacher;
        this->password = "\0";
    }

    // Getter methods
    string getName() const {
        return name;
    }

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return this->password;
    }

    bool getIsTeacher() const {
        return isTeacher;
    }

    // Setter methods
    void setName(string name) {
        this->name = name;
    }

    void setUsername(string username) {
        this->username = username;
    }

    void setPassword(string password) {
        this->password = password;
    }

    void setIsTeacher(bool teacher) {
        this->isTeacher = teacher;
    }

    //Functions
    bool changePassword() {
        string newPassword;
        bool hasUppercase = false;
        bool hasLowercase = false;
        bool hasDigit = false;
        bool hasSpecialChar = false;

        // Prompt the user to enter a new password
        cout << "Enter a new password: ";
        cin >> newPassword;

        // Validate the password length
        if (newPassword.length() < 6) {
            cout << "Password must have at least 6 characters." << endl;
            return false;
        }

        // Check if the password contains uppercase, lowercase, numeric digit, and special characters
        for (int i = 0; i < newPassword.length(); i++) {
            char c = newPassword[i];

            //upper case
            if (c >= 'A' && c <= 'Z') {
                hasUppercase = true;
            }
            //lower case
            else if (c >= 'a' && c <= 'z') {
                hasLowercase = true;
            }
            //digit
            else if (c >= '0' && c <= '9') {
                hasDigit = true;
            }
            //punctuation
            else if (c >= 33 && c <= 47) { // ASCII values for special characters
                hasSpecialChar = true;
            }
        }

        if (!hasUppercase || !hasLowercase || !hasDigit || !hasSpecialChar) {
            cout << "Password must contain at least one uppercase letter, one lowercase letter, one numeric digit, and one special character." << endl;
            return false;
        }

        this->password = newPassword;
        cout << "Password updated successfully." << endl;
        return true;
    }


};

class Student : public User {
private:
    int numEnrolledCourses;

public:
    Subject* subjectArray;
    // Default constructor
    Student() : User() {
        numEnrolledCourses = 0;
        subjectArray = nullptr;
        isTeacher = false;
    }

    // Parameterized constructor
    Student(string name, string username, int numEnrolledCourses) : User(name, username, false) {
        this->numEnrolledCourses = numEnrolledCourses;
        this->subjectArray = nullptr;
    }

    // Getter method
    int getNumEnrolledCourses() const {
        return numEnrolledCourses;
    }

    // Setter method
    void setNumEnrolledCourses(int numEnrolledCourses) {
        this->numEnrolledCourses = numEnrolledCourses;
    }

    //Functions
    void iniializeSubjectArray()
    {
        subjectArray = new Subject[numEnrolledCourses];
    }

    //Display Subjects
    void displaySubjects() const
    {
        for (int i = 0; i < this->numEnrolledCourses; i++)
        {
            cout << this->subjectArray[i].getName() << ", ";
        }
        cout << "\n";
    }

    //Destructor
    ~Student()
    {
        delete[]subjectArray;
    }
};

class Teacher : public User {
private:
    string teachingCourse;

public:
    // Default constructor
    Teacher() : User() {
        teachingCourse = "";
        isTeacher = true;
    }

    // Parameterized constructor
    Teacher(string name, string username, string teachingCourse) : User(name, username, true) {
        this->teachingCourse = teachingCourse;
    }

    // Getter method
    string getTeachingCourse() const {
        return teachingCourse;
    }

    // Setter method
    void setTeachingCourse(string teachingCourse) {
        this->teachingCourse = teachingCourse;
    }

    //Functions
    void displayInfo() const
    {
        cout << this->name << ": " << this->username << ": " << this->teachingCourse;
    }
};

//Department class composes of both Student and Teacher classes
class Department {
private:
    int numStudents = 0;
    int numTeachers = 0;
    Student* studentArray = nullptr;
    Teacher* teacherArray = nullptr;
    string* subjNamesArray = nullptr;
    string* subjCodesArray = nullptr;

    // Function to get count of students or teachers, based on type
    int getCount(string fname, int type = 0) const {
        ifstream inputFile(fname);

        if (!inputFile.is_open()) {
            cout << "Failed to open file!" << endl;
            return 0;
        }

        int counter = 0;
        string line;
        const int ROWS = 1;  // Maximum number of rows
        int row = 0;
        while (getline(inputFile, line)) {
            counter++;
            if (row >= ROWS) {
                cout << "Maximum number of rows exceeded!" << endl;
                break;
            }
        }

        inputFile.close();
        if (type == 0)
        {
            return counter -= 3;
        }
        else
        {
            return counter;
        }
    }

    void populateSubjects(string subjNames, string subjCodes, int count)
    {
        subjNamesArray = new string[count];
        subjCodesArray = new string[count];

        stringstream ssNames(subjNames);
        stringstream ssCodes(subjCodes);

        for (int i = 0; i < count; i++) {
            //delimiter is _
            getline(ssNames, subjNamesArray[i], '_');
            getline(ssCodes, subjCodesArray[i], '_');
        }
    }

    // Function to read data from CSV file
    int read_csv(string fname, int skipRowsStart = 0) {
        ifstream inputFile(fname);

        if (!inputFile.is_open()) {
            cout << "Failed to open file!" << endl;
            return 0;
        }

        int counter = 0;
        string line;
        const int ROWS = 1;  // Maximum number of rows
        int row = 0;
        string subjNames = "";
        string subjCodes = "";
        int totalSubjCounter = 0;
        for (int i = 0; i < numStudents; i++)
        {
            if (counter < skipRowsStart)
            {
                getline(inputFile, line);
                if (row >= ROWS) {
                    cout << "Maximum number of rows exceeded!" << endl;
                    break;
                }
                stringstream lineStream(line);
                string cell;
                int cellCounter = 0;
                while (getline(lineStream, cell, ',')) {
                    if (cellCounter > 3)
                    {
                        if (counter == 0) //subject name
                        {
                            subjNames += cell;
                            subjNames += "_";
                            totalSubjCounter++;
                        }
                        else if (counter == 1) //subject code
                        {
                            subjCodes += cell;
                            subjCodes += "_";
                        }
                    }
                    cellCounter++;
                }
                i--;
                counter++;
                continue;
            }
            //storing subjects and codes
            if (counter == skipRowsStart)
            {
                //calling this function only once
                populateSubjects(subjNames, subjCodes, totalSubjCounter);
            }
            getline(inputFile, line);
            if (row >= ROWS) {
                cout << "Maximum number of rows exceeded!" << endl;
                break;
            }
            stringstream lineStream(line);
            string cell;
            int cellCounter = 0;
            int numSubjects = 0;
            bool* subjBoolArray = new bool[totalSubjCounter];
            int currSubjCounter = 0;
            while (getline(lineStream, cell, ',')) {
                if (cellCounter == 1) {
                    studentArray[i].setUsername(cell);
                }
                else if (cellCounter == 2) {
                    studentArray[i].setName(cell);
                }
                else if (cellCounter != 0) {
                    if (cell == "1")
                    {
                        subjBoolArray[currSubjCounter] = 1;
                        numSubjects++;
                    }
                    else
                    {
                        subjBoolArray[currSubjCounter] = 0;
                    }
                    currSubjCounter++;
                }
                cellCounter++;
                //cout << cell << " ";
            }
            //setting students number of subjects
            studentArray[i].setNumEnrolledCourses(numSubjects);
            studentArray[i].iniializeSubjectArray();
            //getting what subject each student is studying
            for (int j = 0, k = 0; j < totalSubjCounter; j++)
            {
                if (subjBoolArray[j] == 1)
                {
                    studentArray[i].subjectArray[k].setName(subjNamesArray[j]);
                    studentArray[i].subjectArray[k].setCode(subjCodesArray[j]);
                    k++;
                }
            }
            counter++;
            delete[] subjBoolArray;
            //cout << endl;
        }
        inputFile.close();
        return 1;
    }

    int read_teacher_txt(string fname) {
        // Open the file
        ifstream file(fname);
        if (!file.is_open()) {
            cout << "Error opening file " << fname << endl;
            return -1;
        }

        // Read the data from the file
        string line;
        int countTeachers = 0;
        while (getline(file, line)) {
            istringstream iss(line);
            string fname, lname, username, teachingCourse;
            iss >> fname >> lname >> username;
            getline(iss, teachingCourse);
            //removing leading whitespace
            teachingCourse.erase(0, teachingCourse.find_first_not_of(" "));
            // Create a new Teacher object and add it to the teacherArray
            teacherArray[countTeachers++] = Teacher(fname + " " + lname, username, teachingCourse);
        }

        // Close the file
        file.close();

        return numTeachers;
    }


public:
    // Default constructor
    Department() {
        numStudents = 0;
        numTeachers = 0;
        studentArray = nullptr;
        teacherArray = nullptr;
        subjCodesArray = nullptr;
        subjNamesArray = nullptr;
    }

    // Parameterized constructor that accepts student .csv and teacher .txt file
    Department(string fnameStudents, string fnameTeacher, int skipRows) {
        numStudents = getCount(fnameStudents); //calling for student counts
        numTeachers = getCount(fnameTeacher, 1); //calling for teacher counts
        studentArray = new Student[numStudents];
        teacherArray = new Teacher[numTeachers];
        subjCodesArray = nullptr;
        subjNamesArray = nullptr;
        read_csv(fnameStudents, skipRows); //storing student data
        read_teacher_txt(fnameTeacher);
    }

    // Getter methods
    int getNumStudents() const {
        return numStudents;
    }

    int getNumTeachers() const {
        return numTeachers;
    }

    Student* getStudentArray() const {
        return studentArray;
    }

    Teacher* getTeacherArray() const {
        return teacherArray;
    }

    // Setter methods
    void setNumStudents(int num) {
        numStudents = num;
    }

    void setNumTeachers(int num) {
        numTeachers = num;
    }

    void setStudentArray(Student* arr) {
        studentArray = arr;
    }

    void setTeacherArray(Teacher* arr) {
        teacherArray = arr;
    }

    //Functions
    void displayStudentDetails() const
    {
        for (int i = 0; i < numStudents; i++)
        {
            cout << studentArray[i].getUsername() << ": " << studentArray[i].getName() << ": " <<
                studentArray[i].getNumEnrolledCourses() << ": ";
            studentArray[i].displaySubjects();
            cout << "\n";
        }
    }

    void displayTeacherDetails() const
    {
        for (int i = 0; i < numTeachers; i++)
        {
            teacherArray[i].displayInfo();
            cout << "\n";
        }
    }

    // Destructor
    ~Department() {
        delete[] studentArray;
        delete[] teacherArray;
        delete[] subjCodesArray;
        delete[] subjNamesArray;
    }

};

class Login {
public:
    Login() {};


    //Functions
    int getLoginType() const
    {
        int loginType;
        while (true) {
            cout << "Select login type:" << endl;
            cout << "0. Student" << endl;
            cout << "1. Teacher" << endl;
            cin >> loginType;
            if (cin.fail()) {
                // Clear the error flag and discard any remaining input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input: not an integer.\n";
            }
            else if (loginType < 0 || loginType > 1) {
                cout << "Invalid input: not a valid login type.\n";
            }
            else {
                break; // Input is valid, exit the loop
            }
        }
        return loginType;
    }

    int performLogin(Student* studentArray, Teacher* teacherArray, int numStudents, int numTeachers, bool loginType) const
    {
        int index = -1;
        //login type tels if teacher or student i-e 0=student, 1=teacher
        if (loginType == 0) {
            // Student login
            string username;
            string password;
            cout << "Student Login" << endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            #ifdef _WIN32 // Windows platform
                char c;
                while ((c = _getch()) != '\r') {
                    password += c;
                    cout << "*";
                }
            #else // Linux platform
                struct termios oldt, newt;
                tcgetattr(STDIN_FILENO, &oldt);
                newt = oldt;
                newt.c_lflag &= ~ECHO;
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);

                char c;
                while ((c = getchar()) != '\n') {
                    password += c;
                }

                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                cout << endl;
            #endif
            cout << endl;
            bool found = false;
            for (int i = 0; i < numStudents; i++) {
                if (studentArray[i].getUsername() == username) {
                    found = true;
                    index = i;
                    break;
                }
            }
            if (found) {
                cout << "Login successful." << endl;
                return index;
            }
            else {
                cout << "Invalid username." << endl;
                return 0;
            }
        }
        else if (loginType == 1) {
            // Teacher login
            string username;
            string password;
            cout << "Teacher Login" << endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            char c;
            while ((c = _getch()) != '\r') {
                password += c;
                cout << "*";
            }
            cout << endl;
            bool found = false;
            for (int i = 0; i < numTeachers; i++) {
                if (teacherArray[i].getUsername() == username) {
                    found = true;
                    index = i;
                    break;
                }
            }
            if (found) {
                cout << "Login successful." << endl;
                return index;
            }
            else {
                cout << "Invalid username." << endl;
                return -1;
            }
        }
        else {
            cout << "Invalid login type." << endl;
            return -1;
        }
    }

    //get option if they want to change password or not
    bool changePassword()
    {
        int option;
        while (true) {
            cout << "0. Change Password" << endl;
            cout << "1. Continue" << endl;
            cin >> option;
            if (cin.fail()) {
                // Clear the error flag and discard any remaining input
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input: not an integer.\n";
            }
            else if (option < 0 || option > 1) {
                cout << "Invalid input: not a valid option type.\n";
            }
            else {
                return option;
            }
        }
    }
};

class Question {
private:
    string subject;
    string topic;
    string type;
    string text;
    int numOptions;
    string options[MAX_QUESTION_OPTIONS];
    int correctOptionIndex;
    string correctOptionText;
public:
    // Default constructor
    Question() {
        subject = "";
        topic = "";
        type = "";
        text = "";
        numOptions = 0;
        for (int i = 0; i < MAX_QUESTION_OPTIONS; i++) {
            options[i] = "";
        }
        correctOptionIndex = -1;
        correctOptionText = "";
    }

    // Parameterized constructor
    Question(string subj, string tp, string qtype, string qtext, int numOpts, string* opts, int correctIndex, string correctText) {
        subject = subj;
        topic = tp;
        type = qtype;
        text = qtext;
        numOptions = numOpts;
        //options = new string[numOpts];
        for (int i = 0; i < MAX_QUESTION_OPTIONS; i++) {
            options[i] = "";
        }
        for (int i = 0; i < numOpts; i++) {
            options[i] = opts[i];
        }
        correctOptionIndex = correctIndex;
        correctOptionText = correctText;
    }

    // Copy constructor
    Question(const Question& q) {
        subject = q.subject;
        topic = q.topic;
        type = q.type;
        text = q.text;
        numOptions = q.numOptions;
        for (int i = 0; i < MAX_QUESTION_OPTIONS; i++) {
            options[i] = "";
        }
        for (int i = 0; i < numOptions; i++) {
            options[i] = q.options[i];
        }
        correctOptionIndex = q.correctOptionIndex;
        correctOptionText = q.correctOptionText;
    }

    // Destructor
    //~Question() {}

    // Getter functions
    string getSubject() const {
        return subject;
    }

    string getTopic() const {
        return topic;
    }

    string getType() const {
        return type;
    }

    string getText() const {
        return text;
    }

    int getNumOptions() const {
        return numOptions;
    }

    const string* getOptions() const {
        return options;
    }

    int getCorrectOptionIndex() const {
        return correctOptionIndex;
    }

    string getCorrectOptionText() const {
        return correctOptionText;
    }

    // Setter functions
    void setSubject(string subj) {
        subject = subj;
    }

    void setTopic(string tp) {
        topic = tp;
    }

    void setType(string qtype) {
        type = qtype;
    }

    void setText(string qtext) {
        text = qtext;
    }

    void setNumOptions(int numOpts) {
        numOptions = numOpts;
    }

    void setOption(int index, string opt) {
        options[index] = opt;
    }

    void setCorrectOptionIndex(int correctIndex) {
        correctOptionIndex = correctIndex;
    }

    void setCorrectOptionText(string correctText) {
        correctOptionText = correctText;
    }

    //Functions

    //displaying attributes
    void displayQuestion() {
        cout << "Subject: " << this->getSubject() << endl;
        cout << "Topic: " << this->getTopic() << endl;
        cout << "Type: " << this->getType() << endl;
        cout << "Text: " << this->getText() << endl;
        cout << "Number of options: " << this->getNumOptions() << endl;
        cout << "Options: ";
        for (int i = 0; i < this->getNumOptions(); i++) {
            cout << options[i];
            if (i != this->getNumOptions() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "Correct option index: " << this->getCorrectOptionIndex() << endl;
        cout << "Correct option text: " << this->getCorrectOptionText() << endl;
    }
};

class QuestionBank {
private:
    int num_questions;
    string CORRECT_MARKER = "dabfac4";
    string fname;
    string subject;

public:
    Question* questions;
    QuestionBank() {
        questions = nullptr;
        num_questions = 0;
        CORRECT_MARKER = "dabfac4";
        fname = "\0";
        subject = "\0";
    }
    QuestionBank(string fname, string subject, string corr_marker = "dabfac4") {
        this->fname = fname;
        this->subject = subject;
        num_questions = 0;
        questions = nullptr;
        this->CORRECT_MARKER = corr_marker;
        ifstream file(fname);
        string line;
        int count_2efcde9 = 0, count_b94d27b = 0, count_88f7ace = 0;

        while (getline(file, line)) {
            if (line.find("2efcde9") != string::npos) {
                count_2efcde9++;
            }
            if (line.find("b94d27b") != string::npos) {
                count_b94d27b++;
            }
            if (line.find("88f7ace") != string::npos) {
                count_88f7ace++;
            }
        }
        file.close();
        num_questions = count_2efcde9 + count_b94d27b + count_88f7ace;
        //dynamic array to store questions
        questions = new Question[num_questions];
    }

    //Copy Constructor
    QuestionBank(const QuestionBank& other) {
        num_questions = other.num_questions;
        CORRECT_MARKER = other.CORRECT_MARKER;
        fname = other.fname;
        subject = other.subject;

        questions = new Question[num_questions];
        for (int i = 0; i < num_questions; i++) {
            questions[i] = other.questions[i];
        }
    }


    //~QuestionBank() {
    //    delete[] questions;
    //}

    // Getter methods
    Question* getQuestions() const {
        return questions;
    }

    int getNumQuestions() const {
        return num_questions;
    }

    string getCorrectMarker() const {
        return CORRECT_MARKER;
    }

    string getFileName() const {
        return fname;
    }

    string getSubject() const {
        return subject;
    }

    // Setter methods
    void setQuestions(Question* q) {
        questions = q;
    }

    void setNumQuestions(int n) {
        num_questions = n;
    }

    void setCorrectMarker(string cm) {
        CORRECT_MARKER = cm;
    }

    void setFileName(string fn) {
        fname = fn;
    }

    void setSubject(string sb) {
        subject = sb;
    }


    //Functions

    //load questions from a text file, given a subject
    int loadQuestions(string filename, string subject) {
        ifstream infile(filename.c_str());
        if (!infile.is_open()) {
            cout << "Error: could not open file " << filename << endl;
            return -1;
        }

        const int MAX_LINE_LENGTH = 1000;
        char line[MAX_LINE_LENGTH];
        char* token;
        string current_topic = "\0";
        string current_type = "\0";
        string current_text = "\0";
        int current_num_options = -1;
        string current_options[MAX_QUESTION_OPTIONS] = { "\0" };
        int current_correct_index = -1;
        string current_correct_text = "\0";
        string current_options_text = "\0";
        int curr_questions_count = 0;
        int txt_count = 0;
        int curr_options_idx = 0;
        int qAddedCounter = 0;

        while (infile.getline(line, MAX_LINE_LENGTH)) {
            if (line[0] == '\0') {
                if (curr_questions_count > 0) {
                    // Empty line, end of current question
                    //add question to question bank

                     // Split the options text into tokens using strtok_s
                    char* token = strtok(&current_options_text[0], "\t");
                    int i = 0;
                    while (token != NULL && i < current_num_options) {
                        current_options[i++] = token;
                        token = strtok(NULL, "\t");
                    }

                    //Question(string subj, string tp, string qtype, string qtext, int numOpts, string* opts, int correctIndex, string correctText)
                    Question q(subject, current_topic, current_type, current_text,
                        current_num_options, current_options, current_correct_index, current_correct_text);
                    questions[qAddedCounter++] = q;


                    //reset the variables for the next question
                    current_type = "\0";
                    current_text = "\0";
                    current_num_options = -1;
                    // Deallocate memory for current_options
                    for (int i = 0; i < 10; i++) {
                        current_options[i] = "";
                    }
                    current_correct_index = -1;
                    current_correct_text = "\0";
                    current_options_text = "\0";
                    txt_count = 0;
                    curr_options_idx = 0;
                }
            }
            else if (strcmp(line, "a5380ee") == 0) {
                //topic
                infile.getline(line, MAX_LINE_LENGTH);
                current_topic = line;
                curr_questions_count = 0;
                txt_count = 0;
            }
            else if (strcmp(line, "2efcde9") == 0) {
                //MCQ
                current_type = "MCQ";
                current_num_options = 4;
                curr_questions_count++;
            }
            else if (strcmp(line, "b94d27b") == 0) {
                //True_False
                current_type = "True/False";
                current_num_options = 2;
                curr_questions_count++;
            }
            else if (strcmp(line, "88f7ace") == 0) {
                current_type = "Descriptive";
                current_num_options = 0;
                curr_questions_count++;
            }
            else if (strstr(line, "option") != NULL) {
                // Line contains an option
                string str_line = string(line);

                // correct option
                if (str_line.find(this->CORRECT_MARKER) != std::string::npos) {
                    int pos = str_line.find(CORRECT_MARKER);
                    str_line.erase(pos, this->CORRECT_MARKER.length() + 1); // add 1 to remove the colon as well
                    current_correct_index = curr_options_idx;
                    current_correct_text = str_line.substr(pos + 1); // extract the rest of the text after the index
                    current_correct_text.erase(0, current_correct_text.find_first_not_of(" \t")); // remove leading whitespace
                }

                // Append the line to a string without the first word
                int index = str_line.find(' ');
                if (index != string::npos) {
                    string option_text = str_line.substr(index + 1);
                    // Remove leading whitespace from option_text
                    option_text.erase(0, option_text.find_first_not_of(" \t"));
                    current_options_text += option_text;
                    if (!(curr_options_idx == current_num_options - 1))
                    {
                        current_options_text += "\t";
                    }
                }
                curr_options_idx++;
            }
            else {
                // Line contains question statement
                if (txt_count == 0) {
                    current_text = "";
                }
                else {
                    current_text += "\n";
                }
                current_text += line;
                txt_count++;
            }
        }

        infile.close();
        //print the number of questions added to the question bank
        cout << "Added " << num_questions << " questions to the question bank." << endl;
    }

    //display all questions from question bank
    void displayQuestions() const {
        for (int i = 0; i < num_questions; i++) {
            cout << "Question " << i + 1 << ":" << endl;
            questions[i].displayQuestion();
            cout << endl;
        }
    }

};

class QuestionBankManager {
private:
    int numberOfQuestionBanks;

public:
    QuestionBank* questionBank;
    // Default constructor
    QuestionBankManager() {
        numberOfQuestionBanks = 0;
        questionBank = nullptr;
    }

    // Parameterized constructor
    QuestionBankManager(int numQB, string* subjectArray, string* fnameArray) {
        numberOfQuestionBanks = numQB;
        questionBank = new QuestionBank[this->numberOfQuestionBanks];

        for (int i = 0; i < this->numberOfQuestionBanks; i++)
        {
            QuestionBank q(fnameArray[i], subjectArray[i]);
            q.loadQuestions(fnameArray[i], subjectArray[i]);
            questionBank[i] = q;
        }
    }

    // Copy constructor
    QuestionBankManager(const QuestionBankManager& other) {
        numberOfQuestionBanks = other.numberOfQuestionBanks;
        questionBank = new QuestionBank[numberOfQuestionBanks];

        for (int i = 0; i < numberOfQuestionBanks; i++) {
            questionBank[i] = other.questionBank[i];
        }
    }

    // Destructor
    ~QuestionBankManager() {
        delete[] questionBank;
    }

    // Getter methods
    QuestionBank* getQuestionBank() const {
        return questionBank;
    }

    int getNumberOfQuestionBanks() const {
        return numberOfQuestionBanks;
    }

    // Setter methods
    void setQuestionBank(QuestionBank* qb) {
        questionBank = qb;
    }

    void setNumberOfQuestionBanks(int numQB) {
        numberOfQuestionBanks = numQB;
    }
};

class Quiz {
private:
    int numberOfQuestions;
    int numDistinctTopics;
    Question* questions;
    string subject;
    string* topics;
    int* topicQuestions;
    tm quiz_time;

    int getIndexOfTopic(string* tp, string required_tp, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (tp[i] == required_tp)
            {
                return i;
            }
        }
        return -1;
    }

public:
    // Default constructor
    Quiz() {
        numberOfQuestions = 0;
        numDistinctTopics = 0;
        subject = "\0";
        topics = nullptr;
        topicQuestions = nullptr;
        questions = nullptr;
    }

    // Parameterized constructor
    Quiz(string subj, string* topicsArr, int* quesArr, int sizeOfQuesArr, QuestionBankManager q) {
        int numQues = 0;
        for (int i = 0; i < sizeOfQuesArr; i++)
        {
            numQues += quesArr[i];
        }

        numberOfQuestions = numQues;
        subject = subj;
        topics = new string[numberOfQuestions];
        topicQuestions = new int[numberOfQuestions];
        numDistinctTopics = sizeOfQuesArr;

        for (int i = 0; i < sizeOfQuesArr; i++)
        {
            topics[i] = topicsArr[i];
            topicQuestions[i] = quesArr[i];
        }

        //finding what question bank contains our subject
        int questionBankIdx = -1;
        for (int i = 0; i < q.getNumberOfQuestionBanks(); i++)
        {
            if (subj == q.questionBank[i].getSubject())
            {
                questionBankIdx = i;
                break;
            }
        }

        //to keep track of how many questions we have chosen
        int* chosenQuestionsCount = new int[sizeOfQuesArr];
        for (int i = 0; i < sizeOfQuesArr; i++)
        {
            chosenQuestionsCount[i] = 0;
        }

        int* arr = nullptr; // initialize pointer to null
        int numElements = 0; // number of elements in array

        cout << "Choosing randomized questions from provided topic(s)...\n";
        for (int j = 0; j < sizeOfQuesArr; j++)
        {
            int* newArr = nullptr;
            //loop until we have not chosen the required amount of questions
            while (chosenQuestionsCount[j] != topicQuestions[j])
            {
                // generate random number between 0 and the number of questions in the topic
                int randNum = 0;
                bool found = false;
                do
                {
                    // initialize random seed
                    srand(time(nullptr));
                    randNum = rand() % q.questionBank[questionBankIdx].getNumQuestions();
                    found = false;

                    // check if randNum is already in array or if the question topic doesn't match ours
                    for (int i = 0; i < numElements; i++)
                    {
                        if (arr[i] == randNum)
                        {
                            found = true;
                            break;
                        }
                        else if (q.questionBank[questionBankIdx].questions[randNum].getTopic() != this->topics[j])
                        {
                            found = true;
                            break;
                        }
                    }
                } while (found);

                chosenQuestionsCount[j]++;
                // add randNum to array if not already present
                numElements++; // increment number of elements in array
                newArr = new int[numElements]; // create new array with one additional element

                // copy elements from old array to new array
                for (int i = 0; i < numElements - 1; i++)
                {
                    newArr[i] = arr[i];
                }

                // add randNum to end of new array
                newArr[numElements - 1] = randNum;

                // free old array memory
                delete[] arr;
                // assign new array to arr pointer
                arr = newArr;
            }
        }

        //storing chosen questions in questions array
        questions = new Question[numElements];
        for (int i = 0; i < numElements; i++) {
            questions[i]=q.questionBank[questionBankIdx].questions[arr[i]];
        }

        //displaying chosen questions
        this->displayQuestions();

        //TODO: now we have to implement and store time functionality


        // free memory allocated to array
        delete[] arr;
        arr = nullptr;
        delete[] chosenQuestionsCount;
        chosenQuestionsCount = nullptr;
    }

    // Copy constructor
    Quiz(const Quiz& quiz) {
        numberOfQuestions = quiz.numberOfQuestions;
        subject = quiz.subject;
        topics = new string[numberOfQuestions];
        topicQuestions = new int[numberOfQuestions];

        for (int i = 0; i < numberOfQuestions; i++) {
            questions[i] = quiz.questions[i];
        }
        for (int i = 0; i<numDistinctTopics; i++) {
            topics[i] = quiz.topics[i];
            topicQuestions[i] = quiz.topicQuestions[i];
        }

        quiz_time = quiz.quiz_time;
    }

    // Destructor
    ~Quiz() {
        delete[] topics;
        delete[] topicQuestions;
        delete[] questions;
    }

    // Getters and setters
    int getNumberOfQuestions() const { return numberOfQuestions; }
    void setNumberOfQuestions(int numQues) { numberOfQuestions = numQues; }

    string getSubject() const { return subject; }
    void setSubject(string subj) { subject = subj; }

    string* getTopics() const { return topics; }
    void setTopics(string* topicsArr) { topics = topicsArr; }

    int* getTopicQuestions() const { return topicQuestions; }
    void setTopicQuestions(int* quesArr) { topicQuestions = quesArr; }

    tm getQuizTime() const { return quiz_time; }
    void setQuizTime(tm quizTime) { quiz_time = quizTime; }

    //Functions
    void displayQuestions() {
        cout << "Subject: " << subject << endl;
        cout << "Number of questions: " << numberOfQuestions << endl;
        cout << "Distinct topics: " << numDistinctTopics << endl;
        //cout << "Quiz time: " << asctime(&quiz_time) << endl;
        cout << endl;

        for (int i = 0; i < numberOfQuestions; i++) {
            cout << "Question " << i + 1 << ":" << endl;
            questions[i].displayQuestion();
            cout << endl;
        }
    }
};


// // Set quiz date and time
// int quiz_day = 7;
// int quiz_month = 5;
// int quiz_year = 2023;
// int quiz_hour = 16;
// int quiz_minute = 35;

// // Convert quiz date and time to struct tm
// struct tm quiz_time = {};
// quiz_time.tm_year = quiz_year - 1900;
// quiz_time.tm_mon = quiz_month - 1;
// quiz_time.tm_mday = quiz_day;
// quiz_time.tm_hour = quiz_hour;
// quiz_time.tm_min = quiz_minute;

// // Calculate quiz end time
// time_t quiz_end_time = mktime(&quiz_time) + 1 * 60; // add 60 minutes

// // Get current time
// time_t now = time(nullptr);

// // Check if quiz is open
// if (now > quiz_end_time) {
//     cout << "Sorry, the quiz is no longer available." << endl;
//     break;
// } else {
//     cout << "You can attempt the quiz." << endl;
// }




int main()
{
    //preleminaries
    string STUDENT_CSV_FILE = "course.csv";
    string TEACHER_TXT_FILE = "teacher.txt";
    string QUESTION_OOP_TXT_FILE = "questions_oop.txt";
    string QUESTION_AI_TXT_FILE = "questions_ai.txt";
    string QUESTION_ICT_TXT_FILE = "questions_ict.txt";
    string SUBJECT_OOP = "oop";
    string SUBJECT_AI = "ai";
    string SUBJECT_ICT = "ict";
    int ROWS_TO_SKIP = 2;
    int NUM_QUESTION_BANKS = 3;

    string* subjArray = new string[NUM_QUESTION_BANKS];
    subjArray[0] = SUBJECT_OOP;
    subjArray[1] = SUBJECT_AI;
    subjArray[2] = SUBJECT_ICT;

    string* fnameArray = new string[NUM_QUESTION_BANKS];
    fnameArray[0] = QUESTION_OOP_TXT_FILE;
    fnameArray[1] = QUESTION_AI_TXT_FILE;
    fnameArray[2] = QUESTION_ICT_TXT_FILE;



    // Create a Department object
    //Department d(STUDENT_CSV_FILE, TEACHER_TXT_FILE, ROWS_TO_SKIP);
    // Create a Login object and call the appropriate login function
    Login l;
    // Create Question Bank Manager, consisting of multiple question banks
    QuestionBankManager q(NUM_QUESTION_BANKS, subjArray, fnameArray);
    //Generate Quiz
    //(string subj, string* topicsArr, int* quesArr, int sizeOfQuesArr, QuestionBankManager q)
    string topics[2] = { "Object Oriented Programming","General questions" };
    int numQuestions[2] = { 2,2 };
    Quiz myQuiz("oop", topics, numQuestions, 2, q);

    cout << "end";



    // Display a menu asking the user to select the login type
    // int loginType = l.getLoginType();
    // int studentIdx = -1;
    // int teacherIdx = -1;
    // if (loginType == 0) {
    //     studentIdx = l.performLogin(d.getStudentArray(), d.getTeacherArray(), d.getNumStudents(), d.getNumTeachers(), 0);
    // } else if (loginType == 1) {
    //     teacherIdx = l.performLogin(d.getStudentArray(), d.getTeacherArray(), d.getNumStudents(), d.getNumTeachers(), 1);
    // }

    // if(loginType==0 && studentIdx!=-1) //Student
    // {
    //     int option = l.changePassword();
    //     if(option==0) //change password
    //     {
    //         bool success= d.getStudentArray()[studentIdx].changePassword();
    //         while(success == 0)
    //         {
    //             cout << "\nTry again\n";
    //             success = d.getStudentArray()[studentIdx].changePassword();
    //         }
    //     }
    // }
    // else if (loginType==1 && teacherIdx!=-1) //Teacher
    // {
    //     int option = l.changePassword();
    //     if(option==0) //change password
    //     {
    //         bool success= d.getTeacherArray()[teacherIdx].changePassword();
    //         while(success == 0)
    //         {
    //             cout << "\nTry again\n";
    //             success = d.getTeacherArray()[teacherIdx].changePassword();
    //         }
    //     }
    // }



    return 0;
}
