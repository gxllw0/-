#include <stdio.h>
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
