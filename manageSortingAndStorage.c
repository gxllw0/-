#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manageSortingAndStorage.h" 

// ���в�����ʵ��
void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

int enqueue(Queue* q, Parcel parcel) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return 0; // �ڴ����ʧ��

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
        printf("����Ϊ�գ��޷�����\n");
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

// ��������ʵ��
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

// ���ݵ�ַ�������еĽڵ�
TreeNode* findNodeByAddress(TreeNode *root, const char *address) {
    while (root != NULL && strcmp(root->address, address) != 0) {
        if (strcmp(address, root->address) < 0)
            root = root->left;
        else
            root = root->right;
    }
    return root; // �����ҵ��Ľڵ��NULL�����δ�ҵ���
}

// ������������ͬһ��ַ�µİ����������Ƴ��ض�ID�İ���
int removeParcelFromList(Parcels *parcels, int parcelId) {
    if (parcels == NULL || parcels->head == NULL) return 0; // ��������

    Parcel *current = parcels->head, *prev = NULL;

    // ���Ұ���
    while (current != NULL && current->id != parcelId) {
        prev = current;
        current = current->next;
    }

    // û���ҵ���Ӧ�İ���
    if (current == NULL) return 0;

    // �Ƴ��ð���
    if (prev == NULL) { // �Ƴ�����ͷ�ڵ�
        parcels->head = current->next;
    } else {
        prev->next = current->next;
    }

    // �ͷŰ����ڵ��ڴ�
    free(current);

    return 1; // ���سɹ���־
}

// �����������ҵ���С�Ľڵ㣨����Ѱ�Ҽ����ߣ�
TreeNode* findMinNode(TreeNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// ����������ɾ���ڵ㣨�����Ѿ��ҵ���Ҫɾ���Ľڵ㣩
void deleteNode(TreeNode **root, const char *address) {
    if (*root == NULL) return;

    if (strcmp((*root)->address, address) < 0) {
        deleteNode(&(*root)->right, address);
    } else if (strcmp((*root)->address, address) > 0) {
        deleteNode(&(*root)->left, address);
    } else { // �ҵ���Ҫɾ���Ľڵ�
        TreeNode *temp = *root;
        if ((*root)->left == NULL) {
            *root = (*root)->right;
        } else if ((*root)->right == NULL) {
            *root = (*root)->left;
        } else { // �������ӽڵ�����
            // �ҵ�������������ߵĽڵ���Ϊ������
            temp = findMinNode(temp->right);
            strcpy((*root)->address, temp->address);
            (*root)->parcels = temp->parcels;
            *root = (*root)->right; // ��Ϊ�����߿϶�û��������
        }
        free(temp);
    }
}

void removeParcelFromTree(TreeNode **root, int parcelId, const char *address) {
    // ���ȸ��ݵ�ַ�ҵ��ڵ�
    TreeNode *node = findNodeByAddress(*root, address);
    if (node == NULL) return; // �����ַ�Ҳ�����ֱ�ӷ���

    // ���ԴӰ����������Ƴ�����
    if (removeParcelFromList(&node->parcels, parcelId)) {
        // ����õ�ַ�µİ�������Ϊ�գ����Ƴ������ڵ�
        if (node->parcels.head == NULL) {
            deleteNode(root, address);
        }
    }
}

// �����ض�ID�İ���
Parcel* findParcelById(Parcels *parcels, int parcelId) {
    Parcel *current = parcels->head;
    while (current != NULL && current->id != parcelId) {
        current = current->next;
    }
    return current; // �����ҵ��İ�����NULL�����δ�ҵ���
}

// ��Ϻ������Ȱ���ַ���ҽڵ㣬�ٰ�ID���Ұ���
Parcel* findParcel(TreeNode *root, const char *address, int parcelId) {
    TreeNode *node = findNodeByAddress(root, address);
    if (node == NULL) return NULL; // ��ַδ�ҵ�
    return findParcelById(&node->parcels, parcelId); // ���ҵ��Ľڵ��в��Ұ���
}

void destroyTree(TreeNode* root) {
    if (!root) return;
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

void movePendingToQueue(Parcels *parcels, Queue *sortingQueue) {
    // ����һ����ʱָ������������
    Parcel *current = parcels->head, *prev = NULL;
    
    while (current != NULL) {
        if (current->Status == PENDING_SORTING) {
            // ����ǰ�ڵ�������
            current->Status = SORTED;
            enqueue(sortingQueue, *current);
            
            // ����ָ��
            Parcel *temp = current;
            current = current->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

//�ּ��� 
void sortParcelsFromQueue(Queue* sortingQueue, TreeNode** root) {
    while (!isQueueEmpty(sortingQueue)) {
        Parcel parcel = dequeue(sortingQueue);
        insertIntoTree(root, parcel.address, parcel);
    }
}
