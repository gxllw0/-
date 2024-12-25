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

// ���õ�ǰ�û� 
void setCurrentUser(User* user);
// ��ϣ�������������û���ת��Ϊ���� 
unsigned int hash(char* key);
// ������ϣ�� 
HashTable* createTable();
// �ͷŹ�ϣ�� 
void freeTable(HashTable* ht);
// �����û� 
User* findUser(HashTable* ht, char* username);
// ��ȡ��ǰ��ɫ���ַ����� 
char* roleToString(Role role);
// ��ȡ��ǰ��ɫ���� 
Role stringToRole(const char* roleStr);
// ע���û� 
void registerUser(HashTable* ht, char* username, char* password, Role role);
// �û���¼ 
void loginUser(HashTable* ht, char* username, char* password);
// �޸����� 
void changePassword(HashTable* ht, char* username, char* old_password, char* new_password);
	// ��ȡ��Ӧ�û���Ϣ 
	void getUserInfo(HashTable* ht, char* username);
// �����û�Ȩ�� 
void updateUserInfo(HashTable* ht, char* username, Role new_role);

#endif // USERS_MANAGE_H
