#include "CourseManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
 
CourseManager::CourseManager() 
{
    loadFromFile();
}

bool CourseManager::isCourseIdExist(const std::string& courseId) const {
    for (const auto& c : courses) {
        if (c.getCourseId() == courseId) return true;
    }
    return false;
}
 
bool CourseManager::isScoreExist(const std::string& studentId, const std::string& courseId) const {
    for (const auto& sc : scores) {
        if (sc.getStudentId() == studentId && sc.getCourseId() == courseId)
            return true;
    }
    return false;
}

bool CourseManager::addCourse(const std::string& courseId, const std::string& courseName,int credits, const std::string& semester) {
    if (!Validator::isValidId(courseId)) {
        std::cout << "[LOI] ID mon hoc khong hop le!\n";
        return false;
    }
    if (!Validator::isNotEmpty(courseName)) {
        std::cout << "[LOI] Ten mon hoc khong duoc de trong!\n";
        return false;
    }
    if (credits <= 0 || credits > 10) {
        std::cout << "[LOI] So tin chi phai tu 1 den 10!\n";
        return false;
    }
    if (isCourseIdExist(courseId)) {
        std::cout << "[LOI] Ma mon hoc '" << courseId << "' da ton tai!\n";
        return false;
    }
 
    courses.emplace_back(courseId, courseName, credits, semester);
    std::cout << "[OK] Da them mon hoc: " << courseName
              << " (" << courseId << ") - " << credits << " tin chi\n";
    return true;
}
 
bool CourseManager::removeCourse(const std::string& courseId) 
{
    for (auto it = courses.begin(); it != courses.end(); ++it) 
    {
        if (it->getCourseId() == courseId) {
            std::cout << "[OK] Da xoa mon hoc: " << it->getCourseName()
                      << " (" << courseId << ")\n";
            courses.erase(it);

            scores.erase(
                std::remove_if(scores.begin(), scores.end(),
                    [&courseId](const Score& sc){ return sc.getCourseId() == courseId; }),
                scores.end());
 
            return true;
        }
    }
    std::cout << "[LOI] Khong tim thay mon hoc co ID: " << courseId << "\n";
    return false;
}
 
Course* CourseManager::searchCourse(const std::string& courseId) 
{
    for (auto& c : courses) {
        if (c.getCourseId() == courseId) return &c;
    }
    return nullptr;
}
 
void CourseManager::displayAllCourses() const 
{
    if (courses.empty()) {
        std::cout << "[THONG BAO] Chua co mon hoc nao.\n";
        return;
    }
    std::cout << "\n============================================================\n";
    std::cout << "               DANH SACH MON HOC (" << courses.size() << " mon)\n";
    std::cout << "============================================================\n";
    std::cout << std::left
              << std::setw(10) << "Ma MH"
              << std::setw(30) << "Ten mon hoc"
              << std::setw(10) << "Tin chi"
              << "Hoc ky\n";
    std::cout << "------------------------------------------------------------\n";
    for (const auto& c : courses) 
    {
        c.display();
    }
    std::cout << "============================================================\n";
}

bool CourseManager::addScore(const std::string& studentId, const std::string& courseId, double midScore, double finalScore, const std::string& semester) {
    if (!Validator::isValidId(studentId)) 
    {
        std::cout << "[LOI] ID sinh vien khong hop le!\n";
        return false;
    }
    if (!isCourseIdExist(courseId)) 
    {
        std::cout << "[LOI] Ma mon hoc '" << courseId << "' khong ton tai!\n";
        return false;
    }
    if (!Validator::isValidScore(midScore)) 
    {
        std::cout << "[LOI] Diem giua ky phai tu 0.0 den 10.0!\n";
        return false;
    }
    if (!Validator::isValidScore(finalScore)) 
    {
        std::cout << "[LOI] Diem cuoi ky phai tu 0.0 den 10.0!\n";
        return false;
    }
    if (isScoreExist(studentId, courseId)) 
    {
        std::cout << "[LOI] Sinh vien " << studentId
                  << " da co diem mon " << courseId << "!\n";
        return false;
    }
 
    scores.emplace_back(studentId, courseId, midScore, finalScore, semester);
    double avg = midScore * 0.4 + finalScore * 0.6;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[OK] Da nhap diem - GK: " << midScore
              << " | CK: " << finalScore
              << " | Trung binh: " << avg << "\n";
    return true;
}
 
bool CourseManager::removeScore(const std::string& studentId, const std::string& courseId) {
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        if (it->getStudentId() == studentId && it->getCourseId() == courseId) 
        {
            scores.erase(it);
            std::cout << "[OK] Da xoa diem cua SV " << studentId
                      << " mon " << courseId << "\n";
            return true;
        }
    }
    std::cout << "[LOI] Khong tim thay diem cua SV " << studentId << " mon " << courseId << "\n";
    return false;
}
 
void CourseManager::displayScoresByCourse(const std::string& courseId, const std::vector<Student>& students) const {
    Course* course = nullptr;
    for (const auto& c : courses) 
    {
        if (c.getCourseId() == courseId) 
        {
            course = const_cast<Course*>(&c);
            break;
        }
    }
    if (!course) 
    {
        std::cout << "[LOI] Khong tim thay mon hoc: " << courseId << "\n";
        return;
    }
 
    std::cout << "\n============================================================\n";
    std::cout << "  BANG DIEM: " << course->getCourseName() << " (" << courseId << ")\n";
    std::cout << "============================================================\n";
    std::cout << std::left << std::setw(12) << "Ma SV" << std::setw(25) << "Ho ten" << std::setw(10) << "Giua ky" << std::setw(10) << "Cuoi ky" << std::setw(12) << "Trung binh" << "Xep loai\n";
    std::cout << "------------------------------------------------------------\n";
 
    int count = 0;
    for (const auto& sc : scores) 
    {
        if (sc.getCourseId() != courseId) continue;
 
        std::string svName = sc.getStudentId();
        for (const auto& sv : students) {
            if (sv.getId() == sc.getStudentId()) 
            {
                svName = sv.getName();
                break;
            }
        }
 
        double avg = sc.calcAvg();
        std::string xepLoai;
        if      (avg >= 9.0) xepLoai = "Xuat sac";
        else if (avg >= 8.0) xepLoai = "Gioi";
        else if (avg >= 7.0) xepLoai = "Kha";
        else if (avg >= 5.0) xepLoai = "Trung binh";
        else                 xepLoai = "Yeu";
 
        std::cout << std::fixed << std::setprecision(1)
                  << std::left
                  << std::setw(12) << sc.getStudentId()
                  << std::setw(25) << svName
                  << std::setw(10) << sc.getMidScore()
                  << std::setw(10) << sc.getFinalScore()
                  << std::setw(12) << avg
                  << xepLoai << "\n";
        count++;
    }
 
    if (count == 0)
        std::cout << "  (Chua co diem nao cho mon hoc nay)\n";
 
    std::cout << "============================================================\n";
    std::cout << "  Tong: " << count << " sinh vien\n";
}

std::vector<StudentRanking> CourseManager::getTopStudents( const std::vector<Student>& students, int topN) 
const {

    std::map<std::string, std::pair<double, int>> sumMap;
 
    for (const auto& sc : scores) 
    {
        sumMap[sc.getStudentId()].first  += sc.calcAvg();
        sumMap[sc.getStudentId()].second += 1;
    }
 
    std::vector<StudentRanking> ranking;
    for (const auto& entry : sumMap) 
    {
        std::string svName = entry.first;
        for (const auto& sv : students) 
        {
            if (sv.getId() == entry.first) 
            {
                svName = sv.getName();
                break;
            }
        }
        double avg = entry.second.second > 0 ? entry.second.first / entry.second.second : 0.0;
        ranking.push_back({entry.first, svName, avg});
    }

    std::sort(ranking.begin(), ranking.end(),
        [](const StudentRanking& a, const StudentRanking& b)
        {
            return a.avgScore > b.avgScore;
        });
 
    if (topN > 0 && topN < static_cast<int>(ranking.size())) ranking.resize(topN);
 
    std::cout << "\n============================================================\n";
    std::cout << "  XEP HANG SINH VIEN" << (topN > 0 ? " (TOP " + std::to_string(topN) + ")" : "") << "\n";
    std::cout << "============================================================\n";
    std::cout << std::left << std::setw(6)  << "Hang" << std::setw(12) << "Ma SV" << std::setw(25) << "Ho ten" << "Diem TB\n";
    std::cout << "------------------------------------------------------------\n";
    int rank = 1;
    for (const auto& r : ranking) 
    {
        std::cout << std::fixed << std::setprecision(2)<< std::left<< std::setw(6)  << rank++ << std::setw(12) << r.studentId << std::setw(25) << r.studentName << r.avgScore << "\n";
    }
    std::cout << "============================================================\n";
 
    return ranking;
}

void CourseManager::classifyStudents(const std::vector<Student>& students) const 
{
    std::cout << "\n============================================================\n";
    std::cout << "              PHAN LOAI HOC LUC SINH VIEN\n";
    std::cout << "============================================================\n";
 
    int xuat_sac = 0, gioi = 0, kha = 0, tb = 0, yeu = 0, chua_co = 0;
    auto ranking = getTopStudents(students); 
 
    for (const auto& r : ranking) 
    {
        if      (r.avgScore >= 9.0) xuat_sac++;
        else if (r.avgScore >= 8.0) gioi++;
        else if (r.avgScore >= 7.0) kha++;
        else if (r.avgScore >= 5.0) tb++;
        else                        yeu++;
    }

    for (const auto& sv : students) 
    {
        bool found = false;
        for (const auto& r : ranking) {
            if (r.studentId == sv.getId()) { found = true; break; }
        }
        if (!found) chua_co++;
    }
 
    std::cout << "  Xuat sac (>= 9.0) : " << xuat_sac << " sinh vien\n";
    std::cout << "  Gioi    (>= 8.0)  : " << gioi     << " sinh vien\n";
    std::cout << "  Kha     (>= 7.0)  : " << kha      << " sinh vien\n";
    std::cout << "  Trung binh(>= 5.0): " << tb       << " sinh vien\n";
    std::cout << "  Yeu     (< 5.0)   : " << yeu      << " sinh vien\n";
    std::cout << "  Chua co diem      : " << chua_co  << " sinh vien\n";
    std::cout << "============================================================\n";
}

bool CourseManager::saveToFile() 
{
    std::vector<std::string> courseLines;
    for (const auto& c : courses) {
        std::ostringstream oss;
        oss << c.getCourseId() << "|"<< c.getCourseName() << "|"<< c.getCredits() << "|"<< c.getSemester();
        courseLines.push_back(oss.str());
    }
 
    std::vector<std::string> scoreLines;
    for (const auto& sc : scores) 
    {
        std::ostringstream oss;
        oss << sc.getStudentId() << "|"<< sc.getCourseId() << "|"<< sc.getMidScore() << "|"<< sc.getFinalScore() << "|"<< sc.getSemester();
        scoreLines.push_back(oss.str());
    }
 
    bool ok1 = fileManager.writeFile(COURSE_FILE, courseLines);
    bool ok2 = fileManager.writeFile(SCORE_FILE,  scoreLines);
 
    if (ok1 && ok2) 
    {
        std::cout << "[OK] Da luu " << courses.size() << " mon hoc va " << scores.size() << " ban ghi diem.\n";
        return true;
    }
    std::cout << "[LOI] Co loi khi luu file!\n";
    return false;
}
 
bool CourseManager::loadFromFile() 
{
    bool loaded = false;

    if (fileManager.fileExists(COURSE_FILE)) 
    {
        auto lines = fileManager.readFile(COURSE_FILE);
        courses.clear();
        for (const auto& line : lines) 
        {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string courseId, courseName, creditsStr, semester;
            if (std::getline(ss, courseId,   '|') && std::getline(ss, courseName, '|') && std::getline(ss, creditsStr, '|') && std::getline(ss, semester)) 
            {
                try 
                {
                    int credits = std::stoi(creditsStr);
                    courses.emplace_back(courseId, courseName, credits, semester);
                } catch (...) 
                {
                    std::cout << "[CANH BAO] Loi doc dong courses: " << line << "\n";
                }
            }
        }
        if (!courses.empty())
            std::cout << "[OK] Da tai " << courses.size() << " mon hoc.\n";
        loaded = true;
    }
 
    if (fileManager.fileExists(SCORE_FILE)) 
    {
        auto lines = fileManager.readFile(SCORE_FILE);
        scores.clear();
        for (const auto& line : lines) 
        {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string studentId, courseId, midStr, finalStr, semester;
            if (std::getline(ss, studentId, '|') && std::getline(ss, courseId,  '|') && std::getline(ss, midStr,    '|') && std::getline(ss, finalStr,  '|') && std::getline(ss, semester)) 
            {
                try 
                {
                    double mid   = std::stod(midStr);
                    double final_= std::stod(finalStr);
                    scores.emplace_back(studentId, courseId, mid, final_, semester);
                } catch (...) 
                {
                    std::cout << "[CANH BAO] Loi doc dong scores: " << line << "\n";
                }
            }
        }
        if (!scores.empty())
            std::cout << "[OK] Da tai " << scores.size() << " ban ghi diem.\n";
        loaded = true;
    }
 
    return loaded;
}

int CourseManager::getCourseCount() const { return static_cast<int>(courses.size()); }
int CourseManager::getScoreCount()  const { return static_cast<int>(scores.size()); }
const std::vector<Course>& CourseManager::getCourses() const { return courses; }