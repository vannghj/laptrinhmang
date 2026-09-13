/*
 * Bai 2 - Quan ly tai khoan nguoi dung
 * main.c chi lo phan menu va trang thai dang nhap; doc file tai khoan nam
 * trong account.c, ghi nhat ky nam trong logger.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "account.h"
#include "logger.h"

#define MSSV "20225654"
#define ACCOUNT_FILE "account.txt"
#define LOG_FILE "log_" MSSV ".txt"
#define LINE_SIZE 256

static Account accounts[MAX_ACCOUNTS];
static int accountCount = 0;

static int isLoggedIn = 0;
static char currentUser[NAME_SIZE] = "";

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
 * @function printMenu: Hien thi menu chinh ra man hinh.
 *
 * @return: nothing.
 */
static void printMenu(void)
{
    printf("1. Log in\n");
    printf("2. Post message\n");
    printf("3. Logout\n");
    printf("4. Exit\n");
    printf("Your choice (1-4): ");
    fflush(stdout);
}

/**
 * @function doLogin: Xu ly chuc nang 1 - Log in.
 *
 * @return: nothing. In ket qua ra man hinh va ghi log.
 */
static void doLogin(void)
{
    char name[LINE_SIZE];
    printf("Username: ");
    fflush(stdout);
    readLine(name, sizeof(name));
    trimWhitespace(name);

    int ok = 0;

    if (isLoggedIn) {
        printf("You have already logged in\n");
    } else {
        int idx = findAccount(accounts, accountCount, name);
        if (idx == -1) {
            printf("Account is not exist\n");
        } else if (accounts[idx].status == STATUS_LOCKED) {
            printf("Account is banned\n");
        } else {
            printf("Hello %s\n", name);
            isLoggedIn = 1;
            strcpy(currentUser, name);
            ok = 1;
        }
    }

    writeLog(LOG_FILE, "1", name, ok);
}

/**
 * @function doPostMessage: Xu ly chuc nang 2 - Post message.
 *
 * @return: nothing. In ket qua ra man hinh va ghi log.
 */
static void doPostMessage(void)
{
    char message[LINE_SIZE];
    printf("Post message: ");
    fflush(stdout);
    readLine(message, sizeof(message));
    trimWhitespace(message);

    if (isLoggedIn) {
        printf("Successful post\n");
    } else {
        printf("You have not logged in.\n");
    }

    writeLog(LOG_FILE, "2", message, isLoggedIn);
}

/**
 * @function doLogout: Xu ly chuc nang 3 - Logout.
 *
 * @return: nothing. In ket qua ra man hinh va ghi log.
 */
static void doLogout(void)
{
    int ok = isLoggedIn;

    if (isLoggedIn) {
        printf("Successful log out\n");
        isLoggedIn = 0;
        currentUser[0] = '\0';
    } else {
        printf("You have not logged in.\n");
    }

    writeLog(LOG_FILE, "3", "", ok);
}

int main(void)
{
    if (!loadAccounts(ACCOUNT_FILE, accounts, MAX_ACCOUNTS, &accountCount)) {
        return 1;
    }

    for (;;) {
        printMenu();

        char line[LINE_SIZE];
        if (!readLine(line, sizeof(line))) {
            break;
        }
        trimWhitespace(line);
        int choice = atoi(line);

        if (choice == 1) {
            doLogin();
        } else if (choice == 2) {
            doPostMessage();
        } else if (choice == 3) {
            doLogout();
        } else if (choice == 4) {
            writeLog(LOG_FILE, "4", "", 1);
            break;
        } else {
            printf("Invalid choice. Please enter a number from 1 to 4.\n");
        }
    }

    return 0;
}
