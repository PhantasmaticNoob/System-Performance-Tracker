#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

#include <stdbool.h>
#include "process-list.h"

#define MAX_MEM 10240  // 10KB total memory

typedef struct NODE {
    bool allocated;
    int address;
    int value;      // Memory block size
    int occupied;   // Total occupied in subtree
    int count;      // Number of allocations
    struct NODE *left;
    struct NODE *right;
    bool lthread;   // Left thread flag
    bool rthread;   // Right thread flag
    Process *owner; // Owning process (if allocated)
} node;

// Function prototypes
void displayMemory(void);
bool allocForProcess(int size, Process *proc);
bool deallocForProcess(Process *proc);
int getAvailableMemory(void);
int getUsedMemory(void);
void free_tree(node *root);  // Declaration for main
int to_allocate(int suggested);
int to_deallocate(void);

// Globals
extern node* root;
extern int ADDRESS_TRACKER;
extern int MEMORY_TRACKER;

#endif
