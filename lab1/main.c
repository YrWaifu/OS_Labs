#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int readFileName(char **name, int *len) {
    errno = 0;
    int newLen = 0;
    int capacity = 1;
    char *newName = (char *)malloc(capacity * sizeof(char));
    if (newName == NULL) return -1;
    char c;
    int resRead;
    while ((resRead = read(STDIN_FILENO, &c, sizeof(char))) > 0 && c != '\n') {
        newName[newLen] = c;
        newLen += 1;
        if (newLen >= capacity) {
            capacity *= 2;
            newName = (char *)realloc(newName, sizeof(char) * capacity);
            if (newName == NULL) return -1;
        }
    }
    newName[newLen] = '\0';
    if (resRead == -1) return errno;
    free(*name);
    *name = newName;
    *len = newLen;
    if (resRead == 0) return EOF;
    return 0;
}

int main() {
    char *fileName = NULL;
    errno = 0;
    int fd[2];
    int fileLen = 0;
    printf("Write the file you want to execute!\n");
    if (readFileName(&fileName, &fileLen)) {
        perror("reading file error!\n");
        return -1;
    }

    int file = open(fileName, O_RDWR | O_CREAT, 0777);
    if (file == -1) {
        perror("opening file error!\n");
        return 2;
    }

    if (pipe(fd) == -1) {
        perror("An error occurred with opening the pipe\n");
        return 1;
    }

    int id = fork();

    if (id == -1) {
        perror("fork error");
        return 9;
    } else if (id == 0) {
        if (dup2(file, STDIN_FILENO) == -1) {
            perror("dup2 error");
            return 10;
        } else if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2 error");
            return 11;
        } else if (close(fd[0])) {
            perror("close pipe error");
            return 12;
        } else if (close(fd[1])) {
            perror("close pipe error");
            return 13;
        } else {
            execl("./child", "./child", (char *)NULL);
            perror("execl error");
            return 14;
        }
    } else {
        if (close(file)) {
            perror("close file error");
            return 13;
        }
        float res;
        if (read(fd[0], &res, sizeof(res)) == -1) {
            perror("read file error");
            return 15;
        }
        close(fd[0]);
        printf("[%d] Result from child: %f\n", getpid(), res);
    }
    return 0;
}
