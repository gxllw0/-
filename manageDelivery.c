#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "parcelInfo.h"
#include "manageDelivery.h"
#include "util.h"

// ����ͼ
void createGraph(Graph* graph, int numVertices) {
    graph->vertexCount = numVertices;
    for (int i = 0; i < numVertices; ++i) {
        strcpy(graph->vertices[i].name, "");
        graph->vertices[i].edges = NULL;
        graph->vertices[i].locker = (Locker*)malloc(sizeof(Locker));
    }
}

// ��ӱ�
void addEdge(Graph* graph, int src, int dest, double weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dest = dest;
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
void priorityenqueue(PriorityQueue* pq, int vertex, Parcel *parcel, double priority) {
    PriorityQueueItem* newItem = (PriorityQueueItem*)malloc(sizeof(PriorityQueueItem));
    if (newItem == NULL) {
    	printf("���ȶ���������ڴ�ʧ��\n");
    	return ;
	}
	newItem->vertex = vertex;
    newItem->parcel = parcel;
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
            printf("-> %s (%.1f)", graph->vertices[edge->dest].name, edge->weight);
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
    priorityenqueue(&pq, start, NULL, 0);

    while (!isPriorityQueueEmpty(&pq)) {
        PriorityQueueItem* item = prioritydequeue(&pq);
        if (item == NULL) continue; // ��� dequeue �Ƿ�ɹ�

        int u = item->vertex; // ֱ��ʹ�� vertex ��Ϊ����
        free(item); // �ͷ����ȶ�����

        if (visited[u]) continue;
        visited[u] = 1;

        if (u == end) break;

        Edge* edge = graph->vertices[u].edges;
        while (edge != NULL) {
            int v = edge->dest;
            if (!visited[v]) {
                double newDist = distances[u] + edge->weight;
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    previous[v] = u;
                    priorityenqueue(&pq, v, NULL, newDist);
                }
            }
            edge = edge->next;
        }
    }
}

// ��������·��
void displayOptimalDeliveryPath(Graph* graph, int start, int end, Parcel* parcel) {
    double distances[6];
    int previous[6];

    dijkstra(graph, start, end, distances, previous);
    
	parcel->Status = DELIVERED;
	
    // ��ӡ·��
    if (distances[end] == DBL_MAX) {
        printf("�޷�����Ŀ�ĵء�\n");
        return;
    }

    int path[6], index = 0;
    for (int at = end; at != -1; at = previous[at]) {
        path[index++] = at;
    }
	
	printf("��ݣ�id: %d\t�ļ���: %s\t�ռ���: %s���ʹ�%s\n", parcel->id, parcel->sender, parcel->receiver, parcel->address);
	printf("�Զ����䵽%s�ص�Ŀ�ݹ�\n", parcel->address);
	printf("ȡ����Ϊ%s\n", parcel->code);
    printf("·��: ");
    for (int i = index - 1; i >= 0; --i) {
        printf("%s", graph->vertices[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
    
//    printf(""); // ��ӡȡ���� 
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
	for (int i = 0; i <= 5; i++) {
		graph->vertices[i].locker->head = NULL;
	}
	
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
int addressToIndex(Graph* graph, const char* address) {
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
        int destIdx = addressToIndex(graph, current->address);

        // ���ת���ɹ�����ʹ�����·���㷨������������·������ӡ
        if (destIdx >= 0) {
        	if (current->Status == SORTED) {
	            char code[7];
				generateCode(code);
				// �����ܿ���������Ϣ 
				strcpy(current->code, code); 
        		
            	displayOptimalDeliveryPath(graph, 0, destIdx, current);
            	// ���¿�ݹ����Ϣ 
        		printf("��ݹ�"); 
				addParcelInfo(graph->vertices[destIdx].locker, current->id, current->sender, current->receiver, current->address, current->code);             	
        		graph->vertices[destIdx].locker->head->Status = DELIVERED;
			}
        } else {
            // ��ʾ��ַ������Ϣ�������˿��
            printf("��ַ����: %s\n", current->address);
        }

        current = current->next; // �ƶ�����һ�������Ϣ�ڵ�
    }
}   

void deliverParcels(Parcels* parcels, Graph* graph, PriorityQueue* pq) {
    if (!parcels || !graph) return;
    // ���ȴ������мӼ����
    while (!isPriorityQueueEmpty(pq)) {
		PriorityQueueItem* urgentItem = prioritydequeue(pq);
        if (urgentItem) { 
            int start = 0;
            int end = addressToIndex(graph, urgentItem->parcel->address);
            
            char code[7];
			generateCode(code);
            Parcel* parcel = urgentItem->parcel; 			
			strcpy(parcel->code, code); 

			// ʹ�����·���㷨������������·������ӡ
            displayOptimalDeliveryPath(graph, start, end, parcel);
            // ���¿�ݹ��е���Ϣ 
			printf("��ݹ�%s��", parcel->address); 
			addParcelInfo(graph->vertices[end].locker, parcel->id, parcel->sender, parcel->receiver, parcel->address, code);
			graph->vertices[end].locker->head->Status = DELIVERED;            
            printf("�Ӽ�������ʹ\n");
            free(urgentItem);
        }
    }
    // Ȼ������ͨ���
    traverseAndDeliver(parcels, graph);
}

//����Ӽ���� 
void expediteParcels(int parcelId, Parcels* parcels, TreeNode** root, Graph* graph, PriorityQueue* pq) {
    // ���ҿ��
    Parcel *parcel = findParcelById(parcels, parcelId);
    if (!parcel) {
        printf("δ�ҵ��ÿ�ݡ�\n");
        return;
    }

    // ��ӡ�����ڼӼ��������Ϣ
    printf("����ΪID %d�Ŀ�����üӼ�״̬...\n", parcelId);

    // ��ӵ����ȶ��У��������ȶ����е�Ԫ���� PriorityParcel ���ͣ�
    priorityenqueue(pq, 0, parcel, 0);

    // �������Ƴ���ݣ�������ڵ���ȫ�ֱ�������ͨ��������ʽ���ݣ�
    removeParcelFromTree(root, parcelId, parcel->address); // ��Ҫʵ�ִ˺���

    // ����״̬Ϊ�Ӽ�
    parcel->Status = URGENT;

    // ����ɹ���Ϣ
    printf("ID %d �Ŀ���ѳɹ�����Ϊ�Ӽ�״̬��\n", parcelId);
}

void getParcel(Graph* graph, Parcels* parcels) {
	printf("��������Ҫǰ���Ŀ�ݹ����ڵ�����¥��");
	char address[5];
	scanf("%s", address);
    if(strcmp(address,"A") < 0 || strcmp(address,"E") > 0 || strlen(address) != 1 ){
        printf("��ַ�������!\n");
        return;
    }
    printf("������ȡ���룺");
    char code[7];
    scanf("%s", code);
    // �Ƚ϶�Ӧ�Ŀ�ݹ���Ŀ�� 
    int dest = addressToIndex(graph, address);
    Parcel* parcel = graph->vertices[dest].locker->head;
    
    if (parcel == NULL) {
    	printf("��ݹ�Ϊ��\n");
	} else {
		printf("%d\n", parcel->id); 
	}
    
    while (parcel != NULL) {
		if (!strcmp(parcel->code, code)) {
			printf("ȡ������ȷ\n");
		} else {
			printf("%s\n%s", parcel->code, code); 
		}
		
		if (parcel->Status == DELIVERED) {
			printf("״̬��ȷ\n"); 
		}
		   	
    	if (!strcmp(parcel->code, code) && parcel->Status == DELIVERED) {
	        printf("���id: %d���ļ���: %s���ռ��ˣ�%s�ѳɹ�ȡ��\n",
	               parcel->id, parcel->sender, parcel->receiver);
			// �ӿ�ݹ������ 
			int id = parcel->id;
	        free(parcel);
	        // ���ܿ�������еĿ��״̬����Ϊ��ǩ��
			parcel = parcels->head;
			while (parcel != NULL) {
				if (id == parcel->id) {
					parcel->Status = SIGNED;
					break;
				}
				parcel = parcel->next;
			} 
	        return ;
		}
		parcel = parcel->next;
	}
	printf("δ�ҵ���Ӧ���\n");
} 
