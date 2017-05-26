/*
 * Declare the Student type and operations on it.
 *
 * Author: Richard Lobb
 * Date: August 2014
 */

#ifndef STUDENT_H
#define STUDENT_H
#define MAX_NAME_SIZE 50

typedef struct student_s Student;

struct student_s {
    char name[MAX_NAME_SIZE];
    int age;
    Student* next;              // Pointer to next student in a list
};

/* Return a pointer to a student record, initialised with the given
 * name and age. This is achieved by allocating records from an internally
 * managed pool of Student records. Returns NULL if no more records available.
 */
Student* newStudent(const char* name, int age);

/* Read a single student from a csv input file with student name in first column,
 * and student age in second.
 * Returns: A pointer to a Student record, or NULL if EOF or an invalid
 * student record is read. Blank lines, or lines in which the name is
 * longer than the provided name buffer, or there is no comma in the line
 * are considered invalid.
 */
Student* readOneStudent(FILE* file);

#endif /* end of ifndef STUDENT_H */
