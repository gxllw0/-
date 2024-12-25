#ifndef MANAGE_SORTING_AND_STROAGE_H
#define MANAGE_SORTING_AND_STROAGE_H

#include "parcelInfo.h"

// �������������ݽṹ
typedef struct QueueNode {
    Parcel parcel;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// �������ڵ����ݽṹ
typedef struct TreeNode {
    char address[50];
    Parcels parcels; // ���������ַ��ͬ����һ�� 
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// �����ݹ�ϵͳ���ݽṹ
typedef struct Locker {
    int lockerNumber;
    char pickupCode[10]; // ȡ����
    Parcel parcel;
    struct Locker* next; // ָ���¸���ݹ��ָ��
} Locker;

typedef struct LockersList {
    Locker* head;
    Locker* tail;
} LockersList;

// ���� 
void initQueue(Queue* q);
int enqueue(Queue* q, Parcel parcel);
Parcel dequeue(Queue* q);
int isQueueEmpty(Queue* q);

// �� 
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
