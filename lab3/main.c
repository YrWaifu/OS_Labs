#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHARED_OBJ_NAME "/message"

// shared data struct
typedef struct {
    float summ;
} message;

bool write_message(float value) {
    int shmFd = shm_open(SHARED_OBJ_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    assert(shmFd != -1);
    assert(ftruncate(shmFd, sizeof(message)) != -1);
    message* msg_ptr = (message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    assert(msg_ptr != MAP_FAILED);
    msg_ptr->summ = value;
    assert(munmap(msg_ptr, sizeof(message)) != -1);
    assert(close(shmFd) != -1);

    return true;
}

float read_message() {
    int shmFd = shm_open(SHARED_OBJ_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    assert(shmFd != -1);
    assert(ftruncate(shmFd, sizeof(message)) != -1);
    message* msg_ptr = (message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    assert(msg_ptr != MAP_FAILED);
    float currValue = msg_ptr->summ;
    assert(munmap(msg_ptr, sizeof(message)) != -1);
    assert(close(shmFd) != -1);

    return currValue;
}

int read_filename(char** name, int* len) {
    int newLen = 0;
    int capacity = 1;
    char* newName = (char*)malloc(capacity * sizeof(char));
    if (newName == NULL) return -1;
    char c;
    int resRead;
    while ((resRead = read(STDIN_FILENO, &c, sizeof(char))) > 0 && c != '\n') {
        newName[newLen] = c;
        newLen += 1;
        if (newLen >= capacity) {
            capacity *= 2;
            newName = (char*)realloc(newName, sizeof(char) * capacity);
            if (newName == NULL) return -1;
        }
    }
    newName = (char*)realloc(newName, sizeof(char) * capacity + 1);
    newName[newLen] = '\0';
    if (resRead == -1) return -1;
    free(*name);
    *name = newName;
    *len = newLen;
    if (resRead == 0) return EOF;
    return 0;
}

int dataToString(char** str, int* strBuf, int* strSize, int fd) {
    char c;
    int res;

    char* newStr = malloc(*strBuf * sizeof(char));

    while (1) {
        if ((res = read(fd, &c, sizeof(char))) > 0 && c != '\n' && c != '\0') {
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
    if (res == -1) return -1;
    free(*str);
    *str = newStr;
    return 0;
}

float summ(char* array) {
    char* newStr;
    int strBuf;
    int strSize;
    float number = 0;
    float finalSumm = 0;
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
            finalSumm += number;
        }
        free(newStr);
        iter += 1;
    }

    return finalSumm;
}

int main(int argc, char** argv) {
    char* filename = NULL;
    int fileLen = 0;
    printf("Write file that u want to execute!\n");
    if (read_filename(&filename, &fileLen) == -1) {
        perror("reading file error!\n");
        return 1;
    }
    int file;
    if ((file = open(filename, O_RDWR | O_CREAT, 0777)) == -1) {
        perror("opening msg error!\n");
        return 2;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork errror!\n");
        return 3;
    }
    if (pid == 0) {
        int strSize = 0;
        int strBuff = 1;
        char* newStr = NULL;
        dataToString(&newStr, &strBuff, &strSize, file);
        float sumF = summ(newStr);
        assert(write_message(sumF) != false);
    } else {
        int statusChild1;
        waitpid(pid, &statusChild1, 0);
        float read_data = read_message();
        if (close(file)) {
            perror("close file error");
            return 5;
        }
        printf("[%d] Result from child: %f\n", getpid(), read_data);
        shm_unlink(SHARED_OBJ_NAME);
    }

    return 0;
}