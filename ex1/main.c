#include <stdio.h>
#include <stdlib.h>

#include "hazchem.h"

/**
 * @function main: Read a HAZCHEM code and display the emergency action advice.
 *
 * The user may retype the code at most MAX_ATTEMPTS times; the program stops
 * after a valid code has been processed or after the last failed attempt.
 *
 * @return: EXIT_SUCCESS if a valid code has been processed.
 *          EXIT_FAILURE if no valid code has been given.
 */
int main(void)
{
    char buffer[INPUT_SIZE];
    int attempt;

    for (attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        char *code;
        char materialChar;
        char letterChar;
        int hasEvacuation = 0;
        int reversed = 0;
        HazchemAdvice advice;

        printf("Enter HAZCHEM code: ");
        fflush(stdout);

        if (!readLine(buffer, sizeof(buffer))) {
            return EXIT_FAILURE;
        }

        code = trimWhitespace(buffer);
        if (!parseHazchemCode(code, &materialChar, &letterChar, &hasEvacuation)) {
            printf("Invalid HAZCHEM code. Please try again.\n");
            continue;
        }

        if (needsColourQuestion(letterChar) &&
            !askReverseColoured(letterChar, &reversed)) {
            return EXIT_FAILURE;
        }

        if (buildAdvice(materialChar, letterChar, reversed, hasEvacuation,
                        &advice) != 0) {
            printf("Invalid HAZCHEM code. Please try again.\n");
            continue;
        }

        printAdvice(&advice);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
