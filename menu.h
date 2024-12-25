#ifndef MENU_H
#define MENU_H

#include "usersManage.h"
#include "system.h"

// ��ʾ��¼ע��˵�
void displayLoginOrRegisterMenu();
// ע��˵� 
void registerMenu(HashTable* ht);
// ��¼
void processUserLogin(HashTable *ht); 
// ���ݲ�ͬ��ɫ���ɵ�¼���� 
void displayMenu();
// ��������ǰ����Ӧģ�� 
void handleUserInput(SystemContext *context, int choice);
// �����Ϣ���� 
void manageParcelInfo(Parcels* parcels); 
// �û����� 
void manageUsers(HashTable* ht);
// ��ݷּ���洢ģ�� 
void manageSortingAndStorage(Parcels* parcels, Queue sortingQueue, TreeNode* root, LockersList lockers); 
// �������ģ�� 
void manageDelivery(Parcels *parcels, TreeNode* root, Graph graph, PriorityQueue* pq);
// ���ݿ��ӻ�ģ�� 
void generateStatistics();

#endif
