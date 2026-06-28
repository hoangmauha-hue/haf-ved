#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H
 
#include <string>
#include <vector>
#include "Course.h"
#include "Score.h"
#include "Student.h"
#include "FileManager.h"
#include "Validator.h"
 
// Struct dung de xep hang sinh vien
struct StudentRanking {
    std::string studentId;
    std::string studentName;
    double      avgScore;
};
 
// ============================================================
//  CourseManager.h - Quan ly mon hoc va diem so
// ============================================================
class CourseManager {
private:
    std::vector<Course> m_courses;
    std::vector<Score>  m_scores;
 
    static const std::string COURSE_FILE;
    static const std::string SCORE_FILE;
 
    bool isCourseIdExist(const std::string& courseId) const;
    bool isScoreExist(const std::string& studentId,
                      const std::string& courseId) const;
 
public:
    CourseManager();
    ~CourseManager();
 
    // --- Quan ly mon hoc ---
    bool    addCourse(const std::string& courseId, const std::string& courseName,
                      int credits, const std::string& semester);
    bool    removeCourse(const std::string& courseId);
    Course* searchCourse(const std::string& courseId);
    void    displayAllCourses() const;
 
    // --- Quan ly diem ---
    bool addScore(const std::string& studentId, const std::string& courseId,
                  double midScore, double finalScore, const std::string& semester);
    bool removeScore(const std::string& studentId, const std::string& courseId);
    void displayScoresByCourse(const std::string& courseId,
                               const std::vector<Student>& students) const;
    void displayScoresByStudent(const std::string& studentId) const;
 
    // --- Thong ke / xep hang ---
    std::vector<StudentRanking> getTopStudents(
        const std::vector<Student>& students, int topN = 0) const;
    void classifyStudents(const std::vector<Student>& students) const;
 
    // --- File ---
    bool saveToFile();
    bool loadFromFile();
 
    // --- Getters ---
    int getCourseCount() const;
    int getScoreCount()  const;
    const std::vector<Course>& getCourses() const;
    const std::vector<Score>&  getScores()  const;
};
 
#endif // COURSEMANAGER_H
