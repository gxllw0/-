#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "parcelInfo.h"
#include "manageDelivery.h"
#include "util.h"

// 创建图
void createGraph(Graph* graph, int numVertices) {
    graph->vertexCount = numVertices;
    for (int i = 0; i < numVertices; ++i) {
        strcpy(graph->vertices[i].name, "");
        graph->vertices[i].edges = NULL;
        graph->vertices[i].locker = (Locker*)malloc(sizeof(Locker));
    }
}

// 添加边
void addEdge(Graph* graph, int src, int dest, double weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dest = dest;
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
void priorityenqueue(PriorityQueue* pq, int vertex, Parcel *parcel, double priority) {
    PriorityQueueItem* newItem = (PriorityQueueItem*)malloc(sizeof(PriorityQueueItem));
    if (newItem == NULL) {
    	printf("优先队列项分配内存失败\n");
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
            printf("-> %s (%.1f)", graph->vertices[edge->dest].name, edge->weight);
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
    priorityenqueue(&pq, start, NULL, 0);

    while (!isPriorityQueueEmpty(&pq)) {
        PriorityQueueItem* item = prioritydequeue(&pq);
        if (item == NULL) continue; // 检查 dequeue 是否成功

        int u = item->vertex; // 直接使用 vertex 作为索引
        free(item); // 释放优先队列项

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

// 最优配送路径
void displayOptimalDeliveryPath(Graph* graph, int start, int end, Parcel* parcel) {
    double distances[6];
    int previous[6];

    dijkstra(graph, start, end, distances, previous);
    
	parcel->Status = DELIVERED;
	
    // 打印路径
    if (distances[end] == DBL_MAX) {
        printf("无法到达目的地。\n");
        return;
    }

    int path[6], index = 0;
    for (int at = end; at != -1; at = previous[at]) {
        path[index++] = at;
    }
	
	printf("快递（id: %d\t寄件人: %s\t收件人: %s）送达%s\n", parcel->id, parcel->sender, parcel->receiver, parcel->address);
	printf("自动分配到%s地点的快递柜\n", parcel->address);
	printf("取件码为%s\n", parcel->code);
    printf("路径: ");
    for (int i = index - 1; i >= 0; --i) {
        printf("%s", graph->vertices[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
    
//    printf(""); // 打印取件码 
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
	for (int i = 0; i <= 5; i++) {
		graph->vertices[i].locker->head = NULL;
	}
	
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
int addressToIndex(Graph* graph, const char* address) {
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
        int destIdx = addressToIndex(graph, current->address);

        // 如果转换成功，则使用最短路径算法计算最优配送路径并打印
        if (destIdx >= 0) {
        	if (current->Status == SORTED) {
	            char code[7];
				generateCode(code);
				// 更新总快递链表的信息 
				strcpy(current->code, code); 
        		
            	displayOptimalDeliveryPath(graph, 0, destIdx, current);
            	// 更新快递柜的信息 
        		printf("快递柜："); 
				addParcelInfo(graph->vertices[destIdx].locker, current->id, current->sender, current->receiver, current->address, current->code);             	
        		graph->vertices[destIdx].locker->head->Status = DELIVERED;
			}
        } else {
            // 显示地址错误信息并跳过此快递
            printf("地址错误: %s\n", current->address);
        }

        current = current->next; // 移动到下一个快递信息节点
    }
}   

void deliverParcels(Parcels* parcels, Graph* graph, PriorityQueue* pq) {
    if (!parcels || !graph) return;
    // 首先处理所有加急快递
    while (!isPriorityQueueEmpty(pq)) {
		PriorityQueueItem* urgentItem = prioritydequeue(pq);
        if (urgentItem) { 
            int start = 0;
            int end = addressToIndex(graph, urgentItem->parcel->address);
            
            char code[7];
			generateCode(code);
            Parcel* parcel = urgentItem->parcel; 			
			strcpy(parcel->code, code); 

			// 使用最短路径算法计算最优配送路径并打印
            displayOptimalDeliveryPath(graph, start, end, parcel);
            // 更新快递柜中的信息 
			printf("快递柜%s：", parcel->address); 
			addParcelInfo(graph->vertices[end].locker, parcel->id, parcel->sender, parcel->receiver, parcel->address, code);
			graph->vertices[end].locker->head->Status = DELIVERED;            
            printf("加急快递已送达。\n");
            free(urgentItem);
        }
    }
    // 然后处理普通快递
    traverseAndDeliver(parcels, graph);
}

//处理加急快递 
void expediteParcels(int parcelId, Parcels* parcels, TreeNode** root, Graph* graph, PriorityQueue* pq) {
    // 查找快递
    Parcel *parcel = findParcelById(parcels, parcelId);
    if (!parcel) {
        printf("未找到该快递。\n");
        return;
    }

    // 打印出正在加急处理的信息
    printf("正在为ID %d的快递设置加急状态...\n", parcelId);

    // 添加到优先队列（假设优先队列中的元素是 PriorityParcel 类型）
    priorityenqueue(pq, 0, parcel, 0);

    // 从树中移除快递（假设根节点是全局变量或者通过其他方式传递）
    removeParcelFromTree(root, parcelId, parcel->address); // 需要实现此函数

    // 更新状态为加急
    parcel->Status = URGENT;

    // 输出成功信息
    printf("ID %d 的快递已成功设置为加急状态。\n", parcelId);
}

void getParcel(Graph* graph, Parcels* parcels) {
	printf("请输入你要前往的快递柜所在的宿舍楼：");
	char address[5];
	scanf("%s", address);
    if(strcmp(address,"A") < 0 || strcmp(address,"E") > 0 || strlen(address) != 1 ){
        printf("地址输入错误!\n");
        return;
    }
    printf("请输入取件码：");
    char code[7];
    scanf("%s", code);
    // 比较对应的快递柜里的快递 
    int dest = addressToIndex(graph, address);
    Parcel* parcel = graph->vertices[dest].locker->head;
    
    if (parcel == NULL) {
    	printf("快递柜为空\n");
	} else {
		printf("%d\n", parcel->id); 
	}
    
    while (parcel != NULL) {
		if (!strcmp(parcel->code, code)) {
			printf("取件码正确\n");
		} else {
			printf("%s\n%s", parcel->code, code); 
		}
		
		if (parcel->Status == DELIVERED) {
			printf("状态正确\n"); 
		}
		   	
    	if (!strcmp(parcel->code, code) && parcel->Status == DELIVERED) {
	        printf("快递id: %d，寄件人: %s，收件人：%s已成功取出\n",
	               parcel->id, parcel->sender, parcel->receiver);
			// 从快递柜中清除 
			int id = parcel->id;
	        free(parcel);
	        // 将总快递链表中的快递状态更新为已签收
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
	printf("未找到对应快递\n");
} 
