#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
        printf("����Ϊ��\n");
        exit(EXIT_FAILURE);
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
    if (root == NULL) {
    	printf("root is empty after sorting.\n");
	} else {
    	printf("root is not empty after sorting.\n");		
	}
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

    addParcelInfo(&current->parcels, parcel.id, parcel.sender, parcel.receiver, parcel.address);
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
//    free(current);

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
//        free(temp); ���� 
    }
}

void destroyTree(TreeNode* root) {
    if (!root) return;
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

void manageLockers(LockersList* lockers) {
    int subChoice;
    do {
        printf("\n-----------------------------\n");
        printf("| ��ݹ����               |\n");
        printf("\n-----------------------------\n");
        printf("| 1. �鿴���п�ݹ�״̬    |\n");
        printf("| 2. �ͷſ�ݹ�            |\n");
        printf("| 3. ������һ���˵�        |\n");
        printf("\n-----------------------------\n");
        printf("����������ѡ��: ");
        scanf("%d", &subChoice);
        
        switch (subChoice) {
            case 1:
                printAllLockers(lockers);
                break;
            case 2: {
                int lockerNumber;
                printf("������Ҫ�ͷŵĿ�ݹ���: ");
                if (scanf("%d", &lockerNumber) == 1) {
                    releaseLocker(lockers, lockerNumber);
                    printf("��ݹ� %d ���ͷš�\n", lockerNumber);
                } else {
                    printf("���벻���Ϲ淶��������������\n");
                    while (getchar() != '\n'); // ������뻺����
                }
                break;
            }
            case 3:
                printf("������һ���˵���\n");
                return;
            default:
                printf("��Ч��ѡ�����������롣\n");
        }
    } while (subChoice != 3);
}
// ��ݹ�����ʵ��
void initLockers(LockersList* lockers) {
    lockers->head = lockers->tail = NULL;
}

Locker* allocateLocker(LockersList* lockers, Parcel parcel) {
    Locker* newLocker = (Locker*)malloc(sizeof(Locker));
    if (!newLocker) return NULL;

    srand(time(NULL)); // ��ʼ�����������
    snprintf(newLocker->pickupCode, sizeof(newLocker->pickupCode), "%06d", rand() % 900000 + 100000); // ���������λȡ����

    newLocker->parcel = parcel;
    newLocker->next = NULL;

    if (!lockers->head) {
        lockers->head = lockers->tail = newLocker;
    } else {
        lockers->tail->next = newLocker;
        lockers->tail = newLocker;
    }

    return newLocker;
}

void releaseLocker(LockersList* lockers, int lockerNumber) {
    Locker* current = lockers->head;
    Locker* prev = NULL;

    while (current && current->lockerNumber != lockerNumber) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return;

    if (prev == NULL) {
        lockers->head = current->next;
    } else {
        prev->next = current->next;
    }

    if (current == lockers->tail) {
        lockers->tail = prev;
    }

    free(current);
}

void printAllLockers(LockersList* lockers) {
    Locker* current = lockers->head;
    while (current) {
        printf("��ݹ� %d: ���ID %d, ȡ���� %s\n", current->lockerNumber, current->parcel.id, current->pickupCode);
        current = current->next;
    }
}

void destroyLockers(LockersList* lockers) {
    Locker* current = lockers->head;
    while (current) {
        Locker* next = current->next;
        free(current);
        current = next;
    }
    lockers->head = lockers->tail = NULL;
}
