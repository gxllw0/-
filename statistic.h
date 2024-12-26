#ifndef STATISTIC_H
#define STATISTIC_H

#include "parcelInfo.h"

#define NUM_ADDRESSES 5
#define NUM_STATUS 5 

// 地址统计结构
typedef struct {
    char address[5]; // 地址名称
    int count;        // 快递数量
} AddressStats;

// 状态统计结构
typedef struct {
    ParcelStatus status; // 快递状态
    int count;           // 状态数量
} StatusStats;

// 初始化地址统计数组 
void initAddressStats(AddressStats* stats, int size); 

// 初始化状态统计结构数组
void initStatusStats(StatusStats* stats, int size);

// 地址统计函数
void countAddressStats(Parcels* parcels, AddressStats* addressStats, int size);

// 状态统计函数
void countStatusStats(Parcels* parcels, StatusStats* statusStats, int size);

// 打印地址数量表 
void printAddressTable(Parcels* parcels, AddressStats* addressStats, int size);

// 打印状态数量表 
void printStatusTable(Parcels* parcels, StatusStats* statusStats, int size); 

// 声明打印地址柱状图函数
void printAddressHistogram(Parcels* parcels, AddressStats* addressStats, int size);

// 声明打印状态柱状图函数
void printStatusHistogram(Parcels* parcels, StatusStats* statusStats, int size);

// 预测快递量 
void predictParcels(Parcels* parcels, StatusStats* statusStats, int size);

// 统计分析高峰期
void analyzeStats(Parcels* parcels, AddressStats* addressStats, int addrSize);

// 优化资源配置 
void allocateDelivery(Parcels* parcels, AddressStats* addressStats, int size);

#endif // STATISTIC_H
