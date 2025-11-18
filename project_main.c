#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "process-list.h"
#include "functions.h"
#include "memoryManagement.h"
#include "deadlock.h"

// Forward declaration for memory cleanup
void free_tree(node *root);

int main() {
    ProcessList *processList = createProcessList();
    node *memoryRoot = NULL;
    root = memoryRoot;  // Global memory root
    bool running = true;
    int nextPid = 1;

    printf("\n=== FULL OS PROCESS SIMULATOR ===\n");
    printf("Create processes → Allocate memory → Manage & Analyze\n\n");

    while (running) {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Create New Process\n");
        printf("2. Display All Processes\n");
        printf("3. Search Process\n");
        printf("4. Remove Process\n");
        printf("5. CPU Scheduling Analysis\n");
        printf("6. Deadlock Detection\n");
        printf("7. Memory Status\n");
        printf("8. Exit\n");
        printf("Choice: ");

        int choice;
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 8) {
            printf("Invalid choice. Please enter 1-8.\n");
            while (getchar() != '\n'); continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1: {  // Create Process
                printf("\n--- CREATE PROCESS ---\n");
                int pid = nextPid++;
                char name[50], state[20];
                int priority, cpu_time, memory_size;

                printf("Auto PID: %d\n", pid);
                printf("Name: "); scanf(" %[^\n]", name);
                printf("Priority (1-10): "); scanf("%d", &priority);
                if (priority < 1 || priority > 10) priority = 5;
                printf("CPU Time (ms): "); scanf("%d", &cpu_time);
                if (cpu_time < 1) cpu_time = 100;
                printf("State (RUNNING/READY/BLOCKED/TERMINATED): "); scanf(" %[^\n]", state);
                
                // Memory size = CPU time by default
                memory_size = cpu_time;
                printf("Memory size (default: %d): ", memory_size);
                int temp; if (scanf("%d", &temp) == 1 && temp > 0) memory_size = temp;

                if (memory_size > getAvailableMemory()) {
                    printf("Not enough memory (%d available). Using max available.\n", getAvailableMemory());
                    memory_size = getAvailableMemory();
                }

                Process *newProc = createProcess(pid, name, priority, cpu_time, state, memory_size);
                if (newProc && allocForProcess(memory_size, newProc)) {
                    addProcess(processList, newProc);
                    printf("Process '%s' (PID %d) created with %d bytes allocated.\n", 
                           name, pid, memory_size);
                } else {
                    if (newProc) free(newProc);
                    printf("Failed to create process - memory allocation error.\n");
                }
                break;
            }

            case 2:  // Display All
                if (processList->count == 0) {
                    printf("\nNo processes created yet.\n");
                } else {
                    displayAllProcesses(processList);
                }
                break;

            case 3: {  // Search
                printf("\n--- SEARCH PROCESS ---\n");
                printf("1. By PID  2. By Name\nChoice: ");
                int sub; scanf("%d", &sub); while (getchar() != '\n');
                
                if (sub == 1) {
                    int pid; printf("PID: "); scanf("%d", &pid);
                    Process *found = searchProcessByPID(processList, pid);
                    if (found) displayProcessDetails(found);
                    else printf("Process not found.\n");
                } else if (sub == 2) {
                    char name[50]; printf("Name: "); scanf(" %[^\n]", name);
                    Process *found = searchProcessByName(processList, name);
                    if (found) displayProcessDetails(found);
                    else printf("Process not found.\n");
                }
                break;
            }

            case 4: {  // Remove Process
                printf("\n--- REMOVE PROCESS ---\n");
                printf("1. By PID  2. By Name\nChoice: ");
                int sub; scanf("%d", &sub); while (getchar() != '\n');
                
                if (sub == 1) {
                    int pid; printf("PID: "); scanf("%d", &pid);
                    Process *proc = searchProcessByPID(processList, pid);
                    if (proc) {
                        if (deallocForProcess(proc)) {
                            removeProcessByPID(processList, pid);
                            printf("Process removed and memory freed.\n");
                        } else {
                            printf("Memory deallocation failed.\n");
                        }
                    } else {
                        printf("Process not found.\n");
                    }
                } else if (sub == 2) {
                    char name[50]; printf("Name: "); scanf(" %[^\n]", name);
                    Process *proc = searchProcessByName(processList, name);
                    if (proc) {
                        if (deallocForProcess(proc)) {
                            removeProcessByName(processList, name);
                            printf("Process removed and memory freed.\n");
                        } else {
                            printf("Memory deallocation failed.\n");
                        }
                    } else {
                        printf("Process not found.\n");
                    }
                }
                break;
            }

            case 5:  // CPU Analysis
                if (processList->count == 0) {
                    printf("No processes for analysis.\n");
                } else {
                    generateCPUReport(processList);
                }
                break;

            case 6:  // Deadlock Detection
                if (processList->count < 2) {
                    printf("Need at least 2 processes for deadlock detection.\n");
                } else {
                    printf("\n--- DEADLOCK ANALYSIS ---\n");
                    if (detectProcessDeadlock(processList)) {
                        printf("DEADLOCK DETECTED! System is UNSAFE.\n");
                        printf("Circular wait condition exists between processes.\n");
                    } else {
                        printf("No deadlock. System is SAFE.\n");
                    }
                }
                break;

            case 7:  // Memory Status
                printf("\n--- MEMORY STATUS ---\n");
                printf("Total Capacity: %d bytes\n", MAX_MEM);
                printf("Process Memory: %d bytes (%d processes)\n", processList->total_memory, processList->count);
                printf("System Used: %d bytes\n", getUsedMemory());
                printf("Available: %d bytes\n", getAvailableMemory());
                displayMemory();
                break;

            case 8:  // Exit
                printf("\n--- CLEANING UP ---\n");
                freeProcessList(processList);
                if (root) free_tree(root);  // FIXED: Now declared
                printf("System shutdown complete.\n");
                running = false;
                break;
        }
    }
    return 0;
}
