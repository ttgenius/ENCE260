/* studentlist.c: implements the StudentList type defined in
 * studentlist.h
 *
 * Author: Richard Lobb
 * Date: August 2014.
 */
#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "studentlist.h"

// Adds a student to the list. This version adds to the end of the list,
// by using the list tail pointer stored within the StudentList struct
void addStudent(StudentList* list, Student* student)
{
    if (list->first == NULL) {
        list->first = list->last = student;
    }
    else {
        list->last->next = student;
        list->last = student;
    }
    student->next = NULL;
}

/* Reads a list of students from a given file into a given StudentList.
 * Input stops when a blank line is read, or an EOF occurs, or an illegal
 * input line is encountered.
 */
StudentList readStudents(FILE* file)
{
    StudentList list = {NULL, NULL};
    Student* student = readOneStudent(file);
    while (student != NULL) {
        addStudent(&list, student);
        student = readOneStudent(file);
    }
    return list;
}




