# Academic Office Replacement System

A terminal-based academic management system developed in C++ that automates university office operations including student enrollment, teacher management, course administration, grading, and exam scheduling.

## Features

### Student Management
- Three student types: Regular, Scholarship (with probation), Exchange (Pass/Fail)
- Complete CRUD operations
- Transcript generation with GPA calculation

### Teacher Management
- CRUD operations with feedback system (1-5 star ratings)
- Automatic average rating calculation

### Course Management
- Three course types: Core (3hr exam), Elective (2hr exam), Lab (no exam)
- Configurable weightages via external file
- Support for Exam, Quiz, Assignment assessments

### Registration System
- Smart enrollment with capacity validation
- Time-slot conflict detection
- Duplicate enrollment prevention

### Exam Scheduler
- Automatic venue assignment based on capacity and requirements
- Conflict detection and resolution

### File Persistence
All data saved to text files: students, teachers, courses, venues, sections, assessments, exam schedule, weightages

## Grading Scale

| Percentage | GPA | Letter |
|------------|-----|--------|
| 90-100% | 4.0 | A+/A |
| 85-89% | 3.7 | A |
| 80-84% | 3.3 | A- |
| 75-79% | 3.0 | B+ |
| 70-74% | 2.7 | B |
| 65-69% | 2.3 | B- |
| 60-64% | 2.0 | C+ |
| 55-59% | 1.7 | C |
| 50-54% | 1.3 | D |
| 0-49% | 0.0 | F |
