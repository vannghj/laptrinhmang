/*
 * Bai 3 - Xu ly diem sinh vien
 * main.c dieu phoi menu; doc/ghi file va tinh diem nam trong scoreboard.c;
 * thao tac danh sach lien ket sinh vien nam trong student.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "student.h"
#include "scoreboard.h"

#define LINE_SIZE 256

/**
 * @function trimWhitespace: Bo khoang trang thua o dau va cuoi chuoi.
 *
 * @param text: Con tro toi chuoi can xu ly (chuoi bi thay doi truc tiep).
 *
 * @return: nothing.
 */
static void trimWhitespace(char *text)
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
 * @function readLine: Doc mot dong tu ban phim, bo ky tu xuong dong.
 *
 * @param buffer: Con tro toi vung nho luu du lieu doc duoc.
 * @param size: Kich thuoc vung nho buffer.
 *
 * @return: 1 neu doc duoc mot dong.
 *          0 neu da het du lieu nhap vao (EOF).
 */
static int readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

/**
 * @function promptLine: In thong bao roi doc mot dong da cat khoang trang.
 *
 * @param prompt: Thong bao hien thi truoc khi doc.
 * @param buffer: Vung nho nhan du lieu doc duoc.
 * @param size: Kich thuoc vung nho buffer.
 *
 * @return: nothing.
 */
static void promptLine(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    fflush(stdout);
    readLine(buffer, size);
    trimWhitespace(buffer);
}

/**
 * @function askContinue: Hoi nguoi dung co muon lap lai chuc nang vua chon khong.
 *
 * @return: 1 neu nguoi dung tra loi y hoac Y.
 *          0 neu tra loi khac.
 */
static int askContinue(void)
{
    char answer[LINE_SIZE];
    promptLine("Continue with this function? (y/n): ", answer, sizeof(answer));
    return (answer[0] == 'y' || answer[0] == 'Y');
}

/**
 * @function inputScoreBoardId: Doc ma mon hoc va ma hoc ky de xac dinh file.
 *
 * @param subjectId: Vung nho nhan ma mon hoc.
 * @param subjectSize: Kich thuoc vung nho subjectId.
 * @param semester: Vung nho nhan ma hoc ky.
 * @param semesterSize: Kich thuoc vung nho semester.
 *
 * @return: nothing.
 */
static void inputScoreBoardId(char *subjectId, int subjectSize,
                              char *semester, int semesterSize)
{
    promptLine("Subject code: ", subjectId, subjectSize);
    promptLine("Semester code: ", semester, semesterSize);
}

/**
 * @function reportLoadError: In thong bao loi tuong ung voi ma loi cua loadScoreBoard.
 *
 * @param code: Ma loi tra ve tu loadScoreBoard.
 *
 * @return: nothing.
 */
static void reportLoadError(int code)
{
    if (code == LOAD_FILE_NOT_FOUND) {
        printf("Loi: khong tim thay bang diem cho mon hoc/hoc ky nay.\n");
    } else if (code == LOAD_BAD_FORMAT) {
        printf("Loi: noi dung file bang diem sai dinh dang.\n");
    }
}

/**
 * @function function1_AddScoreBoard: Chuc nang 1 - Tao bang diem moi cho mot mon hoc.
 *
 * @return: nothing.
 */
static void function1_AddScoreBoard(void)
{
    do {
        ScoreBoard board;
        board.head = NULL;

        promptLine("Subject code: ", board.subjectId, sizeof(board.subjectId));
        promptLine("Subject name: ", board.subjectName, sizeof(board.subjectName));

        char buffer[LINE_SIZE];
        promptLine("Process score weight (%): ", buffer, sizeof(buffer));
        board.processWeight = atof(buffer);
        promptLine("Final score weight (%): ", buffer, sizeof(buffer));
        board.finalWeight = atof(buffer);

        promptLine("Semester code: ", board.semester, sizeof(board.semester));

        char fileName[FILENAME_SIZE];
        buildDataFileName(board.subjectId, board.semester, fileName, sizeof(fileName));

        FILE *existing = fopen(fileName, "r");
        if (existing != NULL) {
            fclose(existing);
            printf("Loi: bang diem cho mon hoc/hoc ky nay da ton tai.\n");
        } else if (saveScoreBoard(fileName, &board)) {
            printf("Da tao bang diem moi: %s\n", fileName);
        } else {
            printf("Loi: khong tao duoc file %s\n", fileName);
        }
    } while (askContinue());
}

/**
 * @function function2_AddScore: Chuc nang 2 - Them diem cho mot sinh vien.
 *
 * @return: nothing.
 */
static void function2_AddScore(void)
{
    do {
        char subjectId[SUBJECT_ID_SIZE];
        char semester[SEMESTER_SIZE];
        inputScoreBoardId(subjectId, sizeof(subjectId), semester, sizeof(semester));

        char fileName[FILENAME_SIZE];
        buildDataFileName(subjectId, semester, fileName, sizeof(fileName));

        ScoreBoard board;
        int rc = loadScoreBoard(fileName, &board);
        if (rc != LOAD_OK) {
            reportLoadError(rc);
        } else {
            char mssv[MSSV_SIZE];
            char lastName[LASTNAME_SIZE];
            char firstName[FIRSTNAME_SIZE];
            char buffer[LINE_SIZE];

            promptLine("MSSV: ", mssv, sizeof(mssv));

            if (findStudent(board.head, mssv) != NULL) {
                printf("Loi: MSSV da ton tai trong bang diem.\n");
            } else {
                promptLine("Ho va ten dem: ", lastName, sizeof(lastName));
                promptLine("Ten: ", firstName, sizeof(firstName));
                promptLine("Diem qua trinh: ", buffer, sizeof(buffer));
                double processScore = atof(buffer);
                promptLine("Diem cuoi ky: ", buffer, sizeof(buffer));
                double finalScore = atof(buffer);

                double finalMark = calcFinalMark(processScore, finalScore,
                                                 board.processWeight, board.finalWeight);
                char letter = calcLetter(finalMark);

                Student *node = createStudent(mssv, lastName, firstName,
                                              processScore, finalScore,
                                              finalMark, letter);
                if (node == NULL) {
                    printf("Loi: khong cap phat duoc bo nho.\n");
                } else {
                    insertStudentTail(&board.head, node);
                    if (saveScoreBoard(fileName, &board)) {
                        printf("Da them sinh vien %s (diem tong ket %.2f, diem chu %c)\n",
                               mssv, finalMark, letter);
                    } else {
                        printf("Loi: khong ghi duoc file %s\n", fileName);
                    }
                }
            }

            freeStudentList(board.head);
        }
    } while (askContinue());
}

/**
 * @function function3_RemoveScore: Chuc nang 3 - Xoa diem cua mot sinh vien.
 *
 * @return: nothing.
 */
static void function3_RemoveScore(void)
{
    do {
        char subjectId[SUBJECT_ID_SIZE];
        char semester[SEMESTER_SIZE];
        inputScoreBoardId(subjectId, sizeof(subjectId), semester, sizeof(semester));

        char fileName[FILENAME_SIZE];
        buildDataFileName(subjectId, semester, fileName, sizeof(fileName));

        ScoreBoard board;
        int rc = loadScoreBoard(fileName, &board);
        if (rc != LOAD_OK) {
            reportLoadError(rc);
        } else {
            char mssv[MSSV_SIZE];
            promptLine("MSSV: ", mssv, sizeof(mssv));

            if (removeStudent(&board.head, mssv)) {
                if (saveScoreBoard(fileName, &board)) {
                    printf("Da xoa sinh vien %s\n", mssv);
                } else {
                    printf("Loi: khong ghi duoc file %s\n", fileName);
                }
            } else {
                printf("Loi: khong tim thay sinh vien co MSSV nay.\n");
            }

            freeStudentList(board.head);
        }
    } while (askContinue());
}

/**
 * @function function4_SearchScore: Chuc nang 4 - Tim va hien thi diem cua mot sinh vien.
 *
 * @return: nothing.
 */
static void function4_SearchScore(void)
{
    do {
        char subjectId[SUBJECT_ID_SIZE];
        char semester[SEMESTER_SIZE];
        inputScoreBoardId(subjectId, sizeof(subjectId), semester, sizeof(semester));

        char fileName[FILENAME_SIZE];
        buildDataFileName(subjectId, semester, fileName, sizeof(fileName));

        ScoreBoard board;
        int rc = loadScoreBoard(fileName, &board);
        if (rc != LOAD_OK) {
            reportLoadError(rc);
        } else {
            char mssv[MSSV_SIZE];
            promptLine("MSSV: ", mssv, sizeof(mssv));

            Student *found = findStudent(board.head, mssv);
            if (found == NULL) {
                printf("Loi: khong tim thay sinh vien co MSSV nay.\n");
            } else {
                printf("MSSV: %s\n", found->mssv);
                printf("Ho va ten dem: %s\n", found->lastName);
                printf("Ten: %s\n", found->firstName);
                printf("Diem qua trinh: %.2f\n", found->processScore);
                printf("Diem cuoi ky: %.2f\n", found->finalScore);
                printf("Diem tong ket: %.2f\n", found->finalMark);
                printf("Diem chu: %c\n", found->letter);
            }

            freeStudentList(board.head);
        }
    } while (askContinue());
}

/**
 * @function function5_DisplayAndReport: Chuc nang 5 - Hien thi bang diem va sinh bao cao.
 *
 * @return: nothing.
 */
static void function5_DisplayAndReport(void)
{
    do {
        char subjectId[SUBJECT_ID_SIZE];
        char semester[SEMESTER_SIZE];
        inputScoreBoardId(subjectId, sizeof(subjectId), semester, sizeof(semester));

        char fileName[FILENAME_SIZE];
        buildDataFileName(subjectId, semester, fileName, sizeof(fileName));

        ScoreBoard board;
        int rc = loadScoreBoard(fileName, &board);
        if (rc != LOAD_OK) {
            reportLoadError(rc);
        } else {
            printScoreBoard(&board);

            char reportFileName[FILENAME_SIZE];
            buildReportFileName(subjectId, semester, reportFileName, sizeof(reportFileName));
            if (writeReport(reportFileName, &board)) {
                printf("Da sinh file bao cao: %s\n", reportFileName);
            } else {
                printf("Loi: khong ghi duoc file bao cao %s\n", reportFileName);
            }

            freeStudentList(board.head);
        }
    } while (askContinue());
}

/**
 * @function printMenu: Hien thi menu chinh ra man hinh.
 *
 * @return: nothing.
 */
static void printMenu(void)
{
    printf("Learning Management System\n");
    printf("-------------------------------------\n");
    printf("1. Add a new score board\n");
    printf("2. Add score\n");
    printf("3. Remove score\n");
    printf("4. Search score\n");
    printf("5. Display score board and score report\n");
    printf("Your choice (1-5, other to quit): ");
    fflush(stdout);
}

int main(void)
{
    for (;;) {
        printMenu();

        char line[LINE_SIZE];
        if (!readLine(line, sizeof(line))) {
            break;
        }
        trimWhitespace(line);
        int choice = atoi(line);

        if (choice == 1) {
            function1_AddScoreBoard();
        } else if (choice == 2) {
            function2_AddScore();
        } else if (choice == 3) {
            function3_RemoveScore();
        } else if (choice == 4) {
            function4_SearchScore();
        } else if (choice == 5) {
            function5_DisplayAndReport();
        } else {
            break;
        }
    }

    return 0;
}
