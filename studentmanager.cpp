#include "StudentManager.h"
#include <iostream>
#include <sstream>

StudentManager::StudentManager() {
    loadFromFile();
}

bool StudentManager::isIdExist(const std::string& id) const {
    for (const auto& s : students) {
        if (s.getId() == id) return true;
    }
    return false;
}

bool StudentManager::addStudent(const std::string& name, const std::string& id,
                                const std::string& studentCode,
                                const std::string& className, double gpa) {
    // Validate input
    if (!Validator::isNotEmpty(name)) {
        std::cout << "[LOI] Ten sinh vien khong duoc de trong!\n";
        return false;
    }
    if (!Validator::isValidId(id)) {
        std::cout << "[LOI] ID khong hop le (khong duoc rong)!\n";
        return false;
    }
    if (!Validator::isValidScore(gpa)) {
        std::cout << "[LOI] GPA phai trong khoang 0.0 - 10.0!\n";
        return false;
    }
    if (isIdExist(id)) {
        std::cout << "[LOI] ID '" << id << "' da ton tai trong danh sach!\n";
        return false;
    }

    students.emplace_back(name, id, studentCode, className, gpa);
    std::cout << "[OK] Da them sinh vien: " << name << " (ID: " << id << ")\n";
    return true;
}

bool StudentManager::removeStudent(const std::string& id) 
{
    for (auto it = students.begin(); it != students.end(); ++it) 
    {
        if (it->getId() == id) 
        {
            std::cout << "[OK] Da xoa sinh vien: " << it->getName()
                      << " (ID: " << id << ")\n";
            students.erase(it);
            return true;
        }
    }
    std::cout << "[LOI] Khong tim thay sinh vien co ID: " << id << "\n";
    return false;
}

Student* StudentManager::searchById(const std::string& id) 
{
    for (auto& s : students) 
    {
        if (s.getId() == id) return &s;
    }
    return nullptr;
}

std::vector<Student*> StudentManager::searchByName(const std::string& name) 
{
    std::vector<Student*> result;
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    for (auto& s : students) 
    {
        std::string lowerStudentName = s.getName();
        std::transform(lowerStudentName.begin(), lowerStudentName.end(), lowerStudentName.begin(), ::tolower);
        if (lowerStudentName.find(lowerName) != std::string::npos) 
        {
            result.push_back(&s);
        }
    }
    return result;
}

void StudentManager::displayAll() const 
{
    if (students.empty()) {
        std::cout << "[THONG BAO] Danh sach sinh vien trong.\n";
        return;
    }
    std::cout << "\n============================================================\n";
    std::cout << "                DANH SACH SINH VIEN (" << students.size() << " nguoi)\n";
    std::cout << "============================================================\n";
    int stt = 1;
    for (const auto& s : students) 
    {
        std::cout << "--- [" << stt++ << "] ";
        s.display(); 
    }
    std::cout << "============================================================\n";
}

void StudentManager::sortByGPA(bool descending) 
{
    std::sort(students.begin(), students.end(),
        [descending](const Student& a, const Student& b) 
        {
            return descending ? a.getGPA() > b.getGPA() : a.getGPA() < b.getGPA();
        });
    std::cout << "[OK] Da sap xep theo GPA "
              << (descending ? "(giam dan).\n" : "(tang dan).\n");
}

bool StudentManager::saveToFile() 
{
    std::vector<std::string> lines;
    for (const auto& s : students) 
    {
        std::ostringstream oss;
        oss << s.getName() << "|" << s.getId() << "|" << s.getStudentCode() << "|" << s.getClassName() << "|" << s.getGPA();
        lines.push_back(oss.str());
    }
    if (fileManager.writeFile(FILE_PATH, lines)) 
    {
        std::cout << "[OK] Da luu " << students.size() << " sinh vien vao " << FILE_PATH << "\n";
        return true;
    }
    std::cout << "[LOI] Khong the luu file " << FILE_PATH << "\n";
    return false;
}

bool StudentManager::loadFromFile() 
{
    if (!fileManager.fileExists(FILE_PATH)) 
    {
        return false;
    }
    std::vector<std::string> lines = fileManager.readFile(FILE_PATH);
    students.clear();
    int loaded = 0;
    for (const auto& line : lines) 
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, id, studentCode, className, gpaStr;
        if (std::getline(ss, name, '|') &&
            std::getline(ss, id, '|') &&
            std::getline(ss, studentCode, '|') &&
            std::getline(ss, className, '|') &&
            std::getline(ss, gpaStr)) {
            try {
                double gpa = std::stod(gpaStr);
                students.emplace_back(name, id, studentCode, className, gpa);
                loaded++;
            } catch (...) 
            {
                std::cout << "[CANH BAO] Dong bi loi khi doc file, bo qua: " << line << "\n";
            }
        }
    }
    if (loaded > 0)
        std::cout << "[OK] Da tai " << loaded << " sinh vien tu " << FILE_PATH << "\n";
    return true;
}

int StudentManager::getCount() const 
{
    return static_cast<int>(students.size());
}
const std::vector<Student>& StudentManager::getStudents() const 
{
    return students;
}