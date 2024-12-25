#ifndef PARCELINFO_H
#define PARCELINFO_H

typedef enum {
    PENDING_SORTING, // 待分拣
    SORTED,          // 已分拣
    DELIVERED,       // 已送达
    SIGNED,			 // 已签收 
    URGENT 			 // 加急 
} ParcelStatus;

// 快递节点 
typedef struct Parcel {
    int id;
    char sender[50];
    char receiver[50];
    char address[50];
    char code[7];
    ParcelStatus Status;
    struct Parcel* next;
} Parcel;

// 快递链表 
typedef struct Parcels {
    Parcel* head;
} Parcels;

// 初始化链表
void initParcels(Parcels* parcels);
// 新添快递（有新增节点） 
void addParcelInfo(Parcels* parcels, int id, const char* sender, const char* receiver, const char* address, const char* code); 
// 将快递加入链表（没有重新分配内存） 
void addParcel(Parcels* parcels, Parcel* parcel);
// 根据快递id更新快递信息 
void updateParcelInfo(Parcels* parcels, int id); 
// 根据快递id删除快递 
void deleteParcelInfo(Parcels* parcels, int id);
// 释放所有快递节点 
void freeParcels(Parcels* parcels);
// 获取快递状态（字符串） 
char* parcelStatusToString(ParcelStatus status);
// 按id查询快递信息 
void queryParcelInfo(Parcels* parcels, int id);
// 查询收件人快递信息 
void queryParcelByReceiver(Parcels* parcels, char receiver[]);
// 批量导入 
void batchImport(Parcels* parcels);
// 批量导出 
void batchExport(Parcels* parcels);
// 多维度排序（按不同标准排序）
void sortParcels(Parcels* parcels, int field);

#endif 
