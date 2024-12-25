#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manageSortingAndStorage.h" 

// 队列操作的实现
void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

int enqueue(Queue* q, Parcel parcel) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return 0; // 内存分配失败

    newNode->parcel = parcel;
    newNode->next = NULL;

    if (isQueueEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    return 1;
}

Parcel dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        printf("队列为空，无法出队\n");
        exit(1); 
    }

    QueueNode* temp = q->front;
    Parcel parcel = temp->parcel;
    q->front = q->front->next;

    if (!q->front) {
        q->rear = NULL;
    }

    free(temp);
    return parcel;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

// 树操作的实现
TreeNode* createTreeNode(const char* address) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(node->address, address);
    initParcels(&node->parcels);
    node->left = node->right = NULL;
    return node;
}

void insertIntoTree(TreeNode** root, const char* address, Parcel parcel) {
    if (*root == NULL) {
        *root = createTreeNode(address);
    }

    TreeNode* current = *root;
    while (strcmp(current->address, address) != 0) {
        int cmp = strcmp(address, current->address);
        if (cmp < 0 && current->left) {
            current = current->left;
        } else if (cmp > 0 && current->right) {
            current = current->right;
        } else {
            break;
        }
    }

    if (strcmp(current->address, address) != 0) {
        if (strcmp(address, current->address) < 0) {
            current->left = createTreeNode(address);
            current = current->left;
        } else {
            current->right = createTreeNode(address);
            current = current->right;
        }
    }

    addParcelInfo(&current->parcels, parcel.id, parcel.sender, parcel.receiver, parcel.address, "000");
}

// 根据地址查找树中的节点
TreeNode* findNodeByAddress(TreeNode *root, const char *address) {
    while (root != NULL && strcmp(root->address, address) != 0) {
        if (strcmp(address, root->address) < 0)
            root = root->left;
        else
            root = root->right;
    }
    return root; // 返回找到的节点或NULL（如果未找到）
}

// 辅助函数：从同一地址下的包裹链表中移除特定ID的包裹
int removeParcelFromList(Parcels *parcels, int parcelId) {
    if (parcels == NULL || parcels->head == NULL) return 0; // 空链表检查

    Parcel *current = parcels->head, *prev = NULL;

    // 查找包裹
    while (current != NULL && current->id != parcelId) {
        prev = current;
        current = current->next;
    }

    // 没有找到对应的包裹
    if (current == NULL) return 0;

    // 移除该包裹
    if (prev == NULL) { // 移除的是头节点
        parcels->head = current->next;
    } else {
        prev->next = current->next;
    }

    // 释放包裹节点内存
    free(current);

    return 1; // 返回成功标志
}

// 辅助函数：找到最小的节点（用于寻找继任者）
TreeNode* findMinNode(TreeNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// 辅助函数：删除节点（假设已经找到了要删除的节点）
void deleteNode(TreeNode **root, const char *address) {
    if (*root == NULL) return;

    if (strcmp((*root)->address, address) < 0) {
        deleteNode(&(*root)->right, address);
    } else if (strcmp((*root)->address, address) > 0) {
        deleteNode(&(*root)->left, address);
    } else { // 找到了要删除的节点
        TreeNode *temp = *root;
        if ((*root)->left == NULL) {
            *root = (*root)->right;
        } else if ((*root)->right == NULL) {
            *root = (*root)->left;
        } else { // 有两个子节点的情况
            // 找到右子树中最左边的节点作为继任者
            temp = findMinNode(temp->right);
            strcpy((*root)->address, temp->address);
            (*root)->parcels = temp->parcels;
            *root = (*root)->right; // 因为继任者肯定没有左子树
        }
        free(temp);
    }
}

void removeParcelFromTree(TreeNode **root, int parcelId, const char *address) {
    // 首先根据地址找到节点
    TreeNode *node = findNodeByAddress(*root, address);
    if (node == NULL) return; // 如果地址找不到则直接返回

    // 尝试从包裹链表中移除包裹
    if (removeParcelFromList(&node->parcels, parcelId)) {
        // 如果该地址下的包裹链表为空，则移除整个节点
        if (node->parcels.head == NULL) {
            deleteNode(root, address);
        }
    }
}

// 查找特定ID的包裹
Parcel* findParcelById(Parcels *parcels, int parcelId) {
    Parcel *current = parcels->head;
    while (current != NULL && current->id != parcelId) {
        current = current->next;
    }
    return current; // 返回找到的包裹或NULL（如果未找到）
}

// 组合函数，先按地址查找节点，再按ID查找包裹
Parcel* findParcel(TreeNode *root, const char *address, int parcelId) {
    TreeNode *node = findNodeByAddress(root, address);
    if (node == NULL) return NULL; // 地址未找到
    return findParcelById(&node->parcels, parcelId); // 在找到的节点中查找包裹
}

void destroyTree(TreeNode* root) {
    if (!root) return;
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

void movePendingToQueue(Parcels *parcels, Queue *sortingQueue) {
    // 创建一个临时指针来遍历链表
    Parcel *current = parcels->head, *prev = NULL;
    
    while (current != NULL) {
        if (current->Status == PENDING_SORTING) {
            // 将当前节点加入队列
            current->Status = SORTED;
            enqueue(sortingQueue, *current);
            
            // 更新指针
            Parcel *temp = current;
            current = current->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

//分拣快递 
void sortParcelsFromQueue(Queue* sortingQueue, TreeNode** root) {
    while (!isQueueEmpty(sortingQueue)) {
        Parcel parcel = dequeue(sortingQueue);
        insertIntoTree(root, parcel.address, parcel);
    }
}
