#ifndef DEADLOCK_H
#define DEADLOCK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process-list.h"  // FIXED: Added for ProcessList

#define MAX1 10  // Max processes + resources

typedef struct {
    int adj[MAX1][MAX1];       // Adjacency matrix
    int vertices;              // Number of nodes
    char labels[MAX1][50];     // Node labels (P1, R1, etc.)
} graph;

// Graph functions
void initGraph(graph *g, int vertices);
bool addEdge(graph *g, int u, int v);
void addVertex(graph *g);
void displayGraph(graph *g);
bool detectCycle(graph *g);

// Process deadlock analysis
bool detectProcessDeadlock(ProcessList *list);

#endif
