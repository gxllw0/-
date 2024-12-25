#include <stdlib.h>
#include <stdio.h>
#include "system.h"

SystemContext* initializeSystem() {
    srand(time(NULL));
		
    SystemContext* context = (SystemContext*)malloc(sizeof(SystemContext));
    if (!context) {
        printf("��ʼ�������ڴ�ʧ�ܡ�\n");
        return NULL;
    }

    context->parcels = (Parcels*)malloc(sizeof(Parcels));
    if (!context->parcels) {
        printf("�����������ڴ�ʧ�ܡ�\n");
        free(context);
        return NULL;
    }
    context->parcels->head = NULL;

    context->ht = createTable();
    if (!context->ht) {
        perror("�洢�û���Ϣ�Ĺ�ϣ������ڴ�ʧ�ܡ�\n");
        free(context->parcels);
        free(context);
        return NULL;
    }

	//��ʼ������ 
    initQueue(&context->sortingQueue);
	
	//��ʼ���� 
    context->root = NULL;

	//��ʼ��ͼ 
    initCampusDeliveryGraph(&context->graph);
	
	//��ʼ�����ȶ��� 
    initPriorityQueue(&context->pq);
    
    printf("�ɹ���ʼ��ϵͳ��\n");

    return context;
}

void cleanupSystem(SystemContext *context) {
    if (!context) return;

    freeParcels(context->parcels);
    freeTable(context->ht);
	destroyTree(context->root);
	
    free(context);
}
