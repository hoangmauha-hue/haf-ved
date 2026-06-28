#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H
 
#include "Course.h"
#include "Score.h"
#include "Student.h"
#include "FileManager.h"
#include "Validator.h"
#include <vector>
#include <string>
 
struct StudentRanking 
{
    std::string studentId;
    std::string studentName;
    double avgScore;
};
 
class CourseManager 
{
private:
    std::vector<Course> courses;
    std::vector<Score>  scores;
    FileManager fileManager;
 
    const std::string COURSE_FILE = "courses.txt";
    const std::string SCORE_FILE  = "scores.txt";
 
    bool isCourseIdExist(const std::string& courseId) const;
    bool isScoreExist(const std::string& studentId, const std::string& courseId) const;
 
public:
    CourseManager();
 
    bool addCourse(const std::string& courseId, const std::string& courseName, int credits, const std::string& semester);
    bool removeCourse(const std::string& courseId);
    Course* searchCourse(const std::string& courseId);
    void displayAllCourses() const;

    bool addScore(const std::string& studentId, const std::string& courseId, double midScore, double finalScore, const std::string& semester);
    bool removeScore(const std::string& studentId, const std::string& courseId);
 
    void displayScoresByCourse(const std::string& courseId, const std::vector<Student>& students) const;
    std::vector<StudentRanking> getTopStudents(const std::vector<Student>& students, int topN = 0) const;
    void classifyStudents(const std::vector<Student>& students) const;

    bool saveToFile();
    bool loadFromFile();
 
    int getCourseCount() const;
    int getScoreCount() const;
    const std::vector<Course>& getCourses() const;
};
#endif