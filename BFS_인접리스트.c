#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 20
typedef struct QueueType {
    int front;
    int rear;
    int data[MAX_QUEUE_SIZE];
} QueueType;
//==================================================
// 큐 관련 함수들
void init_queue(QueueType* q) {
    q->rear = -1;
    q->front = -1;
}
int is_empty_queue(QueueType* q) { return q->front == q->rear; }
int is_full_queue(QueueType* q) { return q->rear == MAX_QUEUE_SIZE - 1; }
void enqueue(QueueType* q, int item) {
    if (is_full_queue(q)) return;

    q->rear++;
    q->data[q->rear] = item;
}
int dequeue(QueueType* q) {
    if (is_empty_queue(q)) return INT32_MIN;

    q->front++;
    return q->data[q->front];
}
// 큐 관련 함수들 끝
//==================================================

#define MAX_VERTICES 256
typedef struct ListNode {
    int vertex;
    struct ListNode* link;
} ListNode;
typedef struct GraphType {
    int n;                            // 정점의 개수
    char vertex_label[MAX_VERTICES];  // 정점의 이름
    ListNode* adj_list[MAX_VERTICES];
} GraphType;
//==================================================
// 그래프 관련 함수들
int is_empty(GraphType* g) { return (g->n == 0); }
int is_full(GraphType* g) { return (g->n >= MAX_VERTICES); }
void init_graph(GraphType* g) {
    // TODO
    g->n = 0;
    for (int i = 0; i < MAX_VERTICES; i++)
        g->adj_list[i] = NULL;
}
void free_graph(GraphType* g) {
    // TODO
    for (int i = 0; i < g->n; i++) {
        ListNode* p = g->adj_list[i];
        while (p != NULL) {
            ListNode* temp = p;
            p = p->link;
            free(temp);
        }
        g->adj_list[i] = NULL;
    }
}
void insert_vertex(GraphType* g, char v_label) {
    // TODO
    if (is_full(g)) {
        fprintf(stderr, "정점의 개수 초과\n");
        return;
    }
    g->vertex_label[g->n++] = v_label;
}
void insert_edge(GraphType* g, int from, int to) {
    // TODO
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->vertex = to;
    node->link = g->adj_list[from];
    g->adj_list[from] = node;

    node = (ListNode*)malloc(sizeof(ListNode));
    node->vertex = from;
    node->link = g->adj_list[to];
    g->adj_list[to] = node;
}
void build_graph(GraphType* g) {
    // 정점의 개수 입력
    int n;
    scanf("%d", &n);

    // 정점의 개수만큼 반복
    char v_label;
    int curr_v, to_v, is_connected;
    for (curr_v = 0; curr_v < n; curr_v++) {
        scanf(" %c", &v_label);
        insert_vertex(g, v_label);

        // 각 정점에 대한 연결 정보 입력
        for (to_v = 0; to_v < n; to_v++) {
            scanf(" %d", &is_connected);

            if (is_connected) insert_edge(g, curr_v, to_v);
        }
    }
}
// 그래프 관련 함수들 끝
//==================================================

//==================================================
// BFS 관련 함수들
int visited[MAX_VERTICES];
void init_visited(GraphType* g) {
    for (int i = 0; i < g->n; i++)
        visited[i] = 0;
}
void BFS(GraphType* g, QueueType* q, int v) {
    // TODO
    visited[v] = 1;
    enqueue(q, v);

    while (!is_empty_queue(q)) {
        int curr = dequeue(q);
        printf(" %c", g->vertex_label[curr]);
        // 인접리스트를 순회하면서 방문하지 않았으면 enqueue하고 방문 표시
        for (ListNode* node = g->adj_list[curr]; node; node = node->link) {
            if (!visited[node->vertex]) {
                enqueue(q, node->vertex);
                visited[node->vertex] = 1;
            }
        }
    }
}
// BFS 관련 함수들 끝
//==================================================

int main() {
    QueueType* q;
    GraphType* g;
    q = (QueueType*)malloc(sizeof(QueueType));
    g = (GraphType*)malloc(sizeof(GraphType));

    init_queue(q);
    init_graph(g);

    build_graph(g);

    printf("BFS:");
    init_visited(g);
    BFS(g, q, 0);

    free(g);

    return 0;
}