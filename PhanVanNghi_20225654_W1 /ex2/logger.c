#include <stdio.h>
#include <time.h>

#include "logger.h"

#define ESCAPED_SIZE 512

/**
 * @function escapeDollar: Thay moi ky tu '$' trong chuoi nguon bang "\$".
 *
 * @param source: Chuoi nguon can xu ly.
 * @param dest: Vung nho luu ket qua sau khi thay the.
 * @param destSize: Kich thuoc vung nho dest.
 *
 * @return: nothing. Ket qua bi cat bot neu vuot qua destSize.
 */
static void escapeDollar(const char *source, char *dest, int destSize)
{
    int j = 0;
    int i;

    for (i = 0; source[i] != '\0' && j < destSize - 2; i++) {
        if (source[i] == '$') {
            dest[j++] = '\\';
            dest[j++] = '$';
        } else {
            dest[j++] = source[i];
        }
    }
    dest[j] = '\0';
}

void writeLog(const char *path, const char *function, const char *value, int ok)
{
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        printf("Loi: khong ghi duoc file log %s\n", path);
        return;
    }

    time_t now = time(NULL);
    struct tm *localNow = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", localNow);

    char escapedValue[ESCAPED_SIZE];
    escapeDollar(value, escapedValue, sizeof(escapedValue));

    fprintf(file, "[%s] $ %s $ %s $ %s\n", timestamp, function, escapedValue,
            ok ? "+OK" : "-ERR");

    fclose(file);
}
