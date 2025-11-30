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
typedef struct GraphType {
    int n;                            // 정점의 개수
    char vertex_label[MAX_VERTICES];  // 정점의 이름
    int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;
//==================================================
// 그래프 관련 함수들
int is_empty_graph(GraphType* g) { return (g->n == 0); }
int is_full_graph(GraphType* g) { return (g->n >= MAX_VERTICES); }
void init_graph(GraphType* g) {
    // TODO
    int row, col;
    g->n = 0;
    for (row = 0; row < MAX_VERTICES; row++)
        for (col = 0; col < MAX_VERTICES; col++)
            g->adj_mat[row][col] = 0;
}
void insert_vertex(GraphType* g, char v_label) {
    // TODO
    if (is_full_graph(g)) {
        fprintf(stderr, "정점의 개수 초과\n");
        return;
    }
    g->vertex_label[g->n++] = v_label;
}
void insert_edge(GraphType* g, int from, int to) {
    // TODO
    g->adj_mat[from][to] = 1;
    g->adj_mat[to][from] = 1;
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
    visited[v] = 1;        // 시작 정점 방문
    enqueue(q, v);         // 시작 정점을 큐에 추가

    while (!is_empty_queue(q)) {
        int curr = dequeue(q);
        printf(" %c", g->vertex_label[curr]);   // 정점 출력

        // curr 정점의 인접 정점 탐색
        for (int w = 0; w < g->n; w++) {
            if (g->adj_mat[curr][w] && !visited[w]) {
                visited[w] = 1;
                enqueue(q, w);
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