#include "CourseManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
using namespace std;
 
// Duong dan file luu du lieu
const string CourseManager::COURSE_FILE = "courses.txt";
const string CourseManager::SCORE_FILE  = "scores.txt";
 
CourseManager::CourseManager() {
    loadFromFile();
}
 
CourseManager::~CourseManager() {}
 
// ── Private helpers
bool CourseManager::isCourseIdExist(const string& courseId) const {
    for (const auto& c : m_courses) {
        if (c.getCourseId() == courseId) return true;
    }
    return false;
}
 
bool CourseManager::isScoreExist(const string& studentId,
                                  const string& courseId) const {
    for (const auto& sc : m_scores) {
        if (sc.getStudentId() == studentId && sc.getCourseId() == courseId)
            return true;
    }
    return false;
}
 
// ── Quan ly mon hoc
bool CourseManager::addCourse(const string& courseId, const string& courseName,
                               int credits, const string& semester) {
    if (!Validator::isValidId(courseId)) {
        cout << "[LOI] ID mon hoc khong hop le!\n";
        return false;
    }
    if (!Validator::isNotEmpty(courseName)) {
        cout << "[LOI] Ten mon hoc khong duoc de trong!\n";
        return false;
    }
    if (!Validator::isValidCredits(credits)) {
        cout << "[LOI] So tin chi phai tu 1 den 10!\n";
        return false;
    }
    if (isCourseIdExist(courseId)) {
        cout << "[LOI] Ma mon hoc '" << courseId << "' da ton tai!\n";
        return false;
    }
 
    m_courses.emplace_back(courseId, courseName, credits, semester);
    cout << "[OK] Da them mon hoc: " << courseName
         << " (" << courseId << ") - " << credits << " tin chi\n";
    return true;
}
 
bool CourseManager::removeCourse(const string& courseId) {
    for (auto it = m_courses.begin(); it != m_courses.end(); ++it) {
        if (it->getCourseId() == courseId) {
            cout << "[OK] Da xoa mon hoc: " << it->getCourseName()
                 << " (" << courseId << ")\n";
            m_courses.erase(it);
 
            // Xoa diem lien quan
            m_scores.erase(
                remove_if(m_scores.begin(), m_scores.end(),
                    [&courseId](const Score& sc) {
                        return sc.getCourseId() == courseId;
                    }),
                m_scores.end());
            return true;
        }
    }
    cout << "[LOI] Khong tim thay mon hoc co ID: " << courseId << "\n";
    return false;
}
 
Course* CourseManager::searchCourse(const string& courseId) {
    for (auto& c : m_courses) {
        if (c.getCourseId() == courseId) return &c;
    }
    return nullptr;
}
 
void CourseManager::displayAllCourses() const {
    if (m_courses.empty()) {
        cout << "[THONG BAO] Chua co mon hoc nao.\n";
        return;
    }
    cout << "\n============================================================\n";
    cout << "        DANH SACH MON HOC (" << m_courses.size() << " mon)\n";
    cout << "============================================================\n";
    cout << left << setw(10) << "Ma MH"
                 << setw(30) << "Ten mon hoc"
                 << setw(10) << "Tin chi"
                 << "Hoc ky\n";
    cout << "------------------------------------------------------------\n";
    for (const auto& c : m_courses)
        c.display();
    cout << "============================================================\n";
}
 
// ── Quan ly diem
bool CourseManager::addScore(const string& studentId, const string& courseId,
                              double midScore, double finalScore,
                              const string& semester) {
    if (!Validator::isValidId(studentId)) {
        cout << "[LOI] ID sinh vien khong hop le!\n";
        return false;
    }
    if (!isCourseIdExist(courseId)) {
        cout << "[LOI] Ma mon hoc '" << courseId << "' khong ton tai!\n";
        return false;
    }
    if (!Validator::isValidScore(midScore)) {
        cout << "[LOI] Diem giua ky phai tu 0.0 den 10.0!\n";
        return false;
    }
    if (!Validator::isValidScore(finalScore)) {
        cout << "[LOI] Diem cuoi ky phai tu 0.0 den 10.0!\n";
        return false;
    }
    if (isScoreExist(studentId, courseId)) {
        cout << "[LOI] Sinh vien " << studentId
             << " da co diem mon " << courseId << "!\n";
        return false;
    }
 
    m_scores.emplace_back(studentId, courseId, midScore, finalScore, semester);
    double avg = midScore * 0.4 + finalScore * 0.6;
    cout << fixed << setprecision(2);
    cout << "[OK] Da nhap diem - GK: " << midScore
         << " | CK: " << finalScore
         << " | Trung binh: " << avg << "\n";
    return true;
}
 
bool CourseManager::removeScore(const string& studentId, const string& courseId) {
    for (auto it = m_scores.begin(); it != m_scores.end(); ++it) {
        if (it->getStudentId() == studentId && it->getCourseId() == courseId) {
            m_scores.erase(it);
            cout << "[OK] Da xoa diem cua SV " << studentId
                 << " mon " << courseId << "\n";
            return true;
        }
    }
    cout << "[LOI] Khong tim thay diem cua SV " << studentId
         << " mon " << courseId << "\n";
    return false;
}
 
void CourseManager::displayScoresByCourse(const string& courseId,
                                           const vector<Student>& students) const {
    const Course* course = nullptr;
    for (const auto& c : m_courses) {
        if (c.getCourseId() == courseId) { course = &c; break; }
    }
    if (!course) {
        cout << "[LOI] Khong tim thay mon hoc: " << courseId << "\n";
        return;
    }
 
    cout << "\n============================================================\n";
    cout << "  BANG DIEM: " << course->getCourseName()
         << " (" << courseId << ")\n";
    cout << "============================================================\n";
    cout << left << setw(12) << "Ma SV"
                 << setw(25) << "Ho ten"
                 << setw(10) << "Giua ky"
                 << setw(10) << "Cuoi ky"
                 << setw(12) << "Trung binh"
                 << "Xep loai\n";
    cout << "------------------------------------------------------------\n";
 
    int count = 0;
    for (const auto& sc : m_scores) {
        if (sc.getCourseId() != courseId) continue;
 
        string svName = sc.getStudentId();
        for (const auto& sv : students) {
            if (sv.getId() == sc.getStudentId()) { svName = sv.getName(); break; }
        }
 
        double avg = sc.calcAvg();
        string xepLoai;
        if      (avg >= 9.0) xepLoai = "Xuat sac";
        else if (avg >= 8.0) xepLoai = "Gioi";
        else if (avg >= 7.0) xepLoai = "Kha";
        else if (avg >= 5.0) xepLoai = "Trung binh";
        else                 xepLoai = "Yeu";
 
        cout << fixed << setprecision(1) << left
             << setw(12) << sc.getStudentId()
             << setw(25) << svName
             << setw(10) << sc.getMidScore()
             << setw(10) << sc.getFinalScore()
             << setw(12) << avg
             << xepLoai << "\n";
        count++;
    }
 
    if (count == 0) cout << "  (Chua co diem nao cho mon hoc nay)\n";
    cout << "============================================================\n";
    cout << "  Tong: " << count << " sinh vien\n";
}
 
void CourseManager::displayScoresByStudent(const string& studentId) const {
    cout << "\n============================================================\n";
    cout << "  BANG DIEM SINH VIEN: " << studentId << "\n";
    cout << "============================================================\n";
    cout << left << setw(12) << "Ma MH"
                 << setw(10) << "Giua ky"
                 << setw(10) << "Cuoi ky"
                 << setw(12) << "Trung binh"
                 << "Hoc ky\n";
    cout << "------------------------------------------------------------\n";
 
    int count = 0;
    for (const auto& sc : m_scores) {
        if (sc.getStudentId() != studentId) continue;
        cout << fixed << setprecision(1) << left
             << setw(12) << sc.getCourseId()
             << setw(10) << sc.getMidScore()
             << setw(10) << sc.getFinalScore()
             << setw(12) << sc.calcAvg()
             << sc.getSemester() << "\n";
        count++;
    }
 
    if (count == 0) cout << "  (Sinh vien nay chua co diem)\n";
    cout << "============================================================\n";
}
 
// ── Thong ke
vector<StudentRanking> CourseManager::getTopStudents(
    const vector<Student>& students, int topN) const
{
    map<string, pair<double, int>> sumMap;
    for (const auto& sc : m_scores) {
        sumMap[sc.getStudentId()].first  += sc.calcAvg();
        sumMap[sc.getStudentId()].second += 1;
    }
 
    vector<StudentRanking> ranking;
    for (const auto& entry : sumMap) {
        string svName = entry.first;
        for (const auto& sv : students) {
            if (sv.getId() == entry.first) { svName = sv.getName(); break; }
        }
        double avg = entry.second.second > 0
                   ? entry.second.first / entry.second.second : 0.0;
        ranking.push_back({entry.first, svName, avg});
    }
 
    sort(ranking.begin(), ranking.end(),
        [](const StudentRanking& a, const StudentRanking& b) {
            return a.avgScore > b.avgScore;
        });
 
    if (topN > 0 && topN < static_cast<int>(ranking.size()))
        ranking.resize(topN);
 
    cout << "\n============================================================\n";
    cout << "  XEP HANG SINH VIEN"
         << (topN > 0 ? " (TOP " + to_string(topN) + ")" : "") << "\n";
    cout << "============================================================\n";
    cout << left << setw(6)  << "Hang"
                 << setw(12) << "Ma SV"
                 << setw(25) << "Ho ten"
                 << "Diem TB\n";
    cout << "------------------------------------------------------------\n";
    int rank = 1;
    for (const auto& r : ranking) {
        cout << fixed << setprecision(2) << left
             << setw(6)  << rank++
             << setw(12) << r.studentId
             << setw(25) << r.studentName
             << r.avgScore << "\n";
    }
    cout << "============================================================\n";
 
    return ranking;
}
 
void CourseManager::classifyStudents(const vector<Student>& students) const {
    cout << "\n============================================================\n";
    cout << "          PHAN LOAI HOC LUC SINH VIEN\n";
    cout << "============================================================\n";
 
    auto ranking = getTopStudents(students);
 
    int xuat_sac = 0, gioi = 0, kha = 0, tb = 0, yeu = 0, chua_co = 0;
    for (const auto& r : ranking) {
        if      (r.avgScore >= 9.0) xuat_sac++;
        else if (r.avgScore >= 8.0) gioi++;
        else if (r.avgScore >= 7.0) kha++;
        else if (r.avgScore >= 5.0) tb++;
        else                        yeu++;
    }
 
    for (const auto& sv : students) {
        bool found = false;
        for (const auto& r : ranking) {
            if (r.studentId == sv.getId()) { found = true; break; }
        }
        if (!found) chua_co++;
    }
 
    cout << "  Xuat sac (>= 9.0) : " << xuat_sac << " sinh vien\n";
    cout << "  Gioi    (>= 8.0)  : " << gioi     << " sinh vien\n";
    cout << "  Kha     (>= 7.0)  : " << kha      << " sinh vien\n";
    cout << "  Trung binh(>= 5.0): " << tb       << " sinh vien\n";
    cout << "  Yeu     (< 5.0)   : " << yeu      << " sinh vien\n";
    cout << "  Chua co diem      : " << chua_co  << " sinh vien\n";
    cout << "============================================================\n";
}
 
// ── File
bool CourseManager::saveToFile() {
    vector<string> courseLines;
    for (const auto& c : m_courses)
        courseLines.push_back(c.toString());
 
    vector<string> scoreLines;
    for (const auto& sc : m_scores)
        scoreLines.push_back(sc.toString());
 
    bool ok1 = FileManager::writeFile(COURSE_FILE, courseLines);
    bool ok2 = FileManager::writeFile(SCORE_FILE,  scoreLines);
 
    if (ok1 && ok2) {
        cout << "[OK] Da luu " << m_courses.size() << " mon hoc va "
             << m_scores.size() << " ban ghi diem.\n";
        return true;
    }
    cout << "[LOI] Co loi khi luu file!\n";
    return false;
}
 
bool CourseManager::loadFromFile() {
    bool loaded = false;
 
    if (FileManager::fileExists(COURSE_FILE)) {
        auto lines = FileManager::readFile(COURSE_FILE);
        m_courses.clear();
        for (const auto& line : lines) {
            if (line.empty()) continue;
            stringstream ss(line);
            string courseId, courseName, creditsStr, semester;
            if (getline(ss, courseId,   '|') &&
                getline(ss, courseName, '|') &&
                getline(ss, creditsStr, '|') &&
                getline(ss, semester))
            {
                try {
                    int credits = stoi(creditsStr);
                    m_courses.emplace_back(courseId, courseName, credits, semester);
                } catch (...) {
                    cout << "[CANH BAO] Loi doc dong courses: " << line << "\n";
                }
            }
        }
        if (!m_courses.empty())
            cout << "[OK] Da tai " << m_courses.size() << " mon hoc.\n";
        loaded = true;
    }
 
    if (FileManager::fileExists(SCORE_FILE)) {
        auto lines = FileManager::readFile(SCORE_FILE);
        m_scores.clear();
        for (const auto& line : lines) {
            if (line.empty()) continue;
            stringstream ss(line);
            string studentId, courseId, midStr, finalStr, semester;
            if (getline(ss, studentId, '|') &&
                getline(ss, courseId,  '|') &&
                getline(ss, midStr,    '|') &&
                getline(ss, finalStr,  '|') &&
                getline(ss, semester))
            {
                try {
                    double mid    = stod(midStr);
                    double final_ = stod(finalStr);
                    m_scores.emplace_back(studentId, courseId, mid, final_, semester);
                } catch (...) {
                    cout << "[CANH BAO] Loi doc dong scores: " << line << "\n";
                }
            }
        }
        if (!m_scores.empty())
            cout << "[OK] Da tai " << m_scores.size() << " ban ghi diem.\n";
        loaded = true;
    }
 
    return loaded;
}
 
// ── Getters
int CourseManager::getCourseCount() const { return static_cast<int>(m_courses.size()); }
int CourseManager::getScoreCount()  const { return static_cast<int>(m_scores.size());  }
const vector<Course>& CourseManager::getCourses() const { return m_courses; }
const vector<Score>&  CourseManager::getScores()  const { return m_scores;  }
