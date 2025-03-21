#ifndef CALC_H
#define CALC_H

#define ANS_FILE_PATH "./.calc_ans"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double ANS = 0;

// METHODS

void load_ans() {
    FILE *ans_file = fopen(ANS_FILE_PATH, "r");
    if (ans_file != NULL) {
        fscanf(ans_file, "%lf", &ANS);
        fclose(ans_file);
    } else {
        ANS = 0; // Default ANS if file doesn't exist
    }
}

void save_ans() {
    FILE *ans_file = fopen(ANS_FILE_PATH, "w");
    if (ans_file != NULL) {
        fprintf(ans_file, "%.2f", ANS);
        fclose(ans_file);
    }
}

int calculate(char *input_operation) {
    char number_1_str[256], operator_str[256], number_2_str[256];
    double num1, num2, result;

    // Parse input using sscanf - more robust than strtok for this fixed format
    if (sscanf(input_operation, "%s %s %s", number_1_str, operator_str, number_2_str) != 3) {
        printf("SYNTAX ERROR\n");
        return 1; // Indicate error
    }

    // Handle "ANS" input for number_1
    if (strcmp(number_1_str, "ANS") == 0) {
        num1 = ANS;
    } else if (isdigit(number_1_str[0]) || ((number_1_str[0] == '-' || number_1_str[0] == '+') && isdigit(number_1_str[1]))) {
        num1 = atof(number_1_str);
    }
     else {
        printf("SYNTAX ERROR\n");
        return 1;
    }


    // Handle "ANS" input for number_2
    if (strcmp(number_2_str, "ANS") == 0) {
        num2 = ANS;
    } else if (isdigit(number_2_str[0]) || ((number_2_str[0] == '-' || number_2_str[0] == '+') && isdigit(number_2_str[1]))) {
        num2 = atof(number_2_str);
    }
    else {
        printf("SYNTAX ERROR\n");
        return 1;
    }


    char operator_char = operator_str[0];

    switch (operator_char) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case 'x':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                printf("MATH ERROR\n");
                return 1;
            }
            result = num1 / num2;
            break;
        case '%':
            if (num2 == 0) {
                printf("MATH ERROR\n");
                return 1;
            }
            result = (int)num1 % (int)num2;
            break;
        default:
            printf("SYNTAX ERROR\n");
            return 1;
    }

    ANS = result;
    char calculation_entry[256];
    sprintf(calculation_entry, "%.2f %c %.2f = %.2f", num1, operator_char, num2, ANS);

    if (ANS / (int)ANS == 1.0) printf("%d\n", (int)ANS);
    else printf("%.2f\n", ANS);
    save_ans();
    return 0; // Indicate success
}

int calc() {
    char input[256];

    load_ans();

    while (1) {
        printf(">> ");
        if (fgets(input, sizeof(input), stdin) == NULL) { // Use fgets to read line
            break; // Handle EOF (Ctrl+D)
        }
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        if (strcmp(input, "EXIT") == 0) {
            break;
        } else {
            calculate(input);
        }
    }

    return 0;
}

#endif // CALC_H