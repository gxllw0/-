#ifndef USERS_MANAGE_H
#define USERS_MANAGE_H

#define TABLE_SIZE 100

typedef enum { STUDENT, COURIER, ADMIN } Role;

typedef struct User {
    char username[50];
    char password[50];
    Role role;
    struct User* next;
} User;

typedef struct HashTable {
    User* table[TABLE_SIZE];
} HashTable;

User* currentUser;

// 设置当前的用户 
void setCurrentUser(User* user);
User* getCurrentUser();
unsigned int hash(char* key);
HashTable* createTable();
void freeTable(HashTable* ht);
User* findUser(HashTable* ht, char* username);
char* roleToString(Role role);
Role stringToRole(const char* roleStr);
void registerUser(HashTable* ht, char* username, char* password, Role role);
void loginUser(HashTable* ht, char* username, char* password);
void changePassword(HashTable* ht, char* username, char* old_password, char* new_password);
void getUserInfo(HashTable* ht, char* username);
void updateUserInfo(HashTable* ht, char* username, char* newUserName, Role new_role);
int hasPermission(User* user, Role required_role);

#endif // USERS_MANAGE_H
