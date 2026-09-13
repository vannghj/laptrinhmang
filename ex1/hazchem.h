#ifndef HAZCHEM_H
#define HAZCHEM_H

#include <stddef.h>

/* Number of times the user may retype an invalid code before the program stops. */
#define MAX_ATTEMPTS 3

/* Size of the buffer used to read one line from the keyboard. */
#define INPUT_SIZE 128

/* Set of letters accepted as the second character of a HAZCHEM code. */
#define LETTER_SET "PRSTWXYZ"

/**
 * @struct HazchemAdvice: Emergency action advice matching one HAZCHEM code.
 *
 * Fields holding NULL mean "do not display this line".
 */
typedef struct {
    const char *material;    /* advice for the first character (1..4)        */
    const char *reactivity;  /* NULL when the letter is not violently reactive */
    const char *protection;  /* protective equipment to be worn              */
    const char *containment; /* how a spillage must be handled               */
    int evacuation;          /* 1 when the code ends with E, 0 otherwise     */
} HazchemAdvice;

/**
 * @function trimWhitespace: Remove leading and trailing blanks of a string in place.
 *
 * @param text: A pointer to the string to be trimmed (modified by the function).
 *
 * @return: A pointer to the first non blank character of the string.
 */
char *trimWhitespace(char *text);

/**
 * @function readLine: Read one line from the standard input without the newline.
 *
 * @param buffer: A pointer to the destination buffer.
 * @param size: Size in bytes of the destination buffer.
 *
 * @return: 1 if a line has been read.
 *          0 if the end of the input has been reached.
 */
int readLine(char *buffer, size_t size);

/**
 * @function parseHazchemCode: Check a HAZCHEM code and split it into its parts.
 *
 * @param code: A pointer to the code typed by the user, already trimmed.
 * @param materialChar: A pointer receiving the first character ('1'..'4').
 * @param letterChar: A pointer receiving the second character, in upper case.
 * @param hasEvacuation: A pointer receiving 1 if the code ends with E, else 0.
 *
 * @return: 1 if the code is valid.
 *          0 if the length, the first, the second or the third character is wrong.
 */
int parseHazchemCode(const char *code, char *materialChar, char *letterChar,
                     int *hasEvacuation);

/**
 * @function needsColourQuestion: Tell whether a letter may be reverse coloured.
 *
 * @param letter: The second character of the code, in upper case.
 *
 * @return: 1 for S, T, Y and Z.
 *          0 for the other letters.
 */
int needsColourQuestion(char letter);

/**
 * @function askReverseColoured: Ask the user whether the letter is reverse coloured.
 *
 * Only the answers yes and no are accepted, upper or lower case; any other
 * answer makes the question be asked again.
 *
 * @param letter: The second character of the code, in upper case.
 * @param reversed: A pointer receiving 1 for yes and 0 for no.
 *
 * @return: 1 if an answer has been obtained.
 *          0 if the end of the input has been reached.
 */
int askReverseColoured(char letter, int *reversed);

/**
 * @function buildAdvice: Fill the advice structure matching one HAZCHEM code.
 *
 * @param materialChar: The first character of the code ('1'..'4').
 * @param letterChar: The second character of the code, in upper case.
 * @param reversed: 1 if the letter is reverse coloured, 0 otherwise.
 * @param hasEvacuation: 1 if the code ends with E, 0 otherwise.
 * @param advice: A pointer to the structure to be filled.
 *
 * @return: 0 if success.
 *          1 if the characters do not appear in the HAZCHEM tables.
 */
int buildAdvice(char materialChar, char letterChar, int reversed,
                int hasEvacuation, HazchemAdvice *advice);

/**
 * @function printAdvice: Display the emergency action advice on the screen.
 *
 * @param advice: A pointer to the advice to be displayed.
 *
 * @return: nothing.
 */
void printAdvice(const HazchemAdvice *advice);

#endif /* HAZCHEM_H */
