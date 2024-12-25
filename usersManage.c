#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usersManage.h"


void setCurrentUser(User* user) {
    currentUser = user;
}

User* getCurrentUser() {
    return currentUser;
}

unsigned int hash(char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) - hash + *key++;
    }
    return hash % TABLE_SIZE;
}

HashTable* createTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable)); // ��ʽ����ת��
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
        case STUDENT: return "student";
        case COURIER: return "courier";
        case ADMIN: return "admin";
        default: return "unknown";
    }
}

Role stringToRole(const char* roleStr) {
    if (strcmp(roleStr, "student") == 0) return STUDENT;
    if (strcmp(roleStr, "courier") == 0) return COURIER;
    if (strcmp(roleStr, "admin") == 0) return ADMIN;
    return STUDENT;
}

void registerUser(HashTable* ht, char* username, char* password, Role role) {
    if (findUser(ht, username) != NULL) {
        printf("�û����Ѵ���\n");
        return;
    }
    User* newUser = (User*)malloc(sizeof(User)); // ��ʽ����ת��
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
    printf("ע��ɹ�\n");
}

void loginUser(HashTable* ht, char* username, char* password) {
    User* user = findUser(ht, username);
    if (user == NULL || strcmp(user->password, password) != 0) {
        printf("�û������������\n");
        return;
    }
    setCurrentUser(user); // ���õ�ǰ�û�
    printf("��ӭ������%s�����Ľ�ɫ�ǣ�%s\n", username, roleToString(user->role));
}

void changePassword(HashTable* ht, char* username, char* old_password, char* new_password) {
    User* user = findUser(ht, username);
    if (user == NULL || strcmp(user->password, old_password) != 0) {
        printf("���������\n");
        return;
    }
    strcpy(user->password, new_password);
    printf("������ĳɹ�\n");
}

void getUserInfo(HashTable* ht, char* username) {
    User* user = findUser(ht, username);
    if (user == NULL) {
        printf("�û�������\n");
        return;
    }
    printf("�û���: %s, ��ɫ: %s\n", username, roleToString(user->role));
}

void updateUserInfo(HashTable* ht, char* username, Role new_role) {
    User* user = findUser(ht, username);
    if (user == NULL) {
        printf("�û�������\n");
        return;
    }
    user->role = new_role;
    printf("�û���Ϣ���³ɹ�\n");
}

int hasPermission(User* user, Role required_role) {
    if (user == NULL) {
        return 0;
    }
    return user->role >= required_role;
}
