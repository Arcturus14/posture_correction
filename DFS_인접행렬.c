#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 256

typedef struct GraphType {
    int n;                            // 정점의 개수
    char vertex_label[MAX_VERTICES];  // 정점의 이름
    int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;
//==================================================
// 그래프 관련 함수들
int is_empty(GraphType* g) { return (g->n == 0); }
int is_full(GraphType* g) { return (g->n >= MAX_VERTICES); }
void init_graph(GraphType* g) {
    // TODO
    int row, col;
    g->n = 0;   // vertex 개수 0개
    for (row = 0; row < MAX_VERTICES; row++)
        for (col = 0; col < MAX_VERTICES; col++)
            g->adj_mat[row][col] = 0;
}
void insert_vertex(GraphType* g, char vertex_label) {
    if (is_full(g)) {
        fprintf(stderr, "정점의 개수 초과\n");
        return;
    }
    g->vertex_label[g->n++] = vertex_label;
}

void insert_edge(GraphType* g, int from, int to) {
    g->adj_mat[from][to] = 1;
    g->adj_mat[to][from] = 1;
}

void build_graph(GraphType* g) {
    // 정점의 개수 입력
    int n;
    scanf("%d", &n);

    // 정점의 개수만큼 반복
    char vertex_label;
    int curr_v, to_v, is_connected;
    for (curr_v = 0; curr_v < n; curr_v++) {
        scanf(" %c", &vertex_label);
        insert_vertex(g, vertex_label);

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
// DFS 관련 함수들
int visited[MAX_VERTICES];
void init_visited(GraphType* g) {
    for (int i = 0; i < g->n; i++)
        visited[i] = 0;
}
void DFS(GraphType* g, int v) {
    // TODO
    visited[v] = 1; // vertex v의 방문 표시
    printf(" %c", g->vertex_label[v]);

    for (int w = 0; w < g->n; w++)
        if (g->adj_mat[v][w] && !visited[w])
            DFS(g, w);
}
// DFS 관련 함수들 끝
//==================================================

int main() {
    GraphType* g;
    g = (GraphType*)malloc(sizeof(GraphType));

    init_graph(g);
    build_graph(g);

    init_visited(g);
    printf("DFS:");
    DFS(g, 0);

    free(g);

    return 0;
}