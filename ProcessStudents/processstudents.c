/* processstudents.c: a program to process students read from a file.
 * In fact it does very little other then read the students and link
 * them into a list, which it then prints.
 * Written to demonstrate the use of structs, linked lists and separate
 * compilation for course ENCE260.
 *
 * Author: Richard Lobb
 * Date: August 2014.
 */

#include <stdio.h>
#include <stdlib.h>

#include "student.h"
#include "studentlist.h"

// Print a list of students. The parameter list is (the pointer to) the
// StudentList object that defines the list.
void printStudents(const StudentList* list)
{
    const Student* student = list->first;
    while (student != NULL) {
        printf("%s (%d)\n", student->name, student->age);
        student = student->next;
    }
}

int main(void)
{
    FILE* inputFile = fopen("studlist.txt", "r");
    StudentList studentList = readStudents(inputFile);
    printStudents(&studentList);
    return EXIT_SUCCESS;
}
