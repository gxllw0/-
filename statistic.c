#include <string.h>
#include <stdio.h>
#include "statistic.h"

// 初始化地址统计结构数组
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

// 初始化状态统计结构数组
void initStatusStats(StatusStats* stats, int size) {
    for (int i = 0; i < size; ++i) { // 因为有5种状态
        stats[i].status = (ParcelStatus)i;
        stats[i].count = 0;
    }
}

// 按地址统计
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

// 按状态统计
void countStatusStats(Parcels* parcels, StatusStats* statusStats, int size) {
    Parcel* current = parcels->head;
    while (current != NULL) {
        for (int i = 0; i < size; ++i) { // 因为有5种状态
            if (current->Status == statusStats[i].status) {
                statusStats[i].count++;
                break;
            }
        }
        current = current->next;
    }
}

// 打印地址数量表 
void printAddressTable(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);		
    printf("快递地址数量表:\n");
    printf("+----------------+--------+\n");
    printf("| 地址           | 数量   |\n");
    printf("+----------------+--------+\n");
    for (int i = 0; i < size; ++i) {
        printf("| %-14s | %6d |\n", addressStats[i].address, addressStats[i].count);
    }
    printf("+----------------+--------+\n");
}

// 打印状态数量表 
void printStatusTable(Parcels* parcels, StatusStats* statusStats, int size) {
	countStatusStats(parcels, statusStats, size);			
    printf("快递状态数量表:\n");
    printf("+----------------+--------+\n");
    printf("| 状态           | 数量   |\n");
    printf("+----------------+--------+\n");
    for (int i = 0; i < 5; ++i) {
        printf("| %-14s | %6d |\n", parcelStatusToString(statusStats[i].status), statusStats[i].count);
    }
    printf("+----------------+--------+\n");
}

// 打印地址柱状图
void printAddressHistogram(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);	
    printf("地址柱状图:\n");
    for (int i = 0; i < size; ++i) {
        printf("%s: ", addressStats[i].address);
        for (int j = 0; j < addressStats[i].count; ++j) {
            printf("#");
        }
        printf(" (%d)\n", addressStats[i].count);
    }
}

// 打印状态柱状图
void printStatusHistogram(Parcels* parcels, StatusStats* statusStats, int size) {
	countStatusStats(parcels, statusStats, size);		
    printf("状态柱状图:\n");
    for (int i = 0; i < size; ++i) {
        printf("%s: ", parcelStatusToString(statusStats[i].status));
        for (int j = 0; j < statusStats[i].count; ++j) {
            printf("#");
        }
        printf(" (%d)\n", statusStats[i].count);
    }
}

// 统计分析高峰期
void analyzeStats(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += addressStats[i].count;	
	}
	double average = (double)sum / size;
	for (int i = 0; i < size; i++) {
		if (addressStats[i].count > average) {
			printf("高峰期时宿舍楼%c区的快递量大于平均值，需要保证%c区的快递能正常送达\n", 'A' + i, 'A' + i);
		}
	}
} 

// 预测快递量 
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
	printf("当前共有%d件快递在快递中心\n", parcelDelivered);
	printf("累计已有%d件快递已派送\n", parcelUndeliver);
	printf("预计快递中心每收到%.2f件快递就需要派送1件快递\n", (double)total / parcelDelivered);
}

// 优化资源配置 
void allocateDelivery(Parcels* parcels, AddressStats* addressStats, int size) {
	countAddressStats(parcels, addressStats, size);
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += addressStats[i].count;	
	}
	double average = (double)sum / size;
	for (int i = 0; i < size; i++) {
		if (addressStats[i].count >= average) {
			printf("宿舍楼%c区的快递量大于等于平均值，可以增派快递员\n", 'A' + i);
		} else {
			printf("宿舍楼%c区的快递量小于平均值，可以减派快递员\n", 'A' + i);
		}
	}
}
