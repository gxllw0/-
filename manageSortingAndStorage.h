#ifndef MANAGE_SORTING_AND_STROAGE_H
#define MANAGE_SORTING_AND_STROAGE_H

#include "parcelInfo.h"

// 队列
typedef struct QueueNode {
    Parcel parcel;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// 树
typedef struct TreeNode {
    char address[50];
    Parcels parcels; // 快递链表，地址相同存在一起 
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 快递柜
typedef struct Locker {
	Parcel* head;	
} Locker;

// 初始化队列 
void initQueue(Queue* q);
// 进队 
int enqueue(Queue* q, Parcel parcel);
// 出队 
Parcel dequeue(Queue* q);
// 判断队列是否为空 
int isQueueEmpty(Queue* q);

// 创建树节点 
TreeNode* createTreeNode(const char* address);
// 将快递插入树中 
void insertIntoTree(TreeNode** root, const char* address, Parcel parcel);
// 从树中移除快递 
void removeParcelFromTree(TreeNode **root, int parcelId, const char* address);
// 释放整棵树 
void destroyTree(TreeNode* root);

// 将待处理的快递放入队列中 
void movePendingToQueue(Parcels *parcels, Queue *sortingQueue);
// 将队列中快递分类存入树中 
void sortParcelsFromQueue(Queue* sortingQueue, TreeNode** root);

#endif
