#include "manageSortingAndStorage.h"

 
// 图的边 
typedef struct Edge {
    int destination;
    double weight;
    struct Edge* next;
} Edge;

// 图的顶点
typedef struct Vertex {
    char name[50];
    Edge* edges;
} Vertex;

// 图结构
typedef struct Graph {
    int vertexCount;
    Vertex vertices[6]; // 假设有6个顶点
} Graph;

// 优先队列项
typedef struct PriorityQueueItem {
	Parcel *parcel;	
	int vertex;	
    double priority;
    struct PriorityQueueItem* next;
} PriorityQueueItem;

// 优先队列
typedef struct PriorityQueue {
    PriorityQueueItem* head;
} PriorityQueue;

// 创建图
void createGraph(Graph* graph, int numVertices);

// 添加边
void addEdge(Graph* graph, int src, int dest, double weight);

// 初始化优先队列
void initPriorityQueue(PriorityQueue* pq);
// 检查优先队列是否为空
int isPriorityQueueEmpty(PriorityQueue* pq);

// 向优先队列中添加一项
void priorityenqueue(PriorityQueue* pq, int vertex, Parcel *parcel, double priority);

// 从优先队列中移除并返回最高优先级项
PriorityQueueItem* prioritydequeue(PriorityQueue* pq);

// 展示邻接图
void displayAdjacencyGraph(Graph* graph);

// Dijkstra算法的具体实现，计算最短路径
void dijkstra(Graph* graph, int start, int end, double distances[], int previous[]);

// 最优配送路径
void displayOptimalDeliveryPath(Graph* graph, int start, int end, Parcel* parcel);

//初始化校园快递图 
void initCampusDeliveryGraph(Graph* graph);
//把A~E转换为1~5 
int convertAddressToIndex(Graph* graph, const char* address);
//普通快递的配送 
void traverseAndDeliver(Parcels* parcels, Graph* graph);

//配送快递 
void deliverParcels(Parcels* parcels, Graph* graph, PriorityQueue* pq);

// 查找快递信息并返回指向该快递的指针
Parcel* findParcelById(Parcels* parcels, int id);
//处理加急快递 
void expediteParcels(int parcelId, Parcels* parcels, TreeNode** root, Graph* graph, PriorityQueue* pq);
// 更新配送状态
void updateParcelStatus(Parcels* parcels, int parcelId, const char* status);
