#include <stdio.h>
#include "parcelInfo.h"

// 初始化链表
void initParcels(Parcels* parcels) {
    parcels->head = NULL;
}

// 添加快递信息
void addParcelInfo(Parcels *parcels, int id, char* sender, char* receiver, char* address) {
	Parcel *current = parcels->head;
    while (current != NULL && current->id != id) {
        current = current->next;
    }
    if (current != NULL) {
        printf("该id已存在!\n");
        return;
    }

    Parcel *node = (Parcel*)malloc(sizeof(Parcel));
    if (!node) {
        printf("内存分配失败。\n");
        return;
    }
    
    node->id = id;
    strcpy(node->sender, sender);
    strcpy(node->receiver, receiver);
    strcpy(node->address, address);
    node->Status = PENDING_SORTING; // 初始化状态为待分拣    
    if (parcels->head == NULL) {
    	node->next = NULL;
    	parcels->head = node;
	} else {
		node->next = parcels->head;
		parcels->head = node;
	}
    printf("快递信息已添加。\n");
}

// 修改快递信息
void updateParcelInfo(Parcels* parcels, int id) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        if (current->id == id) {
            printf("请输入新的寄件人: ");
            scanf("%s", current->sender);
            printf("请输入新的收件人: ");
            scanf("%s", current->receiver);
            printf("请输入新的地址: ");
            scanf("%s", current->address);

            // 添加状态修改选项
            printf("请选择新的状态 (0:待分拣, 1:已分拣, 2:运输中, 3:已送达): ");
            int statusChoice;
            scanf("%d", &statusChoice);
            current->Status = (ParcelStatus)statusChoice;

            printf("快递信息已更新。\n");
            return;
        }
        current = current->next;
    }
    printf("未找到该快递信息。\n");
}

// 删除快递信息
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
            printf("快递信息已删除。\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("未找到该快递信息。\n");
}

// 按id查询快递信息
void queryParcelInfo(Parcels* parcels, int id) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        if (current->id == id) {
            printf("ID: %d\n", current->id);
            printf("寄件人: %s\n", current->sender);
            printf("收件人: %s\n", current->receiver);
            printf("地址: %s\n", current->address);
            printf("状态：%s\n", parcelStatusToString(current->Status));
            return;
        }
        current = current->next;
    }
    printf("未找到该快递信息。\n");
}

// 查询收件人快递信息 
void queryParcelByReceiver(Parcels* parcels, char receiver[]) {
    Parcel* current = parcels->head;
    int cnt = 0;
    while (current != NULL) {
        if (!strcmp(current->receiver,receiver)) {
            printf("ID: %d\n", current->id);
            printf("寄件人: %s\n", current->sender);
            printf("收件人: %s\n", current->receiver);
            printf("地址: %s\n", current->address);
            printf("状态: %s\n", parcelStatusToString(current->Status));
            cnt++;
        }
        current = current->next;
    }
    if(cnt == 0) {
        printf("您没有快递。\n");
	}
    else {
        printf("您一共有%d件快递。\n", cnt);
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
        case PENDING_SORTING: return "待分拣";
        case SORTED:          return "已分拣";
        case DELIVERED:       return "已送达";
        case SIGNED:		  return "已签收"; 
        case URGENT:		  return "加急";
        default:              return "未知状态";
    }
}

typedef int (*CompareFunc)(const Parcel*, const Parcel*);

// 比较函数：按ID排序
int compareById(const Parcel* a, const Parcel* b) {
    return (a->id > b->id) - (a->id < b->id);
}

// 比较函数：按Sender排序
int compareBySender(const Parcel* a, const Parcel* b) {
    return strcmp(a->sender, b->sender);
}

// 比较函数：按Receiver排序
int compareByReceiver(const Parcel* a, const Parcel* b) {
    return strcmp(a->receiver, b->receiver);
}

// 比较函数：按Address排序
int compareByAddress(const Parcel* a, const Parcel* b) {
    return strcmp(a->address, b->address);
}

// 比较函数：按Status排序
int compareByStatus(const Parcel* a, const Parcel* b) {
    return strcmp(parcelStatusToString(a->Status), parcelStatusToString(b->Status));
}

// 归并排序辅助函数：合并两个有序链表
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

// 归并排序辅助函数：分割链表
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

// 归并排序递归函数
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

// 归并排序入口函数
void mergeSortParcels(Parcels* parcels, CompareFunc compare) {
    if (parcels->head == NULL || parcels->head->next == NULL) return;

    // 执行归并排序
    mergeSortRecursive(&(parcels->head), compare);

    printf("快递信息已排序。\n");
    printf("\n排序后的快递信息如下：\n");
    printf("------------------------------------------------------------\n");
    printf("| ID | 寄件人     | 收件人     | 地址                   | 状态   |\n");
    printf("------------------------------------------------------------\n");

    for (Parcel* current = parcels->head; current != NULL; current = current->next) {
        printf("| %-2d | %-10s | %-10s | %-25s | %-6s |\n",
               current->id, current->sender, current->receiver,
               current->address, parcelStatusToString(current->Status));
    }
    printf("------------------------------------------------------------\n");


}

// 多维度排序（按不同标准排序）
void sortParcels(Parcels* parcels, int field) {
    if (parcels->head == NULL || parcels->head->next == NULL) return;

    // 根据字段选择比较函数
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
            printf("未知的排序字段。\n");
            return;
    }

    // 使用归并排序对链表进行排序
    mergeSortParcels(parcels, compare);
}

//导入文件功能 
void batchImport(Parcels* parcels){
    FILE* file = fopen("parcels.txt", "r");
    if (!file) {
        printf("无法打开文件。\n");
        return;
    }
    int id;
    char sender[50], receiver[50], address[100], status[20];
    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%s\n", &id, sender, receiver, address, status) != EOF) {
        addParcelInfo(parcels, id, sender, receiver, address);
    }
    fclose(file);
    printf("快递信息已从 parcels.txt 导入。\n");
}

//导出文件功能 
void batchExport(Parcels* parcels) {
    FILE* file = fopen("parcels.txt", "w+");
    if (!file) {
        printf("无法打开文件。\n");
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
    printf("快递信息已导出到 parcels.txt。\n");
}
