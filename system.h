#ifndef SYSTEM_H
#define SYSTEM_H

#include "parcelInfo.h"
#include "usersManage.h"
#include "manageSortingAndStorage.h"
#include "manageDelivery.h"

typedef struct {
    Parcels* parcels; 
    HashTable* ht; 
    Queue sortingQueue;
    TreeNode* root;
    LockersList lockers;
    Graph graph;
    PriorityQueue pq;
} SystemContext;

// ��ʼ���������ݽṹ 
SystemContext* initializeSystem();
// �ͷ��������ݽṹ 
void cleanupSystem(SystemContext *context);

#endif
