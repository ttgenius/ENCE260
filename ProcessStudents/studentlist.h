/* studentlist.h
 * Defines the StudentList type, which is a linked list of students,
 * with a function to add students to the list and another to read
 * a list of students from an input file.
 *
 * Author: Richard Lobb
 * Date: August 2014.
 */
#ifndef STUDENT_LIST_H

#define STUDENT_LIST_H
#include <stdio.h>
#include "student.h"
/*
 * Declare the StudentList type and operations on it.
 * The list structure simply contains pointers to the first and
 * last elements, permitting constant-time addition and removal
 * at either end.
 */

typedef struct studentList_s StudentList;

struct studentList_s {
    Student* first;
    Student* last;
};

/* Read a list of students from file fp into the given StudentList */
StudentList readStudents(FILE* file);

/* Add a student to the end of a list */
void addStudent(StudentList* list, Student* student);

#endif /* end ifndef STUDENT_LIST_H */
