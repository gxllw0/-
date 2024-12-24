#include <stdio.h>
#include "parcelInfo.h"

// ��ʼ������
void initParcels(Parcels* parcels) {
    parcels->head = NULL;
}

// ��ӿ����Ϣ
void addParcelInfo(Parcels *parcels, int id, char* sender, char* receiver, char* address) {
	Parcel *current = parcels->head;
    while (current != NULL && current->id != id) {
        current = current->next;
    }
    if (current != NULL) {
        printf("��id�Ѵ���!\n");
        return;
    }

    Parcel *node = (Parcel*)malloc(sizeof(Parcel));
    if (!node) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    
    node->id = id;
    strcpy(node->sender, sender);
    strcpy(node->receiver, receiver);
    strcpy(node->address, address);
    node->Status = PENDING_SORTING; // ��ʼ��״̬Ϊ���ּ�    
    if (parcels->head == NULL) {
    	node->next = NULL;
    	parcels->head = node;
	} else {
		node->next = parcels->head;
		parcels->head = node;
	}
    printf("�����Ϣ����ӡ�\n");
}

// �޸Ŀ����Ϣ
void updateParcelInfo(Parcels* parcels, int id) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        if (current->id == id) {
            printf("�������µļļ���: ");
            scanf("%s", current->sender);
            printf("�������µ��ռ���: ");
            scanf("%s", current->receiver);
            printf("�������µĵ�ַ: ");
            scanf("%s", current->address);

            // ���״̬�޸�ѡ��
            printf("��ѡ���µ�״̬ (0:���ּ�, 1:�ѷּ�, 2:������, 3:���ʹ�): ");
            int statusChoice;
            scanf("%d", &statusChoice);
            current->Status = (ParcelStatus)statusChoice;

            printf("�����Ϣ�Ѹ��¡�\n");
            return;
        }
        current = current->next;
    }
    printf("δ�ҵ��ÿ����Ϣ��\n");
}

// ɾ�������Ϣ
void deleteParcelInfo(Parcels* parcels, int id) {
    Parcel* current = parcels->head;
    Parcel* previous = NULL;
    while (current != NULL) {
        if (current->id == id) {
            if (previous == NULL) {
                parcels->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("�����Ϣ��ɾ����\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("δ�ҵ��ÿ����Ϣ��\n");
}

// ��id��ѯ�����Ϣ
void queryParcelInfo(Parcels* parcels, int id) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        if (current->id == id) {
            printf("ID: %d\n", current->id);
            printf("�ļ���: %s\n", current->sender);
            printf("�ռ���: %s\n", current->receiver);
            printf("��ַ: %s\n", current->address);
            printf("״̬��%s\n", parcelStatusToString(current->Status));
            return;
        }
        current = current->next;
    }
    printf("δ�ҵ��ÿ����Ϣ��\n");
}

// ��ѯ�ռ��˿����Ϣ 
void queryParcelByReceiver(Parcels* parcels, char receiver[]) {
    Parcel* current = parcels->head;
    int cnt = 0;
    while (current != NULL) {
        if (!strcmp(current->receiver,receiver)) {
            printf("ID: %d\n", current->id);
            printf("�ļ���: %s\n", current->sender);
            printf("�ռ���: %s\n", current->receiver);
            printf("��ַ: %s\n", current->address);
            printf("״̬: %s\n", parcelStatusToString(current->Status));
            cnt++;
        }
        current = current->next;
    }
    if(cnt == 0) {
        printf("��û�п�ݡ�\n");
	}
    else {
        printf("��һ����%d����ݡ�\n", cnt);
	}
}

void freeParcels(Parcels* parcels){
	if (parcels == NULL || parcels->head == NULL) {
		return ;
	} 
    Parcel* pre = parcels->head;
    Parcel* p = parcels->head->next;
    while(p != NULL)
    {
        free(pre);
        pre = p;
        p = pre->next;
    }
    free(pre); 
}

char* parcelStatusToString(ParcelStatus status) {
    switch (status) {
        case PENDING_SORTING: return "���ּ�";
        case SORTED:          return "�ѷּ�";
        case DELIVERED:       return "���ʹ�";
        case SIGNED:		  return "��ǩ��"; 
        case URGENT:		  return "�Ӽ�";
        default:              return "δ֪״̬";
    }
}

typedef int (*CompareFunc)(const Parcel*, const Parcel*);

// �ȽϺ�������ID����
int compareById(const Parcel* a, const Parcel* b) {
    return (a->id > b->id) - (a->id < b->id);
}

// �ȽϺ�������Sender����
int compareBySender(const Parcel* a, const Parcel* b) {
    return strcmp(a->sender, b->sender);
}

// �ȽϺ�������Receiver����
int compareByReceiver(const Parcel* a, const Parcel* b) {
    return strcmp(a->receiver, b->receiver);
}

// �ȽϺ�������Address����
int compareByAddress(const Parcel* a, const Parcel* b) {
    return strcmp(a->address, b->address);
}

// �ȽϺ�������Status����
int compareByStatus(const Parcel* a, const Parcel* b) {
    return strcmp(parcelStatusToString(a->Status), parcelStatusToString(b->Status));
}

// �鲢�������������ϲ�������������
Parcel* merge(Parcel* left, Parcel* right, CompareFunc compare) {
    if (!left) return right;
    if (!right) return left;

    Parcel* result = NULL;
    if (compare(left, right) <= 0) {
        result = left;
        result->next = merge(left->next, right, compare);
    } else {
        result = right;
        result->next = merge(left, right->next, compare);
    }
    return result;
}

// �鲢�������������ָ�����
void splitList(Parcel* source, Parcel** frontRef, Parcel** backRef) {
    Parcel* fast;
    Parcel* slow;
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
    } else {
        slow = source;
        fast = source->next;
        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        *frontRef = source;
        *backRef = slow->next;
        slow->next = NULL;
    }
}

// �鲢����ݹ麯��
void mergeSortRecursive(Parcel** headRef, CompareFunc compare) {
    Parcel* head = *headRef;
    Parcel* a;
    Parcel* b;

    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    splitList(head, &a, &b);

    mergeSortRecursive(&a, compare);
    mergeSortRecursive(&b, compare);

    *headRef = merge(a, b, compare);
}

// �鲢������ں���
void mergeSortParcels(Parcels* parcels, CompareFunc compare) {
    if (parcels->head == NULL || parcels->head->next == NULL) return;

    // ִ�й鲢����
    mergeSortRecursive(&(parcels->head), compare);

    printf("�����Ϣ������\n");
    printf("\n�����Ŀ����Ϣ���£�\n");
    printf("------------------------------------------------------------\n");
    printf("| ID | �ļ���     | �ռ���     | ��ַ                   | ״̬   |\n");
    printf("------------------------------------------------------------\n");

    for (Parcel* current = parcels->head; current != NULL; current = current->next) {
        printf("| %-2d | %-10s | %-10s | %-25s | %-6s |\n",
               current->id, current->sender, current->receiver,
               current->address, parcelStatusToString(current->Status));
    }
    printf("------------------------------------------------------------\n");


}

// ��ά�����򣨰���ͬ��׼����
void sortParcels(Parcels* parcels, int field) {
    if (parcels->head == NULL || parcels->head->next == NULL) return;

    // �����ֶ�ѡ��ȽϺ���
    CompareFunc compare;
    switch (field) {
        case 0: // ID
            compare = compareById;
            break;
        case 1: // Sender
            compare = compareBySender;
            break;
        case 2: // Receiver
            compare = compareByReceiver;
            break;
        case 3: // Address
            compare = compareByAddress;
            break;
        case 4: // Status
            compare = compareByStatus;
            break;
        default:
            printf("δ֪�������ֶΡ�\n");
            return;
    }

    // ʹ�ù鲢����������������
    mergeSortParcels(parcels, compare);
}

//�����ļ����� 
void batchImport(Parcels* parcels){
    FILE* file = fopen("parcels.txt", "r");
    if (!file) {
        printf("�޷����ļ���\n");
        return;
    }
    int id;
    char sender[50], receiver[50], address[100], status[20];
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%s\n", &id, sender, receiver, address, status) != EOF) {
        addParcelInfo(parcels, id, sender, receiver, address);
    }
    fclose(file);
    printf("�����Ϣ�Ѵ� parcels.txt ���롣\n");
}

//�����ļ����� 
void batchExport(Parcels* parcels) {
    FILE* file = fopen("parcels.txt", "w+");
    if (!file) {
        printf("�޷����ļ���\n");
        return;
    }

    Parcel* current = parcels->head;
    while (current != NULL) {
		fprintf(file, "%d,%s,%s,%s,%s\n", 
                current->id, 
                current->sender, 
                current->receiver, 
                current->address, 
                parcelStatusToString(current->Status));
        current = current->next;
    }
    fclose(file);
    printf("�����Ϣ�ѵ����� parcels.txt��\n");
}
