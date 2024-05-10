#include "../graphutils.h" // header for functions to load and free adjacencyList
#include "../queue/queue.h" // header for queue

// A program to solve a maze that may contain cycles using BFS

int main ( int argc, char* argv[] ) {

    // First, read the query file to get the source and target nodes in the maze
    FILE* query_file = fopen(argv[2], "r");
    size_t source, target;
    fscanf(query_file, "%ld %ld", &source, &target);
    fclose(query_file);

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList = NULL;
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);

    // USE A QUEUE TO PERFORM BFS
    Queue queue = { .front=NULL, .back=NULL };

    // An array that keeps track of who is the parent node of each graph node we visit
    graphNode_t* parents = calloc( graphNodeCount, sizeof(graphNode_t) );
    for (size_t i=0; i<graphNodeCount; i++) {
        parents[i] = -1; // -1 indicates that a node is not yet visited
    }

    parents[source] = source;
    enqueue(&queue, (void*)source);

    while ( queue.front ) {
        size_t current = (size_t)dequeue(&queue);

        if (current == target) {
            break;
        }

        AdjacencyListNode* neighbor = adjacencyList[current].next;
        while (neighbor) {
            if (parents[neighbor->graphNode] == -1) {
                parents[neighbor->graphNode] = current;
                enqueue(&queue, (void*)neighbor->graphNode);
            }
            neighbor = neighbor->next;
        }
    }

    // Now that we've found the target graph node, use the parent array to print maze solution
    if (parents[target] != -1) {
        size_t node = target;
        while (node != source) {
            size_t parent = parents[node];
            printf("%ld %ld\n", node, parent);
            node = parent;
        }
    }

    while ( queue.front ) {
        dequeue(&queue);
    }
    free (parents);
    freeAdjList ( graphNodeCount, adjacencyList );

    return EXIT_SUCCESS;
}
