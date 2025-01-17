#include <stdlib.h>
#include <stdio.h>
#include "system.h"

SystemContext* initializeSystem() {
    srand(time(NULL));
		
    SystemContext* context = (SystemContext*)malloc(sizeof(SystemContext));
    if (!context) {
        printf("初始化分配内存失败。\n");
        return NULL;
    }

    context->parcels = (Parcels*)malloc(sizeof(Parcels));
    if (!context->parcels) {
        printf("快递链表分配内存失败。\n");
        free(context);
        return NULL;
    }
    context->parcels->head = NULL;

    context->ht = createTable();
    if (!context->ht) {
        perror("存储用户信息的哈希表分配内存失败。\n");
        free(context->parcels);
        free(context);
        return NULL;
    }

	// 初始化队列 
    initQueue(&context->sortingQueue);
	
	// 初始化树 
    context->root = NULL;

	// 初始化图 
    initCampusDeliveryGraph(&context->graph);
	
	// 初始化优先队列 
    initPriorityQueue(&context->pq);
    
    // 初始化地址统计数组
	initAddressStats(context->addressStats, NUM_ADDRESSES);
	
	// 初始化状态统计数组
	initStatusStats(context->statusStats, NUM_STATUS); 
    
    printf("成功初始化系统。\n");

    return context;
}

void cleanupSystem(SystemContext *context) {
    if (!context) return;

    freeParcels(context->parcels);
    freeTable(context->ht);
	destroyTree(context->root);
	
    free(context);
}
