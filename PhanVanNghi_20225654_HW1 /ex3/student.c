#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"

Student *createStudent(const char *mssv, const char *lastName,
                       const char *firstName, double processScore,
                       double finalScore, double finalMark, char letter)
{
    Student *node = (Student *)malloc(sizeof(Student));
    if (node == NULL) {
        return NULL;
    }

    strncpy(node->mssv, mssv, MSSV_SIZE - 1);
    node->mssv[MSSV_SIZE - 1] = '\0';
    strncpy(node->lastName, lastName, LASTNAME_SIZE - 1);
    node->lastName[LASTNAME_SIZE - 1] = '\0';
    strncpy(node->firstName, firstName, FIRSTNAME_SIZE - 1);
    node->firstName[FIRSTNAME_SIZE - 1] = '\0';
    node->processScore = processScore;
    node->finalScore = finalScore;
    node->finalMark = finalMark;
    node->letter = letter;
    node->next = NULL;

    return node;
}

void insertStudentTail(Student **head, Student *newStudent)
{
    if (*head == NULL) {
        *head = newStudent;
        return;
    }

    Student *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newStudent;
}

Student *findStudent(Student *head, const char *mssv)
{
    Student *current;

    for (current = head; current != NULL; current = current->next) {
        if (strcmp(current->mssv, mssv) == 0) {
            return current;
        }
    }

    return NULL;
}

int removeStudent(Student **head, const char *mssv)
{
    Student *current = *head;
    Student *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->mssv, mssv) == 0) {
            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    return 0;
}

int countStudents(Student *head)
{
    int count = 0;
    Student *current;

    for (current = head; current != NULL; current = current->next) {
        count++;
    }

    return count;
}

void freeStudentList(Student *head)
{
    Student *current = head;

    while (current != NULL) {
        Student *next = current->next;
        free(current);
        current = next;
    }
}
