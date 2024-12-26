#ifndef SYSTEM_H
#define SYSTEM_H

#include "parcelInfo.h"
#include "usersManage.h"
#include "manageSortingAndStorage.h"
#include "manageDelivery.h"
#include "statistic.h"

typedef struct {
    Parcels* parcels; 
    HashTable* ht; 
    Queue sortingQueue;
    TreeNode* root;
    Graph graph;
    PriorityQueue pq;
	AddressStats addressStats[NUM_ADDRESSES];
	StatusStats statusStats[NUM_STATUS];    
} SystemContext;

// 初始化所有数据结构 
SystemContext* initializeSystem();
// 释放所有数据结构 
void cleanupSystem(SystemContext *context);

#endif
