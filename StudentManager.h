#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H
 
#include <string>
#include <vector>
#include "Student.h"
#include "FileManager.h"
#include "Validator.h"
 
// ============================================================
//  StudentManager.h - Quan ly danh sach sinh vien
// ============================================================
class StudentManager {
private:
    std::vector<Student> m_students;
    static const std::string FILE_PATH;
 
    bool isIdExist(const std::string& id) const;
 
public:
    StudentManager();
    ~StudentManager();
 
    bool addStudent(const std::string& name, const std::string& id,
                    const std::string& studentCode,
                    const std::string& className, double gpa);
    bool removeStudent(const std::string& id);
    bool updateStudent(const std::string& id, const std::string& name,
                       const std::string& className, double gpa);
 
    Student*              searchById(const std::string& id);
    std::vector<Student*> searchByName(const std::string& name);
 
    void displayAll()          const;
    void sortByGpa(bool descending = true);
 
    bool saveToFile();
    bool loadFromFile();
 
    int getCount() const;
    const std::vector<Student>& getStudents() const;
};
 
#endif // STUDENTMANAGER_H
