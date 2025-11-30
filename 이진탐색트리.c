#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int TreeElement;
typedef struct TreeNode {
    TreeElement key;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* create_tree_node(TreeElement key) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void delete_tree_node(TreeNode* node) {
    if (node == NULL) return;
    delete_tree_node(node->left);
    delete_tree_node(node->right);
    free(node);
}

typedef struct {
    TreeNode* root;
} BinaryTree;

void init_tree(BinaryTree* tree) { tree->root = NULL; }
void delete_tree(BinaryTree* tree) {
    if (tree->root == NULL) return;
    delete_tree_node(tree->root);
    tree->root = NULL;
}
int is_empty_tree(BinaryTree* tree) { return tree->root == NULL; }

// 이진 탐색 트리에서 특정 값을 찾는 함수
TreeNode* search_key(TreeNode* subroot, TreeElement key) {
    // 탐색 실패
    if (subroot == NULL) return NULL;
    // 서브 루트 노드가 찾는 키와 일치하는 경우
    // TODO 
    if (subroot->key == key) return subroot;

    // 서브 루트 노드가 찾는 키보다 큰 경우 왼쪽 서브트리에서 탐색
    if (subroot->key > key)  return search_key(subroot->left, key);
    // 서브 루트 노드가 찾는 키보다 작은 경우 오른쪽 서브트리에서 탐색
    // TODO
    if (subroot->key < key)  return search_key(subroot->right, key);
}

// 이진 탐색 트리에 키를 삽입하는 함수
// 키를 삽입 후 서브 트리의 루트를 반환
// 재귀적으로 삽입할 위치를 찾아가며 , 삽입 위치(탐색이 실패한 위치) 새로운 노드를 트리에 추가
TreeNode* insert_key(TreeNode* subroot, TreeElement key) {
    // 서브 루트 노드가 NULL인 경우 새로운 노드를 생성하여 반환
    if (subroot == NULL) return create_tree_node(key);

    // 서브 루트 노드가 찾는 키보다 큰 경우 왼쪽 서브트리에 삽입
    if (subroot->key > key)
        subroot->left = insert_key(subroot->left, key);
    // 서브 루트 노드가 찾는 키보다 작은 경우 오른쪽 서브트리에 삽입
    // TODO
    else
        subroot->right = insert_key(subroot->right, key);

    return subroot;
}

// 이진 탐색 트리에서 최소값을 가진 노드를 찾는 함수
TreeNode* min_value_node(TreeNode* subroot) {
    TreeNode* current = subroot;
    // 맨 왼쪽 단말 노드를 찾는다.
    // TODO
    while (current->left != NULL)
        current = current->left;
    return current;
}

// 이진 탐색 트리에서 키를 삭제하는 함수
// 키를 삭제 후 서브 트리의 루트를 반환
TreeNode* delete_key(TreeNode* subroot, TreeElement key) {
    // 키가 없는 경우
    if (subroot == NULL) return subroot;

    // 삭제할 노드를 찾는 과정
    if (subroot->key > key)
        subroot->left = delete_key(subroot->left, key);
    else if (subroot->key < key)
        subroot->right = delete_key(subroot->right, key);

    // 삭제할 노드를 찾은 경우
    else {
        // 1. 자식이 없음(단말노드) -> 바로 삭제
        // 2. 자식이 하나 있음 -> 자식을 부모에 직접 연결
        if (subroot->left == NULL) {
            TreeNode* temp = subroot->right;
            free(subroot);
            return temp;
        }
        if (subroot->right == NULL) {
            TreeNode* temp = subroot->left;
            free(subroot);
            return temp;
        }

        // 3. 자식이 둘 있는 경우 -> 오른쪽 서브트리에서 가장 작은 값을 찾아 
        // 현재노드와 교체하고, 그 노드를 삭제
        TreeNode* temp = min_value_node(subroot->right);
        subroot->key = temp->key;
        subroot->right = delete_key(subroot->right, temp->key);
    }
    return subroot;
}

void inorder(TreeNode* subroot) {
    if (subroot != NULL) {
        inorder(subroot->left);
        printf("[%d] ", subroot->key);
        inorder(subroot->right);
    }
}

int main() {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    init_tree(tree);  // 트리 초기화
    char c[10];
    int T, key;
    scanf("%d", &T);
    for (int i = 0; i < T; i++) {
        scanf("%s", c);
        if (strcmp(c, "insert") == 0) {
            scanf("%d", &key);
            tree->root = insert_key(tree->root, key);
        }
        else if (strcmp(c, "delete") == 0) {
            scanf("%d", &key);
            tree->root = delete_key(tree->root, key);
        }
        else if (strcmp(c, "printTree") == 0) {
            inorder(tree->root);
            printf("\n");
        }
        else if (strcmp(c, "search") == 0) {
            scanf("%d", &key);
            TreeNode* node = search_key(tree->root, key);
            if (node != NULL)
                printf("[%d]\n", node->key);
            else
                printf("fail\n");
        }
    }

    delete_tree(tree);
    return 0;
}
