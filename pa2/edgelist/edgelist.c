#include "../graphutils.h" // header for functions to load and free adjacencyList

// A program to print the edge list of a graph given the adjacency matrix
int main ( int argc, char* argv[] ) {
    // FIRST, READ THE ADJACENCY MATRIX FILE
    AdjacencyListNode* adjacencyList = NULL;
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);

    // NEXT, TRAVERSE THE ADJACENCY LIST AND PRINT EACH EDGE, REPRESENTED AS A PAIR OF NODES
    for (size_t source = 0; source < graphNodeCount; source++) {
        AdjacencyListNode* dest = adjacencyList[source].next;

        while (dest) {
            if (dest->graphNode > source) {
                printf("%zu %zu\n", source, dest->graphNode);
            }
            dest = dest->next;
        }
    }

    // NOW, BE SURE TO FREE THE ADJACENCY LIST
    freeAdjList(graphNodeCount, adjacencyList);

    return EXIT_SUCCESS;
}