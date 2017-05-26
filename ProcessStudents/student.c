/* This file implements the functions defined in student.h
 * Author: Richard Lobb
 * Date: August 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "student.h"

#define MAX_LINE_LENGTH 80      // The longest line this program will accept
#define MAX_NUM_STUDENTS 500    // The maximum number of students

// The pool of student records to be allocated on demand.

Student studentPool[MAX_NUM_STUDENTS];  // The student pool
int firstFree = 0;

/*
 * Return a pointer to a new student record from the pool, after
 * filling in the provided name and age fields. Returns NULL if
 * the student pool is exhausted.
 */
Student* newStudent(const char *name, int age) {
    Student* student = NULL;
    if (firstFree < MAX_NUM_STUDENTS) {
        student = &studentPool[firstFree];
        firstFree += 1;
        strncpy(student->name, name, MAX_NAME_SIZE);
        student->name[MAX_NAME_SIZE - 1] = '\0'; // Make sure it's terminated
        student->age = age;
        student->next = NULL;
    }
    return student;
}

/* Read a single student from a csv input file with student name in first column,
 * and student age in second.
 * Returns: A pointer to a Student record, or NULL if EOF or an invalid
 * student record is read. Blank lines, or lines in which the name is
 * longer than the provided name buffer, or there is no comma in the line
 * are considered invalid.
 */
Student *readOneStudent(FILE *file)
{
    char buffer[MAX_LINE_LENGTH];  // Buffer into which we read a line from stdin
    Student* student = NULL;       // Pointer to a student record from the pool

    // Read a line, extract name and age

    char* ptr = fgets(buffer, MAX_LINE_LENGTH, file);
    if (ptr != NULL) {           // Proceed only if we read something
        char* commaPos = strchr(buffer, ',');
        if (commaPos != NULL && commaPos > buffer) {
            int age = atoi(commaPos + 1);
            *commaPos = '\0';  // null-terminate the name
            student = newStudent(buffer, age);
        }
    }
    return student;
}
