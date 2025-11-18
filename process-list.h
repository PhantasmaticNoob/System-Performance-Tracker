#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Process structure with memory field (as requested)
typedef struct Process {
    int pid;                    // Process ID
    char name[50];              // Process name
    int priority;               // Priority 1-10, 10 being highest
    int cpu_time;               // CPU time in milliseconds
    char state[20];             // State: RUNNING, READY, BLOCKED, TERMINATED
    struct Process *prev;       // Pointer to previous process
    struct Process *next;       // Pointer to next process
    int memory;                 // Allocated memory size (in bytes) - NEW FIELD
} Process;

typedef struct ProcessList {
    Process *head;
    Process *tail;
    int count;
    int total_memory;           // Total memory used by all processes
} ProcessList;

// Function prototypes
ProcessList* createProcessList();
Process* createProcess(int pid, const char *name, int priority, int cpu_time, const char *state, int memory);
void addProcess(ProcessList *list, Process *proc);
void removeProcessByPID(ProcessList *list, int pid);
void removeProcessByName(ProcessList *list, const char *name);
Process* searchProcessByPID(ProcessList *list, int pid);
Process* searchProcessByName(ProcessList *list, const char *name);
void displayProcessDetails(Process *proc);
void displayAllProcesses(ProcessList *list);
void freeProcessList(ProcessList *list);
void displayProcessMenu();  // Renamed for clarity

#endif // PROCESS_LIST_H
