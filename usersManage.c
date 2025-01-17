#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usersManage.h"


void setCurrentUser(User* user) {
    currentUser = user;
}

unsigned int hash(char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) - hash + *key++;
    }
    return hash % TABLE_SIZE;
}

HashTable* createTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable)); 
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

void freeTable(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = ht->table[i];
        while (current != NULL) {
            User* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

User* findUser(HashTable* ht, char* username) {
    unsigned int index = hash(username);
    User* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

char* roleToString(Role role) {
    switch (role) {
        case STUDENT: return "学生";
        case COURIER: return "快递员";
        case ADMIN: return "管理员";
        default: return "未知用户";
    }
}

Role stringToRole(const char* roleStr) {
    if (strcmp(roleStr, "学生") == 0) return STUDENT;
    if (strcmp(roleStr, "快递员") == 0) return COURIER;
    if (strcmp(roleStr, "管理员") == 0) return ADMIN;
    return STUDENT;
}

void registerUser(HashTable* ht, char* username, char* password, Role role) {
    if (findUser(ht, username) != NULL) {
        printf("用户名已存在\n");
        return;
    }
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->role = role;
    newUser->next = NULL;

    unsigned int index = hash(username);
    if (ht->table[index] == NULL) {
        ht->table[index] = newUser;
    } else {
        User* current = ht->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newUser;
    }
    printf("注册成功\n");
}

void loginUser(HashTable* ht, char* username, char* password) {
    User* user = findUser(ht, username);
    if (user == NULL || strcmp(user->password, password) != 0) {
        printf("用户名或密码错误\n");
        return;
    }
    setCurrentUser(user); // 设置当前用户
    printf("欢迎回来，%s！您的角色是：%s\n", username, roleToString(user->role));
}

void changePassword(HashTable* ht, char* username, char* old_password, char* new_password) {
    User* user = findUser(ht, username);
    if (user == NULL || strcmp(user->password, old_password) != 0) {
        printf("旧密码错误\n");
        return;
    }
    strcpy(user->password, new_password);
    printf("密码更改成功\n");
}

void getUserInfo(HashTable* ht, char* username) {
    User* user = findUser(ht, username);
    if (user == NULL) {
        printf("用户不存在\n");
        return;
    }
    printf("用户名: %s, 角色: %s\n", username, roleToString(user->role));
}

void updateUserInfo(HashTable* ht, char* username, Role new_role) {
    User* user = findUser(ht, username);
    if (user == NULL) {
        printf("用户不存在\n");
        return;
    }
    user->role = new_role;
    printf("用户信息更新成功\n");
}
