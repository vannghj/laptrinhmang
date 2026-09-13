#ifndef STUDENT_H
#define STUDENT_H

#define MSSV_SIZE 16
#define LASTNAME_SIZE 64
#define FIRSTNAME_SIZE 32

/**
 * @struct Student: Mot sinh vien trong bang diem, la mot node cua danh sach lien ket.
 */
typedef struct Student {
    char mssv[MSSV_SIZE];
    char lastName[LASTNAME_SIZE];   /* Ho va ten dem */
    char firstName[FIRSTNAME_SIZE]; /* Ten */
    double processScore;            /* Diem qua trinh */
    double finalScore;              /* Diem cuoi ky */
    double finalMark;               /* Diem tong ket, da lam tron 2 chu so */
    char letter;                    /* Diem chu: A, B, C, D hoac F */
    struct Student *next;
} Student;

/**
 * @function createStudent: Cap phat va khoi tao mot node sinh vien moi.
 *
 * @param mssv: Ma so sinh vien.
 * @param lastName: Ho va ten dem.
 * @param firstName: Ten.
 * @param processScore: Diem qua trinh.
 * @param finalScore: Diem cuoi ky.
 * @param finalMark: Diem tong ket da tinh san.
 * @param letter: Diem chu da tinh san.
 *
 * @return: Con tro toi node moi, hoac NULL neu cap phat that bai.
 */
Student *createStudent(const char *mssv, const char *lastName,
                       const char *firstName, double processScore,
                       double finalScore, double finalMark, char letter);

/**
 * @function insertStudentTail: Them mot node vao cuoi danh sach lien ket.
 *
 * @param head: Con tro toi con tro dau danh sach (co the bi thay doi neu
 *              danh sach dang rong).
 * @param newStudent: Node can them vao.
 *
 * @return: nothing.
 */
void insertStudentTail(Student **head, Student *newStudent);

/**
 * @function findStudent: Tim mot sinh vien theo MSSV.
 *
 * @param head: Con tro dau danh sach.
 * @param mssv: MSSV can tim.
 *
 * @return: Con tro toi node tim thay, hoac NULL neu khong co.
 */
Student *findStudent(Student *head, const char *mssv);

/**
 * @function removeStudent: Xoa mot sinh vien khoi danh sach theo MSSV.
 *
 * @param head: Con tro toi con tro dau danh sach (co the bi thay doi).
 * @param mssv: MSSV can xoa.
 *
 * @return: 1 neu xoa thanh cong.
 *          0 neu khong tim thay sinh vien co MSSV do.
 */
int removeStudent(Student **head, const char *mssv);

/**
 * @function countStudents: Dem so sinh vien hien co trong danh sach.
 *
 * @param head: Con tro dau danh sach.
 *
 * @return: So luong node trong danh sach.
 */
int countStudents(Student *head);

/**
 * @function freeStudentList: Giai phong toan bo bo nho cua danh sach.
 *
 * @param head: Con tro dau danh sach.
 *
 * @return: nothing.
 */
void freeStudentList(Student *head);

#endif
