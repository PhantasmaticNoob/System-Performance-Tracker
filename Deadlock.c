#include "Deadlock.h"

void initGraph(graph *g, int vertices) {
    if (!g) return;
    g->vertices = vertices; memset(g->adj, 0, sizeof(g->adj));
    for (int i = 0; i < vertices; i++) sprintf(g->labels[i], "P%d", i);
    printf("Graph init: %d nodes\n", vertices);
}

void addVertex(graph *g) {
    if (g->vertices >= MAX1) { printf("Max vertices.\n"); return; }
    int n = g->vertices; sprintf(g->labels[n], "P%d", n);
    for (int i = 0; i <= n; i++) g->adj[i][n] = g->adj[n][i] = 0;
    g->vertices++; printf("Added %s\n", g->labels[n]);
}

bool addEdge(graph *g, int u, int v) {
    if (!g || u < 0 || v < 0 || u >= g->vertices || v >= g->vertices) return false;
    g->adj[u][v] = 1;
    printf("Edge %s->%s\n", g->labels[u], g->labels[v]);
    return true;
}

void displayGraph(graph *g) {
    if (!g || g->vertices <= 0) return;
    printf("\nAdjacency Matrix:\n"); for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) printf("%d ", g->adj[i][j]);
        printf(" (%s)\n", g->labels[i]);
    }
    printf("Edges: "); for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) if (g->adj[i][j]) 
            printf("%s->%s ", g->labels[i], g->labels[j]);
    } printf("\n");
}

bool detectCycle(graph *g) {
    bool visited[MAX1] = {0};
    int parent[MAX1];
    int stack[MAX1], top;

    if (!g) return false;
    memset(parent, -1, sizeof(parent));

    for (int start = 0; start < g->vertices; start++) {
        if (!visited[start]) {
            top = -1; stack[++top] = start;
            while (top >= 0) {
                int curr = stack[top];
                if (!visited[curr]) {
                    visited[curr] = true; bool found = false;
                    for (int adj = 0; adj < g->vertices; adj++) if (g->adj[curr][adj]) {
                        if (!visited[adj]) { stack[++top] = adj; parent[adj] = curr; found = true; break; }
                        else if (parent[curr] != adj) { printf("Cycle: %s->%s\n", g->labels[curr], g->labels[adj]); return true; }
                    }
                    if (!found) top--;
                } else top--;
            }
        }
    }
    return false;
}

bool detectProcessDeadlock(ProcessList *list) {  
    if (list->count < 2) return false;
    graph g; initGraph(&g, list->count);
    Process *curr = list->head; int idx = 0;
    while (curr && curr->next) {
        if (curr->priority > curr->next->priority && strcmp(curr->state, "RUNNING") == 0) {
            addEdge(&g, idx, idx + 1);
            if (strcmp(curr->next->state, "BLOCKED") == 0) addEdge(&g, idx + 1, idx);
        }
        curr = curr->next; idx++;
    }
    displayGraph(&g);
    return detectCycle(&g);
}
