#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "student.h"

#define SUBJECT_ID_SIZE 32
#define SUBJECT_NAME_SIZE 128
#define SEMESTER_SIZE 16
#define FILENAME_SIZE 128

/* Ma loi tra ve tu loadScoreBoard */
#define LOAD_OK 0
#define LOAD_FILE_NOT_FOUND 1
#define LOAD_BAD_FORMAT 2

/**
 * @struct ScoreBoard: Thong tin bang diem cua mot mon hoc trong mot hoc ky.
 */
typedef struct {
    char subjectId[SUBJECT_ID_SIZE];
    char subjectName[SUBJECT_NAME_SIZE];
    double processWeight;  /* He so diem qua trinh (%) */
    double finalWeight;    /* He so diem cuoi ky (%) */
    char semester[SEMESTER_SIZE];
    Student *head;         /* Danh sach lien ket cac sinh vien */
} ScoreBoard;

/**
 * @function buildDataFileName: Tao ten file du lieu tu ma mon hoc va hoc ky.
 *
 * @param subjectId: Ma mon hoc.
 * @param semester: Ma hoc ky.
 * @param outName: Vung nho nhan ten file ket qua.
 * @param size: Kich thuoc vung nho outName.
 *
 * @return: nothing.
 */
void buildDataFileName(const char *subjectId, const char *semester,
                       char *outName, int size);

/**
 * @function buildReportFileName: Tao ten file bao cao tu ma mon hoc va hoc ky.
 *
 * @param subjectId: Ma mon hoc.
 * @param semester: Ma hoc ky.
 * @param outName: Vung nho nhan ten file ket qua.
 * @param size: Kich thuoc vung nho outName.
 *
 * @return: nothing.
 */
void buildReportFileName(const char *subjectId, const char *semester,
                         char *outName, int size);

/**
 * @function calcFinalMark: Tinh diem tong ket theo he so va lam tron 2 chu so.
 *
 * @param processScore: Diem qua trinh.
 * @param finalScore: Diem cuoi ky.
 * @param processWeight: He so diem qua trinh (%).
 * @param finalWeight: He so diem cuoi ky (%).
 *
 * @return: Diem tong ket da duoc lam tron den 2 chu so thap phan.
 */
double calcFinalMark(double processScore, double finalScore,
                     double processWeight, double finalWeight);

/**
 * @function calcLetter: Quy doi diem tong ket sang diem chu.
 *
 * @param finalMark: Diem tong ket da lam tron.
 *
 * @return: 'A', 'B', 'C', 'D' hoac 'F' tuong ung.
 */
char calcLetter(double finalMark);

/**
 * @function loadScoreBoard: Doc file du lieu vao mot ScoreBoard.
 *
 * Danh sach sinh vien duoc doc vao danh sach lien ket (board->head).
 *
 * @param fileName: Duong dan file du lieu can doc.
 * @param board: Con tro toi cau truc se duoc dien thong tin.
 *
 * @return: LOAD_OK neu thanh cong.
 *          LOAD_FILE_NOT_FOUND neu khong mo duoc file.
 *          LOAD_BAD_FORMAT neu noi dung file sai dinh dang.
 */
int loadScoreBoard(const char *fileName, ScoreBoard *board);

/**
 * @function saveScoreBoard: Ghi mot ScoreBoard ra file, ghi de toan bo noi dung cu.
 *
 * StudentCount duoc tinh lai tu do dai thuc te cua danh sach lien ket.
 *
 * @param fileName: Duong dan file can ghi.
 * @param board: Con tro toi thong tin can ghi.
 *
 * @return: 1 neu ghi thanh cong.
 *          0 neu khong mo duoc file de ghi.
 */
int saveScoreBoard(const char *fileName, const ScoreBoard *board);

/**
 * @function printScoreBoard: Hien thi toan bo bang diem ra man hinh.
 *
 * @param board: Con tro toi bang diem can hien thi.
 *
 * @return: nothing.
 */
void printScoreBoard(const ScoreBoard *board);

/**
 * @function writeReport: Sinh file bao cao thong ke diem cua mot mon hoc.
 *
 * @param fileName: Duong dan file bao cao can ghi.
 * @param board: Con tro toi bang diem can thong ke.
 *
 * @return: 1 neu ghi thanh cong.
 *          0 neu khong mo duoc file de ghi.
 */
int writeReport(const char *fileName, const ScoreBoard *board);

#endif
