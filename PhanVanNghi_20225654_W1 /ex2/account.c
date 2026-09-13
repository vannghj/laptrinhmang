#include <stdio.h>
#include <string.h>

#include "account.h"

int loadAccounts(const char *path, Account accounts[], int maxCount, int *count)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Loi: khong mo duoc file %s\n", path);
        return 0;
    }

    char name[NAME_SIZE];
    int status;
    *count = 0;

    while (*count < maxCount && fscanf(file, "%63s %d", name, &status) == 2) {
        strcpy(accounts[*count].username, name);
        accounts[*count].status = status;
        (*count)++;
    }

    fclose(file);
    return 1;
}

int findAccount(const Account accounts[], int count, const char *username)
{
    int i;

    for (i = 0; i < count; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            return i;
        }
    }

    return -1;
}
