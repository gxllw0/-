#include <stdio.h>
#include "util.h"
 
int getIntChoice(void) {
    int choice;
    while (scanf("%d", &choice) != 1) {
        clearInputBuffer(); // 清除输入缓冲区中的无效数据
        printf("无效的选择，请重新输入。\n");
        printf("请输入您的选择: ");
    }
    return choice;
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n');
}
