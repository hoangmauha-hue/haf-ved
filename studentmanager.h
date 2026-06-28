#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H
#include "Student.h"
#include "FileManager.h"
#include "Validator.h"
#include <vector>
#include <string>
#include <algorithm>

class StudentManager 
{
private:
    std::vector<Student> students;
    FileManager fileManager;
    const std::string FILE_PATH = "students.txt";
    bool isIdExist(const std::string& id) const;

public:
    StudentManager();

    bool addStudent(const std::string& name, const std::string& id, const std::string& studentCode, const std::string& className,double gpa);
    bool removeStudent(const std::string& id);
    Student* searchById(const std::string& id);
    std::vector<Student*> searchByName(const std::string& name);

    void displayAll() const;
    void sortByGPA(bool descending = true);

    bool saveToFile();
    bool loadFromFile();

    int getCount() const;
    const std::vector<Student>& getStudents() const;
};
#endif