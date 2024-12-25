#ifndef MANAGE_SORTING_AND_STROAGE_H
#define MANAGE_SORTING_AND_STROAGE_H

#include "parcelInfo.h"

// ����
typedef struct QueueNode {
    Parcel parcel;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// ��
typedef struct TreeNode {
    char address[50];
    Parcels parcels; // ���������ַ��ͬ����һ�� 
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// ��ݹ�
typedef struct Locker {
	Parcel* head;	
} Locker;

// ��ʼ������ 
void initQueue(Queue* q);
// ���� 
int enqueue(Queue* q, Parcel parcel);
// ���� 
Parcel dequeue(Queue* q);
// �ж϶����Ƿ�Ϊ�� 
int isQueueEmpty(Queue* q);

// �������ڵ� 
TreeNode* createTreeNode(const char* address);
// ����ݲ������� 
void insertIntoTree(TreeNode** root, const char* address, Parcel parcel);
// �������Ƴ���� 
void removeParcelFromTree(TreeNode **root, int parcelId, const char* address);
// �ͷ������� 
void destroyTree(TreeNode* root);

// ��������Ŀ�ݷ�������� 
void movePendingToQueue(Parcels *parcels, Queue *sortingQueue);
// �������п�ݷ���������� 
void sortParcelsFromQueue(Queue* sortingQueue, TreeNode** root);

#endif
