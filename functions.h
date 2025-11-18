#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>
#include "process-list.h"

#define MAX 100

typedef struct {
    float heap[MAX];
    int size;
} Heap;

// Heap functions for CPU analysis
void insertMax_float(Heap *h, float val);
void insertMin_float(Heap *h, float val);
float getMax(Heap *h);
float getMin(Heap *h);
float getAverage(float arr[], int n);
void checkThreshold(float arr[], int n, float threshold);
void displayTopK(Heap h, int k);
void swap(float *a, float *b);

// Process-based wrappers
void insertMax(Heap *h, Process proc);
void insertMin(Heap *h, Process proc);

// CPU report generation
void generateCPUReport(ProcessList *list);

#endif
