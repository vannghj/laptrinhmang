#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "hazchem.h"

/* Wording used in the emergency action advice. */
#define REACTIVITY_V   "can be violently reactive"
#define PROTECTION_FULL "full protective clothing must be worn"
#define PROTECTION_BA   "breathing apparatus"
#define PROTECTION_BAF  "breathing apparatus, protective gloves for fire only"
#define CONTAIN_DILUTE  "may be diluted and washed down the drain"
#define CONTAIN_CONTAIN "must be contained, prevent from entering drains or water courses"
#define EVACUATION_TEXT "consider evacuation"

/* Value used in the letter table when the colour of the letter does not matter. */
#define ANY_COLOUR (-1)

/**
 * @struct MaterialEntry: One row of the table of the first character.
 */
typedef struct {
    char digit;             /* first character of the code */
    const char *material;   /* material used to fight the fire */
} MaterialEntry;

/**
 * @struct LetterEntry: One row of the table of the second character.
 */
typedef struct {
    char letter;             /* second character of the code */
    int reversed;            /* 0 normal, 1 reverse coloured, ANY_COLOUR both */
    const char *reactivity;  /* NULL when the line must be hidden */
    const char *protection;
    const char *containment;
} LetterEntry;

static const MaterialEntry MATERIAL_TABLE[] = {
    { '1', "jets"      },
    { '2', "fog"       },
    { '3', "foam"      },
    { '4', "dry agent" }
};

static const LetterEntry LETTER_TABLE[] = {
    { 'P', ANY_COLOUR, REACTIVITY_V, PROTECTION_FULL, CONTAIN_DILUTE  },
    { 'R', ANY_COLOUR, NULL,         PROTECTION_FULL, CONTAIN_DILUTE  },
    { 'S', 0,          REACTIVITY_V, PROTECTION_BA,   CONTAIN_DILUTE  },
    { 'S', 1,          REACTIVITY_V, PROTECTION_BAF,  CONTAIN_DILUTE  },
    { 'T', 0,          NULL,         PROTECTION_BA,   CONTAIN_DILUTE  },
    { 'T', 1,          NULL,         PROTECTION_BAF,  CONTAIN_DILUTE  },
    { 'W', ANY_COLOUR, REACTIVITY_V, PROTECTION_FULL, CONTAIN_CONTAIN },
    { 'X', ANY_COLOUR, NULL,         PROTECTION_FULL, CONTAIN_CONTAIN },
    { 'Y', 0,          REACTIVITY_V, PROTECTION_BA,   CONTAIN_CONTAIN },
    { 'Y', 1,          REACTIVITY_V, PROTECTION_BAF,  CONTAIN_CONTAIN },
    { 'Z', 0,          NULL,         PROTECTION_BA,   CONTAIN_CONTAIN },
    { 'Z', 1,          NULL,         PROTECTION_BAF,  CONTAIN_CONTAIN }
};

static const size_t MATERIAL_COUNT = sizeof(MATERIAL_TABLE) / sizeof(MATERIAL_TABLE[0]);
static const size_t LETTER_COUNT = sizeof(LETTER_TABLE) / sizeof(LETTER_TABLE[0]);

char *trimWhitespace(char *text)
{
    char *start = text;
    size_t length;

    while (*start != '\0' && isspace((unsigned char)*start)) {
        start++;
    }

    length = strlen(start);
    while (length > 0 && isspace((unsigned char)start[length - 1])) {
        start[length - 1] = '\0';
        length--;
    }

    return start;
}

int readLine(char *buffer, size_t size)
{
    size_t length;

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        /* The line was longer than the buffer: drop the remaining characters. */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* discard */
        }
    }

    return 1;
}

int parseHazchemCode(const char *code, char *materialChar, char *letterChar,
                     int *hasEvacuation)
{
    size_t length = strlen(code);
    char first;
    char second;

    if (length != 2 && length != 3) {
        return 0;
    }

    first = code[0];
    if (first < '1' || first > '4') {
        return 0;
    }

    second = (char)toupper((unsigned char)code[1]);
    if (strchr(LETTER_SET, second) == NULL) {
        return 0;
    }

    if (length == 3) {
        if (toupper((unsigned char)code[2]) != 'E') {
            return 0;
        }
        *hasEvacuation = 1;
    } else {
        *hasEvacuation = 0;
    }

    *materialChar = first;
    *letterChar = second;
    return 1;
}

int needsColourQuestion(char letter)
{
    return (letter == 'S' || letter == 'T' || letter == 'Y' || letter == 'Z');
}

int askReverseColoured(char letter, int *reversed)
{
    char buffer[INPUT_SIZE];
    char *answer;

    for (;;) {
        printf("Is the %c reverse coloured? ", letter);
        fflush(stdout);

        if (!readLine(buffer, sizeof(buffer))) {
            return 0;
        }

        answer = trimWhitespace(buffer);
        if (strcasecmp(answer, "yes") == 0) {
            *reversed = 1;
            return 1;
        }
        if (strcasecmp(answer, "no") == 0) {
            *reversed = 0;
            return 1;
        }

        printf("Please answer yes or no.\n");
    }
}

/**
 * @function findMaterial: Look for the material matching the first character.
 *
 * @param digit: The first character of the code ('1'..'4').
 *
 * @return: A pointer to the material text, or NULL if the character is unknown.
 */
static const char *findMaterial(char digit)
{
    size_t index;

    for (index = 0; index < MATERIAL_COUNT; index++) {
        if (MATERIAL_TABLE[index].digit == digit) {
            return MATERIAL_TABLE[index].material;
        }
    }

    return NULL;
}

/**
 * @function findLetter: Look for the row matching the second character and its colour.
 *
 * @param letter: The second character of the code, in upper case.
 * @param reversed: 1 if the letter is reverse coloured, 0 otherwise.
 *
 * @return: A pointer to the matching row, or NULL if no row matches.
 */
static const LetterEntry *findLetter(char letter, int reversed)
{
    size_t index;

    for (index = 0; index < LETTER_COUNT; index++) {
        if (LETTER_TABLE[index].letter != letter) {
            continue;
        }
        if (LETTER_TABLE[index].reversed == ANY_COLOUR ||
            LETTER_TABLE[index].reversed == reversed) {
            return &LETTER_TABLE[index];
        }
    }

    return NULL;
}

int buildAdvice(char materialChar, char letterChar, int reversed,
                int hasEvacuation, HazchemAdvice *advice)
{
    const char *material = findMaterial(materialChar);
    const LetterEntry *entry = findLetter(letterChar, reversed);

    if (material == NULL || entry == NULL) {
        return 1;
    }

    advice->material = material;
    advice->reactivity = entry->reactivity;
    advice->protection = entry->protection;
    advice->containment = entry->containment;
    advice->evacuation = hasEvacuation;
    return 0;
}

void printAdvice(const HazchemAdvice *advice)
{
    printf("***Emergency action advice***\n");
    printf("Material: %s\n", advice->material);
    if (advice->reactivity != NULL) {
        printf("Reactivity: %s\n", advice->reactivity);
    }
    printf("Protection: %s\n", advice->protection);
    printf("Containment: %s\n", advice->containment);
    if (advice->evacuation) {
        printf("Evacuation: %s\n", EVACUATION_TEXT);
    }
    printf("*****************************\n");
}
