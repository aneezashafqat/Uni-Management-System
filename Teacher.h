#ifndef TEACHER_H
#define TEACHER_H

#include "AcademicEntity.h"
#include "Constants.h"
#include <string>

class Teacher : public AcademicEntity {
    int feedbackRatings[MAX_FEEDBACK];
    std::string feedbackComments[MAX_FEEDBACK];
    int feedbackCount;
    double avgFeedback;
    std::string assignedCourseIDs[MAX_COURSES];
    int courseCount;

public:
    Teacher();
    Teacher(const std::string& id, const std::string& n, const std::string& e);

    void addFeedback(int rating, const std::string& comment);
    void recalcAvg();
    void setAvgFeedback(double v) { avgFeedback = v; }
    double getAvgFeedback() const { return avgFeedback; }
    int getFeedbackCount() const { return feedbackCount; }
    int getFeedbackRating(int i) const { return feedbackRatings[i]; }
    std::string getFeedbackComment(int i) const { return feedbackComments[i]; }

    void showFeedbacks() const;
    void addCourse(const std::string& cid);
    void displayProfile() const override;

    int getCourseCount() const { return courseCount; }
    std::string getCourseAt(int i) const { return assignedCourseIDs[i]; }

};

#endif