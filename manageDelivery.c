#include "manageDelivery.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>

// 创建图
void createGraph(Graph* graph, int numVertices) {
    graph->vertexCount = numVertices;
    for (int i = 0; i < numVertices; ++i) {
        strcpy(graph->vertices[i].name, "");
        graph->vertices[i].edges = NULL;
    }
}

// 添加边
void addEdge(Graph* graph, int src, int dest, double weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = dest;
    newEdge->weight = weight;
    newEdge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = newEdge;
}

// 初始化优先队列
void initPriorityQueue(PriorityQueue* pq) {
    pq->head = NULL;
}
 
// 检查优先队列是否为空
int isPriorityQueueEmpty(PriorityQueue* pq) {
    return pq->head == NULL;
}

// 向优先队列中添加一项
void priorityenqueue(PriorityQueue* pq, int vertexIndex, double priority) {
    PriorityQueueItem* newItem = (PriorityQueueItem*)malloc(sizeof(PriorityQueueItem));
    if (newItem == NULL) {
    	printf("优先队列项分配内存失败\n");
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

// 从优先队列中移除并返回最高优先级项
PriorityQueueItem* prioritydequeue(PriorityQueue* pq) {
    if (pq->head == NULL) return NULL;

    PriorityQueueItem* oldHead = pq->head;
    pq->head = pq->head->next;
    return oldHead;
}

// 展示邻接图
void displayAdjacencyGraph(Graph* graph) {
    printf("邻接图:\n");
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

// Dijkstra算法的具体实现，计算最短路径
void dijkstra(Graph* graph, int start, int end, double distances[], int previous[]) {
    double INFINITY = DBL_MAX;
    int visited[6] = {0};
    
    // 初始化距离和访问状态
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

// 最优配送路径
void displayOptimalDeliveryPath(Graph* graph, int start, int end) {
    double distances[6];
    int previous[6];

    dijkstra(graph, start, end, distances, previous);

    // 打印路径
    if (distances[end] == DBL_MAX) {
        printf("无法到达目的地。\n");
        return;
    }

    int path[6], index = 0;
    for (int at = end; at != -1; at = previous[at]) {
        path[index++] = at;
    }

    printf("路径: ");
    for (int i = index - 1; i >= 0; --i) {
        printf("%s", graph->vertices[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

//初始化校园快递图 
void initCampusDeliveryGraph(Graph* graph) {
    createGraph(graph, 6);

    strcpy(graph->vertices[0].name, "快递总站");
    strcpy(graph->vertices[1].name, "A");
    strcpy(graph->vertices[2].name, "B");
    strcpy(graph->vertices[3].name, "C");
    strcpy(graph->vertices[4].name, "D");
    strcpy(graph->vertices[5].name, "E");

    addEdge(graph, 0, 1, 5.0); // 快递总站 -> A
    addEdge(graph, 1, 0, 5.0); // A -> 快递总站 
    
    addEdge(graph, 0, 2, 3.0); // 快递总站 -> B
    addEdge(graph, 2, 0, 3.0); // B -> 快递总站
    
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


// 根据地址字符串转换为顶点索引
int convertAddressToIndex(Graph* graph, const char* address) {
    for (int i = 0; i < graph->vertexCount; ++i) {
        if (strcmp(graph->vertices[i].name, address) == 0) {
            return i;
        }
    }
    return -1; // 返回一个特殊值表示错误
}

void traverseAndDeliver(Parcels* parcels, Graph* graph) {
    if (!parcels || !parcels->head) return;

    Parcel* current = parcels->head;
    while (current != NULL) {
        int destinationIndex = convertAddressToIndex(graph, current->address);

        // 如果转换成功，则使用最短路径算法计算最优配送路径并打印
        if (destinationIndex >= 0) {
            displayOptimalDeliveryPath(graph, 0, destinationIndex); // 假设所有快递都从“快递总站”出发
        } else {
            // 显示地址错误信息并跳过此快递
            printf("地址错误: %s\n", current->address);
        }

        current = current->next; // 移动到下一个快递信息节点
    }
}   

void deliverParcels(Parcels* parcels, Graph* graph, PriorityQueue* pq) {
    if (!parcels || !graph) return;

	if (isPriorityQueueEmpty(pq)) {
		printf("pq is empty11\n");
	} else {
		printf("pq is not empty2\n");
	}
    // 首先处理所有加急快递
    while (!isPriorityQueueEmpty(pq)) {
    	
    	printf("1111\n");
        
		PriorityQueueItem* urgentItem = prioritydequeue(pq);
        if (urgentItem) {
            // 使用最短路径算法计算最优配送路径并打印
            displayOptimalDeliveryPath(graph, 0, urgentItem->vertexIndex);

            printf("加急快递已送达。\n");
            free(urgentItem);
        }
    }

    // 然后处理普通快递
    traverseAndDeliver(parcels, graph);
}

// 查找快递信息并返回指向该快递的指针
Parcel* findParcelById(Parcels* parcels, int id) {
    if (!parcels || !parcels->head) return NULL;

    Parcel *current = parcels->head;
    while (current != NULL && current->id != id) {
        current = current->next;
    }

    // 如果找到了匹配的快递，则返回指向它的指针
    if (current != NULL && current->id == id) {
        return current;
    } else {
        return NULL; // 未找到快递
    }
}

//处理加急快递 
void expediteParcels(int parcelId, Parcels* parcels, Graph* graph, PriorityQueue* pq) {
    if (!parcels) {
        printf("未找到ID为 %d 的快递或链表为空。\n", parcelId);
        return;
    }

    // 查找快递信息
    Parcel* parcel = findParcelById(parcels, parcelId);
    if (parcel) { // 找到快递
        // 将快递状态更新为加急
        parcel->Status = URGENT;
//        strcpy(parcel->Status, "加急");

        // 将找到的快递加入优先队列（这里只添加目的地索引）
        int destinationIndex = convertAddressToIndex(graph, parcel->address);
        if (destinationIndex >= 0) {
            priorityenqueue(pq, destinationIndex, 0); // 假设优先级为0表示最高优先级
            
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
            
            printf("快递 ID: %d 已被标记为加急并加入加急队列。\n", parcelId);
        } else {
            printf("地址错误: %s\n", parcel->address);
        }
    } else {
        printf("未找到ID为 %d 的快递。\n", parcelId);
    }
}



// 更新配送状态
//void updateParcelStatus(Parcels* parcels, int parcelId, const char* status) {
//    Parcel* parcel = findParcelById(parcels, parcelId);
//    if (parcel) {
//        strcpy(parcel->Status, Status);
//        printf("快递 ID: %d 的状态已更新为 %s。\n", parcelId, status);
//    } else {
//        printf("未找到ID为 %d 的快递。\n", parcelId);
//    }
//}


