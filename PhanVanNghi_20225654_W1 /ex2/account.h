#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_ACCOUNTS 100
#define NAME_SIZE 64

/* Trang thai tai khoan luu trong account.txt */
#define STATUS_LOCKED 0
#define STATUS_ACTIVE 1

/**
 * @struct Account: Mot dong du lieu doc duoc tu account.txt.
 */
typedef struct {
    char username[NAME_SIZE];
    int status;
} Account;

/**
 * @function loadAccounts: Doc file account.txt vao mang accounts.
 *
 * @param path: Duong dan file du lieu can doc.
 * @param accounts: Mang se duoc dien du lieu tai khoan.
 * @param maxCount: So phan tu toi da cua mang accounts.
 * @param count: Con tro nhan so tai khoan da doc duoc.
 *
 * @return: 1 neu mo va doc file thanh cong.
 *          0 neu khong mo duoc file.
 */
int loadAccounts(const char *path, Account accounts[], int maxCount, int *count);

/**
 * @function findAccount: Tim mot tai khoan theo ten dang nhap.
 *
 * @param accounts: Mang danh sach tai khoan.
 * @param count: So tai khoan trong mang.
 * @param username: Ten dang nhap can tim.
 *
 * @return: Vi tri (index) cua tai khoan trong mang neu tim thay.
 *          -1 neu khong tim thay.
 */
int findAccount(const Account accounts[], int count, const char *username);

#endif
