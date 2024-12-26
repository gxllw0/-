#ifndef STATISTIC_H
#define STATISTIC_H

#include "parcelInfo.h"

#define NUM_ADDRESSES 5
#define NUM_STATUS 5 

// ��ַͳ�ƽṹ
typedef struct {
    char address[5]; // ��ַ����
    int count;        // �������
} AddressStats;

// ״̬ͳ�ƽṹ
typedef struct {
    ParcelStatus status; // ���״̬
    int count;           // ״̬����
} StatusStats;

// ��ʼ����ַͳ������ 
void initAddressStats(AddressStats* stats, int size); 

// ��ʼ��״̬ͳ�ƽṹ����
void initStatusStats(StatusStats* stats, int size);

// ��ַͳ�ƺ���
void countAddressStats(Parcels* parcels, AddressStats* addressStats, int size);

// ״̬ͳ�ƺ���
void countStatusStats(Parcels* parcels, StatusStats* statusStats, int size);

// ��ӡ��ַ������ 
void printAddressTable(Parcels* parcels, AddressStats* addressStats, int size);

// ��ӡ״̬������ 
void printStatusTable(Parcels* parcels, StatusStats* statusStats, int size); 

// ������ӡ��ַ��״ͼ����
void printAddressHistogram(Parcels* parcels, AddressStats* addressStats, int size);

// ������ӡ״̬��״ͼ����
void printStatusHistogram(Parcels* parcels, StatusStats* statusStats, int size);

// Ԥ������ 
void predictParcels(Parcels* parcels, StatusStats* statusStats, int size);

// ͳ�Ʒ����߷���
void analyzeStats(Parcels* parcels, AddressStats* addressStats, int addrSize);

// �Ż���Դ���� 
void allocateDelivery(Parcels* parcels, AddressStats* addressStats, int size);

#endif // STATISTIC_H
