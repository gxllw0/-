#include "manageSortingAndStorage.h"

 
// ͼ�ı� 
typedef struct Edge {
    int destination;
    double weight;
    struct Edge* next;
} Edge;

// ͼ�Ķ���
typedef struct Vertex {
    char name[50];
    Edge* edges;
} Vertex;

// ͼ�ṹ
typedef struct Graph {
    int vertexCount;
    Vertex vertices[6]; // ������6������
} Graph;

// ���ȶ�����
typedef struct PriorityQueueItem {
	Parcel *parcel;	
	int vertex;	
    double priority;
    struct PriorityQueueItem* next;
} PriorityQueueItem;

// ���ȶ���
typedef struct PriorityQueue {
    PriorityQueueItem* head;
} PriorityQueue;

// ����ͼ
void createGraph(Graph* graph, int numVertices);

// ��ӱ�
void addEdge(Graph* graph, int src, int dest, double weight);

// ��ʼ�����ȶ���
void initPriorityQueue(PriorityQueue* pq);
// ������ȶ����Ƿ�Ϊ��
int isPriorityQueueEmpty(PriorityQueue* pq);

// �����ȶ��������һ��
void priorityenqueue(PriorityQueue* pq, int vertex, Parcel *parcel, double priority);

// �����ȶ������Ƴ�������������ȼ���
PriorityQueueItem* prioritydequeue(PriorityQueue* pq);

// չʾ�ڽ�ͼ
void displayAdjacencyGraph(Graph* graph);

// Dijkstra�㷨�ľ���ʵ�֣��������·��
void dijkstra(Graph* graph, int start, int end, double distances[], int previous[]);

// ��������·��
void displayOptimalDeliveryPath(Graph* graph, int start, int end, Parcel* parcel);

//��ʼ��У԰���ͼ 
void initCampusDeliveryGraph(Graph* graph);
//��A~Eת��Ϊ1~5 
int convertAddressToIndex(Graph* graph, const char* address);
//��ͨ��ݵ����� 
void traverseAndDeliver(Parcels* parcels, Graph* graph);

//���Ϳ�� 
void deliverParcels(Parcels* parcels, Graph* graph, PriorityQueue* pq);

// ���ҿ����Ϣ������ָ��ÿ�ݵ�ָ��
Parcel* findParcelById(Parcels* parcels, int id);
//����Ӽ���� 
void expediteParcels(int parcelId, Parcels* parcels, TreeNode** root, Graph* graph, PriorityQueue* pq);
// ��������״̬
void updateParcelStatus(Parcels* parcels, int parcelId, const char* status);
