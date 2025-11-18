#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Deadlock.h"

void initGraph(graph *g, int vertices) {
    if (g == NULL) {
        printf("Graph pointer is NULL!\n");
        return;
    }

    g->vertices = vertices;

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->adj[i][j] = 0;
        }
    }
    printf("Graph initialized with %d vertices\n",vertices);
}

void addVertex(graph*g){
    if(g->vertices >=MAX1){
        printf("Maximum number of vertices reached");
        return;
    }

    int n = g->vertices;
    for (int i = 0; i <= n; i++) {
        g->adj[i][n] = 0;
        g->adj[n][i] = 0;
    }
    (g->vertices)++;
    printf(" Vertex %d added successfully\n", n);
}

bool addEdge(graph *g, int u, int v) {
    if (g == NULL) {
        printf("Graph not initialized!\n");
        return false;
    }

    if(g->vertices <=0){
        printf("Graph is not initialized, Please create a graph first.");
        return false;
    }

    if (u < 0 || v < 0 || u >= g->vertices || v >= g->vertices) {
        printf("Invalid edge (%d, %d)\n", u, v);
        return false;
    }

    g->adj[u][v] = 1;
    printf("Edge added between vertex %d and %d\n", u, v);
    return true;
}

void displayGraph(graph *g) {
    if (g == NULL) {
        printf("Graph not initialized!\n");
        return;
    }
    if(g->vertices <=0){
        printf("Graph is not initialized, Please create a graph first.");
        return;
    }
     printf("\nAdjacency Matrix (%d vertices):\n", g->vertices);
     for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) {
            printf("%3d", g->adj[i][j]);
        }
        printf("\n");
    }
    
    // Simple edge display
    printf("\nEdges: ");
    bool firstEdge = true;
    for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) {
            if (g->adj[i][j] == 1) {
                if (!firstEdge) printf(", ");
                printf("%d->%d", i, j);
                firstEdge = false;
            }
        }
    }
    if (firstEdge) printf("None");
    printf("\n");
}

bool detectCycle(graph *g) {
    if (g == NULL) return false;

    int n = g->vertices;
    bool visited[MAX1] = {false}; //explored
    int parent[MAX1];    //discovered by dfs
    int stack[MAX1],top;

    for (int i = 0; i < n; i++) parent[i] = -1;

    for (int start = 0; start < n; start++) {
        if (!visited[start]) {
            top = -1;
            stack[++top] = start;

            while (top >= 0) {
                int curr = stack[top]; 

                if (!visited[curr]) {
                    visited[curr] = true;
                }
                bool found = false;
                for (int i = 0; i < n; i++) {
                    if (g->adj[curr][i] == 1) {
                        if (!visited[i]) {
                            stack[++top] = i;
                            found=true;
                            parent[i] = curr; //discovered by dfs->add to parent
                            break;
                            
                        } else if (parent[curr] != i) {
                            printf("\nCycle detected between %d and %d\n", curr, i);

                            // simple inline backtrack printing
                            printf("\nCYCLE DETECTED! Potential deadlock found!\n");
                            printf("   Cycle: %d", i);
                            int temp = curr;
                            while (temp != i && temp != -1) {
                                printf(" -> %d", temp);
                                temp = parent[temp]; //printed em backwards
                            }
                            printf(" -> %d (cycle complete)\n", i); // complete loop
                            return true;
                        }
                    }
                    if(!found){ top--; }
                }
            }
        }
    }
    return false;
}
