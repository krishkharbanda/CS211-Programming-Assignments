#include "../graphutils.h"

// A program to find a cycle in a directed graph
struct element {
    graphNode_t close;
    struct element* next;
};

struct element* push (
    struct element* stack,
    graphNode_t close
) {
    struct element* new = malloc(sizeof(struct element));
    new->close = close;
    new->next = stack;
    stack = new;
    return stack;
}

char pop (struct element** stack ) {
    if (*stack != NULL) {
        struct element* temp = *stack;
        graphNode_t tempClose = temp->close;
        *stack = temp -> next;
        free(temp);
        return tempClose;
    } else {
        return '\0';
    }
}

// You may use DFS or BFS as needed
bool DFS (size_t graphNodeCount, AdjacencyListNode* adjacencyList, struct element** root, graphNode_t parent, graphNode_t current, int* endLoop, int* visited) {
    if (visited[current] == 0) {
        endLoop[0] = current;
        *root = push(*root, current);
        return true;
    }
    visited[current] = 0;

    AdjacencyListNode* neighbor = adjacencyList[current].next;
    while (neighbor) {
        if (neighbor->graphNode == parent) {
            printf("%ld %ld\n", neighbor->graphNode, current);
            exit(0);
        }
        if (DFS(graphNodeCount, adjacencyList, root, current, neighbor->graphNode, endLoop, visited) == true) {
            if (current == endLoop[0]) {
                while(*root != NULL) {
                    graphNode_t data = pop(root);
                    printf( "%ld ", data);
                }
                exit(0);
            }
            else {
                *root = push(*root, current);
                return true;
            }
        }
    neighbor = neighbor->next;
    }
    visited[current] = 1;
    return false;
}

int main ( int argc, char* argv[] ) {

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList;
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);
    
    bool isCyclic = false;
    for (unsigned source=0; source<graphNodeCount; source++) {
        struct element* root = NULL;
        int* endLoop = malloc(1*sizeof(int));
        int* visited = malloc(graphNodeCount*sizeof(int));
        for (int i = 0; i < graphNodeCount; i++) {
            visited[i] = -1;
        }
        isCyclic = DFS(graphNodeCount, adjacencyList, &root, source, source, endLoop, visited);
        free(root);
        free(endLoop);
        free(visited);
    }

    if (!isCyclic) { 
        printf("DAG\n"); 
    }
    
    freeAdjList ( graphNodeCount, adjacencyList );
    return EXIT_SUCCESS;
}