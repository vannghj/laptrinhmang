#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "hazchem.h"

int checkCode(const char *code, char *firstChar, char *letterChar,
              int *hasEvacuation)
{
    int len = strlen(code);

    if (len != 2 && len != 3) {
        return 0;
    }
    if (code[0] < '1' || code[0] > '4') {
        return 0;
    }

    char letter = (char)toupper((unsigned char)code[1]);
    if (strchr("PRSTWXYZ", letter) == NULL) {
        return 0;
    }

    if (len == 3) {
        if (toupper((unsigned char)code[2]) != 'E') {
            return 0;
        }
        *hasEvacuation = 1;
    } else {
        *hasEvacuation = 0;
    }

    *firstChar = code[0];
    *letterChar = letter;
    return 1;
}

int needsColourQuestion(char letterChar)
{
    return (letterChar == 'S' || letterChar == 'T' ||
            letterChar == 'Y' || letterChar == 'Z');
}

/**
 * @function getMaterial: Tra ten vat lieu xu ly theo ky tu thu nhat.
 *
 * @param firstChar: Ky tu thu nhat cua ma ('1'..'4').
 *
 * @return: Chuoi mo ta vat lieu tuong ung.
 */
static const char *getMaterial(char firstChar)
{
    switch (firstChar) {
        case '1': return "jets";
        case '2': return "fog";
        case '3': return "foam";
        default:  return "dry agent";
    }
}

void buildAdvice(char firstChar, char letterChar, int reversed,
                 int hasEvacuation, Advice *advice)
{
    advice->material = getMaterial(firstChar);
    advice->hasEvacuation = hasEvacuation;

    switch (letterChar) {
        case 'P':
            advice->reactivity = "can be violently reactive";
            advice->protection = "full protective clothing must be worn";
            advice->containment = "may be diluted and washed down the drain";
            break;
        case 'R':
            advice->reactivity = NULL;
            advice->protection = "full protective clothing must be worn";
            advice->containment = "may be diluted and washed down the drain";
            break;
        case 'S':
            advice->reactivity = "can be violently reactive";
            advice->protection = reversed
                ? "breathing apparatus, protective gloves for fire only"
                : "breathing apparatus";
            advice->containment = "may be diluted and washed down the drain";
            break;
        case 'T':
            advice->reactivity = NULL;
            advice->protection = reversed
                ? "breathing apparatus, protective gloves for fire only"
                : "breathing apparatus";
            advice->containment = "may be diluted and washed down the drain";
            break;
        case 'W':
            advice->reactivity = "can be violently reactive";
            advice->protection = "full protective clothing must be worn";
            advice->containment = "must be contained, prevent from entering drains or water courses";
            break;
        case 'X':
            advice->reactivity = NULL;
            advice->protection = "full protective clothing must be worn";
            advice->containment = "must be contained, prevent from entering drains or water courses";
            break;
        case 'Y':
            advice->reactivity = "can be violently reactive";
            advice->protection = reversed
                ? "breathing apparatus, protective gloves for fire only"
                : "breathing apparatus";
            advice->containment = "must be contained, prevent from entering drains or water courses";
            break;
        default: /* 'Z' */
            advice->reactivity = NULL;
            advice->protection = reversed
                ? "breathing apparatus, protective gloves for fire only"
                : "breathing apparatus";
            advice->containment = "must be contained, prevent from entering drains or water courses";
            break;
    }
}

void printAdvice(const Advice *advice)
{
    printf("***Emergency action advice***\n");
    printf("Material: %s\n", advice->material);
    if (advice->reactivity != NULL) {
        printf("Reactivity: %s\n", advice->reactivity);
    }
    printf("Protection: %s\n", advice->protection);
    printf("Containment: %s\n", advice->containment);
    if (advice->hasEvacuation) {
        printf("Evacuation: consider evacuation\n");
    }
    printf("*****************************\n");
}
