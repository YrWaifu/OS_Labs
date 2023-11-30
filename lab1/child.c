#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

float summ(char* array) {
    char* newStr;
    int strBuf;
    int strSize;
    float number = 0;
    float finalSum = 0;
    int iter = 0;
    while (array[iter] != '\n' && array[iter] != '\0') {
        strBuf = 1;
        strSize = 0;
        newStr = malloc(sizeof(char));
        while (array[iter] != '\0' && array[iter] != ' ' && array[iter] != '\n' && array[iter] != '\t') {
            if (strBuf <= strSize) {
                strBuf *= 2;
                newStr = realloc(newStr, sizeof(char) * strBuf);
            }
            newStr[strSize] = array[iter];
            strSize += 1;
            iter += 1;
        }
        if (newStr != NULL) {
            number = atof(newStr);
            finalSum += number;
        }
        free(newStr);
        iter += 1;
    }

    return finalSum;
}

int dataToString(char** str, int* strBuf, int* strSize) {
    char c;
    int res;
    errno = 0;

    char* newStr = malloc(*strBuf * sizeof(char));

    while (1) {
        if ((res = read(STDIN_FILENO, &c, sizeof(char))) > 0 && c != '\n') {
            if (strBuf <= strSize) {
                *strBuf *= 2;
                newStr = realloc(newStr, *strBuf * sizeof(char));
            }
            newStr[*strSize] = c;
            *strSize += 1;
        } else {
            break;
        }
    }
    newStr[*strSize] = '\0';
    if (res == -1) return errno;
    free(*str);
    *str = newStr;
    return 0;
}

int main() {
    int strSize = 0;
    int strBuff = 1;
    char* newStr = NULL;
    dataToString(&newStr, &strBuff, &strSize);
    float res = summ(newStr);
    if ((write(STDOUT_FILENO, &res, sizeof(res)) == -1)) perror("write error");

    return 0;
}