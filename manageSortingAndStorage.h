#ifndef MANAGE_SORTING_AND_STROAGE_H
#define MANAGE_SORTING_AND_STROAGE_H

#include "parcelInfo.h"

// 定义队列相关数据结构
typedef struct QueueNode {
    Parcel parcel;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// 定义树节点数据结构
typedef struct TreeNode {
    char address[50];
    Parcels parcels; // 快递链表，地址相同存在一起 
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 定义快递柜系统数据结构
typedef struct Locker {
    int lockerNumber;
    char pickupCode[10]; // 取件码
    Parcel parcel;
    struct Locker* next; // 指向下个快递柜的指针
} Locker;

typedef struct LockersList {
    Locker* head;
    Locker* tail;
} LockersList;

// 队列 
void initQueue(Queue* q);
int enqueue(Queue* q, Parcel parcel);
Parcel dequeue(Queue* q);
int isQueueEmpty(Queue* q);

// 树 
TreeNode* createTreeNode(const char* address);
void insertIntoTree(TreeNode** root, const char* address, Parcel parcel);
void removeParcelFromTree(TreeNode **root, int parcelId, const char* address);
int removeParcelFromList(Parcels *parcels, int parcelId);
void deleteNode(TreeNode **root, const char *address); 
TreeNode* findMinNode(TreeNode *node);
void destroyTree(TreeNode* root);
void sortParcelsFromQueue(Queue* sortingQueue, TreeNode** root);

void initLockers(LockersList* lockers);
Locker* allocateLocker(LockersList* lockers, Parcel parcel);
void releaseLocker(LockersList* lockers, int lockerNumber);
void printAllLockers(LockersList* lockers);
void destroyLockers(LockersList* lockers);
void manageLockers(LockersList* lockers);
void movePendingToQueue(Parcels *parcels, Queue *sortingQueue);

#endif
