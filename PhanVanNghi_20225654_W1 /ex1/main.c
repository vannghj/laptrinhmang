/*
 * Bai 1 - Ma hoa chat doc hai (HAZCHEM)
 * main.c lo phan doc/xu ly nhap; viec tra bang y nghia ma nam trong hazchem.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hazchem.h"

#define MAX_ATTEMPTS 3
#define LINE_SIZE 128

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
 * @function askReverseColoured: Hoi nguoi dung ky tu co duoc in dao mau khong.
 *
 * @param letterChar: Ky tu thu hai cua ma, da la chu in hoa.
 *
 * @return: 1 neu nguoi dung tra loi yes.
 *          0 neu nguoi dung tra loi no.
 */
static int askReverseColoured(char letterChar)
{
    char answer[LINE_SIZE];

    for (;;) {
        printf("Is the %c reverse coloured? ", letterChar);
        fflush(stdout);
        readLine(answer, sizeof(answer));
        trimWhitespace(answer);

        if (strcasecmp(answer, "yes") == 0) {
            return 1;
        }
        if (strcasecmp(answer, "no") == 0) {
            return 0;
        }
        printf("Please answer yes or no.\n");
    }
}

int main(void)
{
    char buffer[LINE_SIZE];
    int attempt;

    for (attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        printf("Enter HAZCHEM code: ");
        fflush(stdout);
        if (!readLine(buffer, sizeof(buffer))) {
            return 1;
        }
        trimWhitespace(buffer);

        char firstChar, letterChar;
        int hasEvacuation = 0;
        if (!checkCode(buffer, &firstChar, &letterChar, &hasEvacuation)) {
            printf("Invalid HAZCHEM code. Please try again.\n");
            continue;
        }

        int reversed = 0;
        if (needsColourQuestion(letterChar)) {
            reversed = askReverseColoured(letterChar);
        }

        Advice advice;
        buildAdvice(firstChar, letterChar, reversed, hasEvacuation, &advice);
        printAdvice(&advice);
        return 0;
    }

    return 1;
}
