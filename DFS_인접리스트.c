#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 256
typedef struct ListNode {
    int vertex;
    struct ListNode* link;
} ListNode;
typedef struct GraphType {
    int n;                            // 정점의 개수
    char vertex_label[MAX_VERTICES];  // 정점의 이름
    ListNode* adj_list[MAX_VERTICES];   // 정점 i 와 연결된 연결 리스트의 머리를 가리키는 포인터  
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
        fprintf(stderr, "그래프: 정점의 개수 초과\n");
        return;
    }
    g->vertex_label[g->n++] = v_label;
}
void insert_edge(GraphType* g, int from, int to) {
    // TODO
    //  from → to 연결
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->vertex = to;
    node->link = g->adj_list[from];   // 기존 리스트 앞에 연결
    g->adj_list[from] = node;       // adj_list의 from 인덱스 헤드노드를 새 노드로 교체

    //  to → from 연결 (무방향 그래프이므로 반대 방향도 추가)
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
// DFS 관련 함수들
int visited[MAX_VERTICES];
void init_visited(GraphType* g) {
    for (int i = 0; i < g->n; i++)
        visited[i] = 0;
}
void DFS(GraphType* g, int v) {
    // TODO
    visited[v] = 1;
    printf(" %c", g->vertex_label[v]);
    for (ListNode* w = g->adj_list[v]; w; w = w->link)
        if (!visited[w->vertex])
            DFS(g, w->vertex);
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

    free_graph(g);

    return 0;
}