#include "manageDelivery.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>

// ����ͼ
void createGraph(Graph* graph, int numVertices) {
    graph->vertexCount = numVertices;
    for (int i = 0; i < numVertices; ++i) {
        strcpy(graph->vertices[i].name, "");
        graph->vertices[i].edges = NULL;
    }
}

// ��ӱ�
void addEdge(Graph* graph, int src, int dest, double weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = dest;
    newEdge->weight = weight;
    newEdge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = newEdge;
}

// ��ʼ�����ȶ���
void initPriorityQueue(PriorityQueue* pq) {
    pq->head = NULL;
}
 
// ������ȶ����Ƿ�Ϊ��
int isPriorityQueueEmpty(PriorityQueue* pq) {
    return pq->head == NULL;
}

// �����ȶ��������һ��
void priorityenqueue(PriorityQueue* pq, int vertexIndex, double priority) {
    PriorityQueueItem* newItem = (PriorityQueueItem*)malloc(sizeof(PriorityQueueItem));
    if (newItem == NULL) {
    	printf("���ȶ���������ڴ�ʧ��\n");
    	return ;
	}
    newItem->vertexIndex = vertexIndex;
    newItem->priority = priority;
    newItem->next = NULL;

    if (pq->head == NULL || priority < pq->head->priority) {
        newItem->next = pq->head;
        pq->head = newItem;
    } else {
        PriorityQueueItem* current = pq->head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newItem->next = current->next;
        current->next = newItem;
    }
}

// �����ȶ������Ƴ�������������ȼ���
PriorityQueueItem* prioritydequeue(PriorityQueue* pq) {
    if (pq->head == NULL) return NULL;

    PriorityQueueItem* oldHead = pq->head;
    pq->head = pq->head->next;
    return oldHead;
}

// չʾ�ڽ�ͼ
void displayAdjacencyGraph(Graph* graph) {
    printf("�ڽ�ͼ:\n");
    for (int i = 0; i < graph->vertexCount; ++i) {
        printf("%s ", graph->vertices[i].name);
        Edge* edge = graph->vertices[i].edges;
        while (edge != NULL) {
            printf("-> %s (%.1f)", graph->vertices[edge->destination].name, edge->weight);
            edge = edge->next;
        }
        printf("\n");
    }
}

// Dijkstra�㷨�ľ���ʵ�֣��������·��
void dijkstra(Graph* graph, int start, int end, double distances[], int previous[]) {
    double INFINITY = DBL_MAX;
    int visited[6] = {0};
    
    // ��ʼ������ͷ���״̬
    for (int i = 0; i < graph->vertexCount; ++i) {
        distances[i] = INFINITY;
        previous[i] = -1;
    }
    distances[start] = 0;

    PriorityQueue pq;
    initPriorityQueue(&pq);
    priorityenqueue(&pq, start, 0);

    while (pq.head != NULL) {
        PriorityQueueItem* item = prioritydequeue(&pq);
        int u = item->vertexIndex;
        free(item);
        if (visited[u]) continue;
        visited[u] = 1;

        if (u == end) break;

        Edge* edge = graph->vertices[u].edges;
        while (edge != NULL) {
            int v = edge->destination;
            if (!visited[v]) {
                double newDist = distances[u] + edge->weight;
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    previous[v] = u;
                    priorityenqueue(&pq, v, newDist);
                }
            }
            edge = edge->next;
        }
    }
}

// ��������·��
void displayOptimalDeliveryPath(Graph* graph, int start, int end) {
    double distances[6];
    int previous[6];

    dijkstra(graph, start, end, distances, previous);

    // ��ӡ·��
    if (distances[end] == DBL_MAX) {
        printf("�޷�����Ŀ�ĵء�\n");
        return;
    }

    int path[6], index = 0;
    for (int at = end; at != -1; at = previous[at]) {
        path[index++] = at;
    }

    printf("·��: ");
    for (int i = index - 1; i >= 0; --i) {
        printf("%s", graph->vertices[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

//��ʼ��У԰���ͼ 
void initCampusDeliveryGraph(Graph* graph) {
    createGraph(graph, 6);

    strcpy(graph->vertices[0].name, "�����վ");
    strcpy(graph->vertices[1].name, "A");
    strcpy(graph->vertices[2].name, "B");
    strcpy(graph->vertices[3].name, "C");
    strcpy(graph->vertices[4].name, "D");
    strcpy(graph->vertices[5].name, "E");

    addEdge(graph, 0, 1, 5.0); // �����վ -> A
    addEdge(graph, 1, 0, 5.0); // A -> �����վ 
    
    addEdge(graph, 0, 2, 3.0); // �����վ -> B
    addEdge(graph, 2, 0, 3.0); // B -> �����վ
    
    addEdge(graph, 1, 2, 2.0); // A -> B
    addEdge(graph, 2, 1, 2.0); // B -> A
    
    addEdge(graph, 1, 3, 6.0); // A -> C
    addEdge(graph, 3, 1, 6.0); // C -> A
    
    addEdge(graph, 2, 4, 4.0); // B -> D
    addEdge(graph, 4, 2, 4.0); // D -> B
    
    addEdge(graph, 3, 4, 2.0); // C -> D
    addEdge(graph, 4, 3, 2.0); // D -> C
    
    addEdge(graph, 3, 5, 7.0); // C -> E
    addEdge(graph, 5, 3, 7.0); // E -> C 
    
    addEdge(graph, 4, 5, 5.0); // D -> E
    addEdge(graph, 5, 4, 5.0); // E -> D 
}


// ���ݵ�ַ�ַ���ת��Ϊ��������
int convertAddressToIndex(Graph* graph, const char* address) {
    for (int i = 0; i < graph->vertexCount; ++i) {
        if (strcmp(graph->vertices[i].name, address) == 0) {
            return i;
        }
    }
    return -1; // ����һ������ֵ��ʾ����
}

void traverseAndDeliver(Parcels* parcels, Graph* graph) {
    if (!parcels || !parcels->head) return;

    Parcel* current = parcels->head;
    while (current != NULL) {
        int destinationIndex = convertAddressToIndex(graph, current->address);

        // ���ת���ɹ�����ʹ�����·���㷨������������·������ӡ
        if (destinationIndex >= 0) {
            displayOptimalDeliveryPath(graph, 0, destinationIndex); // �������п�ݶ��ӡ������վ������
        } else {
            // ��ʾ��ַ������Ϣ�������˿��
            printf("��ַ����: %s\n", current->address);
        }

        current = current->next; // �ƶ�����һ�������Ϣ�ڵ�
    }
}   

void deliverParcels(Parcels* parcels, Graph* graph, PriorityQueue* pq) {
    if (!parcels || !graph) return;

	if (isPriorityQueueEmpty(pq)) {
		printf("pq is empty11\n");
	} else {
		printf("pq is not empty2\n");
	}
    // ���ȴ������мӼ����
    while (!isPriorityQueueEmpty(pq)) {
    	
    	printf("1111\n");
        
		PriorityQueueItem* urgentItem = prioritydequeue(pq);
        if (urgentItem) {
            // ʹ�����·���㷨������������·������ӡ
            displayOptimalDeliveryPath(graph, 0, urgentItem->vertexIndex);

            printf("�Ӽ�������ʹ\n");
            free(urgentItem);
        }
    }

    // Ȼ������ͨ���
    traverseAndDeliver(parcels, graph);
}

// ���ҿ����Ϣ������ָ��ÿ�ݵ�ָ��
Parcel* findParcelById(Parcels* parcels, int id) {
    if (!parcels || !parcels->head) return NULL;

    Parcel *current = parcels->head;
    while (current != NULL && current->id != id) {
        current = current->next;
    }

    // ����ҵ���ƥ��Ŀ�ݣ��򷵻�ָ������ָ��
    if (current != NULL && current->id == id) {
        return current;
    } else {
        return NULL; // δ�ҵ����
    }
}

//����Ӽ���� 
void expediteParcels(int parcelId, Parcels* parcels, Graph* graph, PriorityQueue* pq) {
    if (!parcels) {
        printf("δ�ҵ�IDΪ %d �Ŀ�ݻ�����Ϊ�ա�\n", parcelId);
        return;
    }

    // ���ҿ����Ϣ
    Parcel* parcel = findParcelById(parcels, parcelId);
    if (parcel) { // �ҵ����
        // �����״̬����Ϊ�Ӽ�
        parcel->Status = URGENT;
//        strcpy(parcel->Status, "�Ӽ�");

        // ���ҵ��Ŀ�ݼ������ȶ��У�����ֻ���Ŀ�ĵ�������
        int destinationIndex = convertAddressToIndex(graph, parcel->address);
        if (destinationIndex >= 0) {
            priorityenqueue(pq, destinationIndex, 0); // �������ȼ�Ϊ0��ʾ������ȼ�
            
            if (isPriorityQueueEmpty(pq)) {
            	printf("YES\n");
			} else {
				printf("NO\n"); 
//				PriorityQueueItem* tmp = prioritydequeue(pq);
//				printf("%d\n", tmp->vertexIndex);
				if (isPriorityQueueEmpty(pq)) {
					printf("YES2\n");
				} else {
					printf("NO2\n");
				}
			}
            
            printf("��� ID: %d �ѱ����Ϊ�Ӽ�������Ӽ����С�\n", parcelId);
        } else {
            printf("��ַ����: %s\n", parcel->address);
        }
    } else {
        printf("δ�ҵ�IDΪ %d �Ŀ�ݡ�\n", parcelId);
    }
}



// ��������״̬
//void updateParcelStatus(Parcels* parcels, int parcelId, const char* status) {
//    Parcel* parcel = findParcelById(parcels, parcelId);
//    if (parcel) {
//        strcpy(parcel->Status, Status);
//        printf("��� ID: %d ��״̬�Ѹ���Ϊ %s��\n", parcelId, status);
//    } else {
//        printf("δ�ҵ�IDΪ %d �Ŀ�ݡ�\n", parcelId);
//    }
//}


