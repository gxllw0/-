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

// 设置当前用户 
void setCurrentUser(User* user);
// 哈希函数，将根据用户名转化为数字 
unsigned int hash(char* key);
// 创建哈希表 
HashTable* createTable();
// 释放哈希表 
void freeTable(HashTable* ht);
// 查找用户 
User* findUser(HashTable* ht, char* username);
// 获取当前角色（字符串） 
char* roleToString(Role role);
// 获取当前角色类型 
Role stringToRole(const char* roleStr);
// 注册用户 
void registerUser(HashTable* ht, char* username, char* password, Role role);
// 用户登录 
void loginUser(HashTable* ht, char* username, char* password);
// 修改密码 
void changePassword(HashTable* ht, char* username, char* old_password, char* new_password);
	// 获取对应用户信息 
	void getUserInfo(HashTable* ht, char* username);
// 更新用户权限 
void updateUserInfo(HashTable* ht, char* username, Role new_role);

#endif // USERS_MANAGE_H
