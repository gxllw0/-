#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"

int getIntChoice(void) {
    int choice;
    while (scanf("%d", &choice) != 1) {
        clearInputBuffer(); // ������뻺�����е���Ч����
        printf("��Ч��ѡ�����������롣\n");
        printf("����������ѡ��: ");
    }
    return choice;
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n');
}

void generateCode(char *code) {
    for (int i = 0; i < 6; ++i) {
        code[i] = '0' + rand() % 10;
    }

    code[6] = '\0';
}
