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
}

// ��������ʵ��
TreeNode* createTreeNode(const char* addressPrefix) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(node->addressPrefix, addressPrefix);
    initParcels(&node->parcels);
    node->left = node->right = NULL;
    return node;
}

void insertIntoTree(TreeNode** root, const char* addressPrefix, Parcel parcel) {
    if (!*root) {
        *root = createTreeNode(addressPrefix);
    }

    TreeNode* current = *root;
    while (strcmp(current->addressPrefix, addressPrefix) != 0) {
        int cmp = strcmp(addressPrefix, current->addressPrefix);
        if (cmp < 0 && current->left) {
            current = current->left;
        } else if (cmp > 0 && current->right) {
            current = current->right;
        } else {
            break;
        }
    }

    if (strcmp(current->addressPrefix, addressPrefix) != 0) {
        if (strcmp(addressPrefix, current->addressPrefix) < 0) {
            current->left = createTreeNode(addressPrefix);
            current = current->left;
        } else {
            current->right = createTreeNode(addressPrefix);
            current = current->right;
        }
    }

    addParcelInfo(&current->parcels, parcel.id, parcel.sender, parcel.receiver, parcel.address);
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
