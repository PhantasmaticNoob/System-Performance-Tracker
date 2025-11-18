#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

void swap(float *a, float *b) {
    float temp = *a; *a = *b; *b = temp;
}

void insertMax_float(Heap *h, float val) {
    if (h->size >= MAX) { printf("Max heap full!\n"); return; }
    h->heap[h->size] = val; int i = h->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->heap[i] > h->heap[parent]) { swap(&h->heap[i], &h->heap[parent]); i = parent; }
        else break;
    }
}

void insertMin_float(Heap *h, float val) {
    if (h->size >= MAX) { printf("Min heap full!\n"); return; }
    h->heap[h->size] = val; int i = h->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->heap[i] < h->heap[parent]) { swap(&h->heap[i], &h->heap[parent]); i = parent; }
        else break;
    }
}

float getMax(Heap *h) { return h->size ? h->heap[0] : -1.0f; }
float getMin(Heap *h) { return h->size ? h->heap[0] : -1.0f; }

float getAverage(float arr[], int n) {
    if (n <= 0) return 0.0f; float sum = 0; for (int i = 0; i < n; i++) sum += arr[i]; return sum / n;
}

void checkThreshold(float arr[], int n, float threshold) {
    bool exceeded = false;
    printf("\nThreshold Analysis (%.1f):\n", threshold);
    for (int i = 0; i < n; i++) {
        if (arr[i] > threshold) {
            printf("⚠️  High usage: %.1f > %.1f\n", arr[i], threshold);
            exceeded = true;
        }
    }
    if (!exceeded) printf("All within limits.\n");
}

void displayTopK(Heap h, int k) {
    if (k <= 0 || !h.size) return;
    k = k > h.size ? h.size : k; Heap temp = h;
    printf("\nTop %d CPU Usages:\n", k);
    for (int i = 0; i < k; i++) {
        float val = getMax(&temp); printf("%.1f ", val);
        if (temp.size > 1) {
            temp.heap[0] = temp.heap[--temp.size];
            int idx = 0; while (true) {
                int l = 2*idx+1, r = 2*idx+2, largest = idx;
                if (l < temp.size && temp.heap[l] > temp.heap[largest]) largest = l;
                if (r < temp.size && temp.heap[r] > temp.heap[largest]) largest = r;
                if (largest == idx) break;
                swap(&temp.heap[idx], &temp.heap[largest]); idx = largest;
            }
        } else temp.size = 0;
    } printf("\n");
}

void insertMax(Heap *h, Process proc) {
    insertMax_float(h, (float)proc.cpu_time);
    printf("Added %s (%.0f ms) to max heap\n", proc.name, (float)proc.cpu_time);
}

void insertMin(Heap *h, Process proc) {
    insertMin_float(h, (float)proc.cpu_time);
    printf("Added %s (%.0f ms) to min heap\n", proc.name, (float)proc.cpu_time);
}

void generateCPUReport(ProcessList *list) {
    if (!list->count) { printf("No processes.\n"); return; }
    Heap maxH = {0}, minH = {0}; float usages[list->count]; int i = 0;
    Process *curr = list->head;
    printf("\n=== CPU SCHEDULING REPORT ===\n");
    
    while (curr) {
        insertMax(&maxH, *curr); insertMin(&minH, *curr);
        usages[i++] = (float)curr->cpu_time / 10.0f;  // Convert to %
        curr = curr->next;
    }
    
    printf("Processes: %d | Max CPU: %.1f%% | Min CPU: %.1f%% | Avg: %.1f%%\n",
           list->count, getMax(&maxH), getMin(&minH), getAverage(usages, i));
    
    float thresh; printf("Threshold (%%): "); scanf("%f", &thresh);
    checkThreshold(usages, i, thresh);
    
    int k; printf("Top K: "); scanf("%d", &k); displayTopK(maxH, k);
}
