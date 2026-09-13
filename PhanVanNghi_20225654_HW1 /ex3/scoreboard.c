#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "scoreboard.h"

#define LINE_SIZE 512
#define MAX_FIELDS 8

/**
 * @function trim: Bo khoang trang thua o dau va cuoi chuoi, sua truc tiep.
 *
 * @param text: Con tro toi chuoi can xu ly.
 *
 * @return: nothing.
 */
static void trim(char *text)
{
    int start = 0;
    int end = (int)strlen(text) - 1;

    while (text[start] != '\0' && isspace((unsigned char)text[start])) {
        start++;
    }
    while (end >= start && isspace((unsigned char)text[end])) {
        text[end] = '\0';
        end--;
    }
    if (start > 0) {
        memmove(text, text + start, strlen(text + start) + 1);
    }
}

/**
 * @function splitFields: Tach mot dong thanh cac truong theo ky tu phan cach '|'.
 *
 * Chuoi line bi thay doi truc tiep (strtok ghi de cac ky tu '|' bang '\0').
 * Moi truong sau khi tach deu duoc cat khoang trang thua.
 *
 * @param line: Chuoi can tach, se bi thay doi noi dung.
 * @param fields: Mang con tro se nhan cac truong da tach.
 * @param maxFields: So phan tu toi da cua mang fields.
 *
 * @return: So truong thuc su tach duoc.
 */
static int splitFields(char *line, char *fields[], int maxFields)
{
    int count = 0;
    char *token = strtok(line, "|");

    while (token != NULL && count < maxFields) {
        trim(token);
        fields[count++] = token;
        token = strtok(NULL, "|");
    }

    return count;
}

/**
 * @function readLine: Doc mot dong tu file, bo ky tu xuong dong.
 *
 * @param file: File dang mo de doc.
 * @param buffer: Vung nho luu du lieu doc duoc.
 * @param size: Kich thuoc vung nho buffer.
 *
 * @return: 1 neu doc duoc mot dong.
 *          0 neu da het file.
 */
static int readLine(FILE *file, char *buffer, int size)
{
    if (fgets(buffer, size, file) == NULL) {
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

void buildDataFileName(const char *subjectId, const char *semester,
                       char *outName, int size)
{
    snprintf(outName, size, "%s_%s.txt", subjectId, semester);
}

void buildReportFileName(const char *subjectId, const char *semester,
                         char *outName, int size)
{
    snprintf(outName, size, "%s_%s_rp.txt", subjectId, semester);
}

double calcFinalMark(double processScore, double finalScore,
                     double processWeight, double finalWeight)
{
    double mark = (processWeight * processScore + finalWeight * finalScore) / 100.0;
    return round(mark * 100.0) / 100.0;
}

char calcLetter(double finalMark)
{
    if (finalMark >= 8.5) {
        return 'A';
    }
    if (finalMark >= 7.0) {
        return 'B';
    }
    if (finalMark >= 5.5) {
        return 'C';
    }
    if (finalMark >= 4.0) {
        return 'D';
    }
    return 'F';
}

int loadScoreBoard(const char *fileName, ScoreBoard *board)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return LOAD_FILE_NOT_FOUND;
    }

    board->head = NULL;
    char line[LINE_SIZE];
    char *fields[MAX_FIELDS];

    /* Dong 1: SubjectID */
    if (!readLine(file, line, sizeof(line))) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    if (splitFields(line, fields, MAX_FIELDS) < 2 || strcmp(fields[0], "SubjectID") != 0) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    strncpy(board->subjectId, fields[1], SUBJECT_ID_SIZE - 1);
    board->subjectId[SUBJECT_ID_SIZE - 1] = '\0';

    /* Dong 2: Subject */
    if (!readLine(file, line, sizeof(line))) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    if (splitFields(line, fields, MAX_FIELDS) < 2 || strcmp(fields[0], "Subject") != 0) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    strncpy(board->subjectName, fields[1], SUBJECT_NAME_SIZE - 1);
    board->subjectName[SUBJECT_NAME_SIZE - 1] = '\0';

    /* Dong 3: F|heSoQuaTrinh|heSoCuoiKy */
    if (!readLine(file, line, sizeof(line))) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    if (splitFields(line, fields, MAX_FIELDS) < 3 || strcmp(fields[0], "F") != 0) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    board->processWeight = atof(fields[1]);
    board->finalWeight = atof(fields[2]);

    /* Dong 4: Semester */
    if (!readLine(file, line, sizeof(line))) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    if (splitFields(line, fields, MAX_FIELDS) < 2 || strcmp(fields[0], "Semester") != 0) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    strncpy(board->semester, fields[1], SEMESTER_SIZE - 1);
    board->semester[SEMESTER_SIZE - 1] = '\0';

    /* Dong 5: StudentCount (chi de kiem tra dinh dang, so luong thuc te se
     * duoc dem lai tu danh sach lien ket sau khi doc xong) */
    if (!readLine(file, line, sizeof(line))) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }
    if (splitFields(line, fields, MAX_FIELDS) < 2 || strcmp(fields[0], "StudentCount") != 0) {
        fclose(file);
        return LOAD_BAD_FORMAT;
    }

    /* Tu dong 6: cac dong sinh vien, bat dau bang S */
    while (readLine(file, line, sizeof(line))) {
        if (line[0] == '\0') {
            continue;
        }

        int fieldCount = splitFields(line, fields, MAX_FIELDS);
        if (fieldCount < 6 || strcmp(fields[0], "S") != 0) {
            freeStudentList(board->head);
            fclose(file);
            return LOAD_BAD_FORMAT;
        }

        double processScore = atof(fields[4]);
        double finalScore = atof(fields[5]);
        double finalMark = calcFinalMark(processScore, finalScore,
                                         board->processWeight, board->finalWeight);
        char letter = calcLetter(finalMark);

        Student *node = createStudent(fields[1], fields[2], fields[3],
                                      processScore, finalScore, finalMark, letter);
        if (node == NULL) {
            freeStudentList(board->head);
            fclose(file);
            return LOAD_BAD_FORMAT;
        }
        insertStudentTail(&board->head, node);
    }

    fclose(file);
    return LOAD_OK;
}

int saveScoreBoard(const char *fileName, const ScoreBoard *board)
{
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "SubjectID|%s\n", board->subjectId);
    fprintf(file, "Subject|%s\n", board->subjectName);
    fprintf(file, "F|%g|%g\n", board->processWeight, board->finalWeight);
    fprintf(file, "Semester|%s\n", board->semester);
    fprintf(file, "StudentCount|%d\n", countStudents(board->head));

    Student *current;
    for (current = board->head; current != NULL; current = current->next) {
        fprintf(file, "S|%s|%s|%s|%.2f|%.2f|%c|\n",
                current->mssv, current->lastName, current->firstName,
                current->processScore, current->finalScore, current->letter);
    }

    fclose(file);
    return 1;
}

void printScoreBoard(const ScoreBoard *board)
{
    printf("Subject: %s (%s)\n", board->subjectName, board->subjectId);
    printf("Semester: %s\n", board->semester);
    printf("%-10s %-20s %-10s %8s %8s %8s %6s\n",
           "MSSV", "Ho va ten dem", "Ten", "QuaTrinh", "CuoiKy", "TongKet", "Chu");

    Student *current;
    for (current = board->head; current != NULL; current = current->next) {
        printf("%-10s %-20s %-10s %8.2f %8.2f %8.2f %6c\n",
               current->mssv, current->lastName, current->firstName,
               current->processScore, current->finalScore,
               current->finalMark, current->letter);
    }
}

int writeReport(const char *fileName, const ScoreBoard *board)
{
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return 0;
    }

    int total = countStudents(board->head);

    if (total == 0) {
        fprintf(file, "\n");
        fprintf(file, "\n");
        fprintf(file, "The average mark is: 0.00\n");
        fprintf(file, "A histogram of the subject %s is:\n", board->subjectId);
        fprintf(file, "A:\n");
        fprintf(file, "B:\n");
        fprintf(file, "C:\n");
        fprintf(file, "D:\n");
        fprintf(file, "F:\n");
        fclose(file);
        return 1;
    }

    Student *highest = board->head;
    Student *lowest = board->head;
    double sum = 0.0;
    int countA = 0, countB = 0, countC = 0, countD = 0, countF = 0;

    Student *current;
    for (current = board->head; current != NULL; current = current->next) {
        sum += current->finalMark;

        if (current->finalMark > highest->finalMark) {
            highest = current;
        }
        if (current->finalMark < lowest->finalMark) {
            lowest = current;
        }

        switch (current->letter) {
            case 'A': countA++; break;
            case 'B': countB++; break;
            case 'C': countC++; break;
            case 'D': countD++; break;
            default:  countF++; break;
        }
    }

    double average = round((sum / total) * 100.0) / 100.0;

    fprintf(file, "The student with the highest mark is: %s %s\n",
            highest->lastName, highest->firstName);
    fprintf(file, "The student with the lowest mark is: %s %s\n",
            lowest->lastName, lowest->firstName);
    fprintf(file, "The average mark is: %.2f\n", average);
    fprintf(file, "A histogram of the subject %s is:\n", board->subjectId);

    fprintf(file, "A:");
    for (int i = 0; i < countA; i++) fprintf(file, "*");
    fprintf(file, "\n");

    fprintf(file, "B:");
    for (int i = 0; i < countB; i++) fprintf(file, "*");
    fprintf(file, "\n");

    fprintf(file, "C:");
    for (int i = 0; i < countC; i++) fprintf(file, "*");
    fprintf(file, "\n");

    fprintf(file, "D:");
    for (int i = 0; i < countD; i++) fprintf(file, "*");
    fprintf(file, "\n");

    fprintf(file, "F:");
    for (int i = 0; i < countF; i++) fprintf(file, "*");
    fprintf(file, "\n");

    fclose(file);
    return 1;
}
