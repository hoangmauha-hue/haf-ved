#include "StudentManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;
 
// Duong dan file luu du lieu
const string StudentManager::FILE_PATH = "data/students.txt";
 
StudentManager::StudentManager() {
    loadFromFile();
}
 
StudentManager::~StudentManager() {}
 
bool StudentManager::isIdExist(const string& id) const {
    for (const auto& s : m_students) {
        if (s.getId() == id) return true;
    }
    return false;
}
 
bool StudentManager::addStudent(const string& name, const string& id,
                                const string& studentCode,
                                const string& className, double gpa) {
    if (!Validator::isNotEmpty(name)) {
        cout << "[LOI] Ten sinh vien khong duoc de trong!\n";
        return false;
    }
    if (!Validator::isValidId(id)) {
        cout << "[LOI] ID khong hop le!\n";
        return false;
    }
    if (!Validator::isValidScore(gpa)) {
        cout << "[LOI] GPA phai trong khoang 0.0 - 10.0!\n";
        return false;
    }
    if (isIdExist(id)) {
        cout << "[LOI] ID '" << id << "' da ton tai!\n";
        return false;
    }
 
    m_students.emplace_back(name, id, studentCode, className, gpa);
    cout << "[OK] Da them sinh vien: " << name << " (ID: " << id << ")\n";
    return true;
}
 
bool StudentManager::removeStudent(const string& id) {
    for (auto it = m_students.begin(); it != m_students.end(); ++it) {
        if (it->getId() == id) {
            cout << "[OK] Da xoa sinh vien: " << it->getName()
                 << " (ID: " << id << ")\n";
            m_students.erase(it);
            return true;
        }
    }
    cout << "[LOI] Khong tim thay sinh vien co ID: " << id << "\n";
    return false;
}
 
bool StudentManager::updateStudent(const string& id, const string& name,
                                   const string& className, double gpa) {
    Student* sv = searchById(id);
    if (!sv) {
        cout << "[LOI] Khong tim thay sinh vien co ID: " << id << "\n";
        return false;
    }
    if (!name.empty())      sv->setName(name);
    if (!className.empty()) sv->setClassName(className);
    if (gpa >= 0.0 && gpa <= 10.0) sv->setGpa(gpa);
    cout << "[OK] Da cap nhat sinh vien ID: " << id << "\n";
    return true;
}
 
Student* StudentManager::searchById(const string& id) {
    for (auto& s : m_students) {
        if (s.getId() == id) return &s;
    }
    return nullptr;
}
 
vector<Student*> StudentManager::searchByName(const string& name) {
    vector<Student*> result;
    string lowerName = name;
    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
 
    for (auto& s : m_students) {
        string lowerStudentName = s.getName();
        transform(lowerStudentName.begin(), lowerStudentName.end(),
                  lowerStudentName.begin(), ::tolower);
        if (lowerStudentName.find(lowerName) != string::npos)
            result.push_back(&s);
    }
    return result;
}
 
void StudentManager::displayAll() const {
    if (m_students.empty()) {
        cout << "[THONG BAO] Danh sach sinh vien trong.\n";
        return;
    }
    cout << "\n============================================================\n";
    cout << "        DANH SACH SINH VIEN (" << m_students.size() << " nguoi)\n";
    cout << "============================================================\n";
    cout << left << setw(5)  << "STT"
                 << setw(12) << "ID"
                 << setw(25) << "Ho ten"
                 << setw(10) << "Ma SV"
                 << setw(10) << "Lop"
                 << "GPA\n";
    cout << "------------------------------------------------------------\n";
    int stt = 1;
    for (const auto& s : m_students) {
        cout << left << setw(5)  << stt++
                     << setw(12) << s.getId()
                     << setw(25) << s.getName()
                     << setw(10) << s.getStudentCode()
                     << setw(10) << s.getClassName()
                     << fixed << setprecision(2) << s.getGpa() << "\n";
    }
    cout << "============================================================\n";
}
 
void StudentManager::sortByGpa(bool descending) {
    sort(m_students.begin(), m_students.end(),
        [descending](const Student& a, const Student& b) {
            return descending ? a.getGpa() > b.getGpa()
                              : a.getGpa() < b.getGpa();
        });
    cout << "[OK] Da sap xep theo GPA "
         << (descending ? "(giam dan).\n" : "(tang dan).\n");
}
 
bool StudentManager::saveToFile() {
    vector<string> lines;
    for (const auto& s : m_students) {
        ostringstream oss;
        oss << s.getName()        << "|"
            << s.getId()          << "|"
            << s.getStudentCode() << "|"
            << s.getClassName()   << "|"
            << s.getGpa();
        lines.push_back(oss.str());
    }
    if (FileManager::writeFile(FILE_PATH, lines)) {
        cout << "[OK] Da luu " << m_students.size()
             << " sinh vien vao " << FILE_PATH << "\n";
        return true;
    }
    cout << "[LOI] Khong the luu file " << FILE_PATH << "\n";
    return false;
}
 
bool StudentManager::loadFromFile() {
    if (!FileManager::fileExists(FILE_PATH)) return false;
 
    auto lines = FileManager::readFile(FILE_PATH);
    m_students.clear();
    int loaded = 0;
 
    for (const auto& line : lines) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, id, studentCode, className, gpaStr;
        if (getline(ss, name,        '|') &&
            getline(ss, id,          '|') &&
            getline(ss, studentCode, '|') &&
            getline(ss, className,   '|') &&
            getline(ss, gpaStr))
        {
            try {
                double gpa = stod(gpaStr);
                m_students.emplace_back(name, id, studentCode, className, gpa);
                loaded++;
            } catch (...) {
                cout << "[CANH BAO] Dong bi loi khi doc file, bo qua: " << line << "\n";
            }
        }
    }
    if (loaded > 0)
        cout << "[OK] Da tai " << loaded << " sinh vien tu " << FILE_PATH << "\n";
    return true;
}
 
int StudentManager::getCount() const {
    return static_cast<int>(m_students.size());
}
 
const vector<Student>& StudentManager::getStudents() const {
    return m_students;
}
