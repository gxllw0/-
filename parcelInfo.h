#ifndef PARCELINFO_H
#define PARCELINFO_H

typedef enum {
    PENDING_SORTING, // ���ּ�
    SORTED,          // �ѷּ�
    DELIVERED,       // ���ʹ�
    SIGNED,			 // ��ǩ�� 
    URGENT 			 // �Ӽ� 
} ParcelStatus;

typedef struct Parcel {
    int id;
    char sender[50];
    char receiver[50];
    char address[100];
    ParcelStatus Status; // ������״̬�ֶ�
    struct Parcel* next;
} Parcel;

typedef struct Parcels {
    Parcel* head;
} Parcels;

// ��ʼ������
void initParcels(Parcels* parcels);
// ���ӿ�� 
void addParcelInfo(Parcels *parcels, int id, char* sender, char* receiver, char* address); 
// ���ݿ��id���¿����Ϣ 
void updateParcelInfo(Parcels* parcels, int id); 
// ���ݿ��idɾ����� 
void deleteParcelInfo(Parcels* parcels, int id);
// �ͷ����п�ݽڵ� 
void freeParcels(Parcels* parcels);
// ��ȡ���״̬���ַ����� 
char* parcelStatusToString(ParcelStatus status);
// ��id��ѯ�����Ϣ 
void queryParcelInfo(Parcels* parcels, int id);
// ��ѯ�ռ��˿����Ϣ 
void queryParcelByReceiver(Parcels* parcels, char receiver[]);
// �������� 
void batchImport(Parcels* parcels);
// �������� 
void batchExport(Parcels* parcels);
// ��ά�����򣨰���ͬ��׼����
void sortParcels(Parcels* parcels, int field);

#endif 