#include <string.h>
#include <stdio.h>
#include "statistic.h"

// ��ʼ����ַͳ�ƽṹ����
void initAddressStats(AddressStats* stats, int size) {
    strcpy(stats[0].address, "A");
    strcpy(stats[1].address, "B");
    strcpy(stats[2].address, "C");
    strcpy(stats[3].address, "D");
    strcpy(stats[4].address, "E");
    stats[0].count = 0;
    stats[1].count = 0;
    stats[2].count = 0;
    stats[3].count = 0;
    stats[4].count = 0;
}

// ��ʼ��״̬ͳ�ƽṹ����
void initStatusStats(StatusStats* stats, int size) {
    for (int i = 0; i < size; ++i) { // ��Ϊ��5��״̬
        stats[i].status = (ParcelStatus)i;
        stats[i].count = 0;
    }
}

// ����ַͳ��
void countAddressStats(Parcels* parcels, AddressStats* addressStats, int size) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        for (int i = 0; i < size; ++i) {
            if (strcmp(current->address, addressStats[i].address) == 0) {
                addressStats[i].count++;
                break;
            }
        }
        current = current->next;
    }
}

// ��״̬ͳ��
void countStatusStats(Parcels* parcels, StatusStats* statusStats, int size) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        for (int i = 0; i < size; ++i) { // ��Ϊ��5��״̬
            if (current->Status == statusStats[i].status) {
                statusStats[i].count++;
                break;
            }
        }
        current = current->next;
    }
}

// ��ӡ��ַ������ 
void printAddressTable(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);		
    printf("��ݵ�ַ������:\n");
    printf("+----------------+--------+\n");
    printf("| ��ַ           | ����   |\n");
    printf("+----------------+--------+\n");
    for (int i = 0; i < size; ++i) {
        printf("| %-14s | %6d |\n", addressStats[i].address, addressStats[i].count);
    }
    printf("+----------------+--------+\n");
}

// ��ӡ״̬������ 
void printStatusTable(Parcels* parcels, StatusStats* statusStats, int size) {
	countStatusStats(parcels, statusStats, size);			
    printf("���״̬������:\n");
    printf("+----------------+--------+\n");
    printf("| ״̬           | ����   |\n");
    printf("+----------------+--------+\n");
    for (int i = 0; i < 5; ++i) {
        printf("| %-14s | %6d |\n", parcelStatusToString(statusStats[i].status), statusStats[i].count);
    }
    printf("+----------------+--------+\n");
}

// ��ӡ��ַ��״ͼ
void printAddressHistogram(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);	
    printf("��ַ��״ͼ:\n");
    for (int i = 0; i < size; ++i) {
        printf("%s: ", addressStats[i].address);
        for (int j = 0; j < addressStats[i].count; ++j) {
            printf("#");
        }
        printf(" (%d)\n", addressStats[i].count);
    }
}

// ��ӡ״̬��״ͼ
void printStatusHistogram(Parcels* parcels, StatusStats* statusStats, int size) {
	countStatusStats(parcels, statusStats, size);		
    printf("״̬��״ͼ:\n");
    for (int i = 0; i < size; ++i) {
        printf("%s: ", parcelStatusToString(statusStats[i].status));
        for (int j = 0; j < statusStats[i].count; ++j) {
            printf("#");
        }
        printf(" (%d)\n", statusStats[i].count);
    }
}

// ͳ�Ʒ����߷���
void analyzeStats(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += addressStats[i].count;	
	}
	double average = (double)sum / size;
	for (int i = 0; i < size; i++) {
		if (addressStats[i].count > average) {
			printf("�߷���ʱ����¥%c���Ŀ��������ƽ��ֵ����Ҫ��֤%c���Ŀ���������ʹ�\n", 'A' + i, 'A' + i);
		}
	}
} 

// Ԥ������ 
void predictParcels(Parcels* parcels, StatusStats* statusStats, int size) {
	countStatusStats(parcels, statusStats, size);			
	int parcelUndeliver = 0;
	int parcelDelivered = 0;
	int total = 0;
	for (int i = 0; i < size; i++) {
		ParcelStatus status = statusStats[i].status;
		if (status == DELIVERED || status == SIGNED) {
			parcelDelivered += statusStats[i].count;
			total += statusStats[i].count;
		} else {
			parcelUndeliver += statusStats[i].count;
			total += statusStats[i].count;
		}
	}
	printf("��ǰ����%d������ڿ������\n", parcelDelivered);
	printf("�ۼ�����%d�����������\n", parcelUndeliver);
	printf("Ԥ�ƿ������ÿ�յ�%.2f����ݾ���Ҫ����1�����\n", (double)total / parcelDelivered);
}

// �Ż���Դ���� 
void allocateDelivery(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += addressStats[i].count;	
	}
	double average = (double)sum / size;
	for (int i = 0; i < size; i++) {
		if (addressStats[i].count >= average) {
			printf("����¥%c���Ŀ�������ڵ���ƽ��ֵ���������ɿ��Ա\n", 'A' + i);
		} else {
			printf("����¥%c���Ŀ����С��ƽ��ֵ�����Լ��ɿ��Ա\n", 'A' + i);
		}
	}
}
