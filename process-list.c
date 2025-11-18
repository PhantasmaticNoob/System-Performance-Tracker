#include "process-list.h"

void displayProcessMenu() {  // Renamed from displayMenu
    printf("\n--- PROCESS OPERATIONS ---\n");
    printf("1. Add Process\n2. Remove Process\n3. Search Process\n");
    printf("4. Display All\n5. Back\nChoice: ");
}

ProcessList *createProcessList() {
    ProcessList *list = malloc(sizeof(ProcessList));
    list->head = list->tail = NULL; list->count = 0; list->total_memory = 0;
    return list;
}

Process *createProcess(int pid, const char *name, int priority, int cpu_time, const char *state, int memory) {
    Process *proc = malloc(sizeof(Process));
    if (!proc) return NULL;
    proc->pid = pid; strncpy(proc->name, name, 49); proc->name[49] = '\0';
    proc->priority = priority; proc->cpu_time = cpu_time;
    strncpy(proc->state, state, 19); proc->state[19] = '\0';
    proc->prev = proc->next = NULL; proc->memory = memory;
    return proc;
}

void addProcess(ProcessList *list, Process *proc) {
    if (!proc || searchProcessByPID(list, proc->pid)) return;
    if (!list->head) {
        list->head = list->tail = proc;
    } else {
        list->tail->next = proc; proc->prev = list->tail; list->tail = proc;
    }
    list->count++; list->total_memory += proc->memory;
    printf("Added %s (PID %d, %d bytes)\n", proc->name, proc->pid, proc->memory);
}

void removeProcessByPID(ProcessList *list, int pid) {
    Process *proc = searchProcessByPID(list, pid);
    if (!proc) { printf("PID %d not found.\n", pid); return; }
    list->total_memory -= proc->memory;
    if (proc == list->head && proc == list->tail) {
        list->head = list->tail = NULL;
    } else if (proc == list->head) {
        list->head = proc->next; list->head->prev = NULL;
    } else if (proc == list->tail) {
        list->tail = proc->prev; list->tail->next = NULL;
    } else {
        proc->prev->next = proc->next; proc->next->prev = proc->prev;
    }
    list->count--; printf("Removed %s (PID %d)\n", proc->name, pid);
    free(proc);  // FIXED: No deallocForProcess call here - handled in main
}

void removeProcessByName(ProcessList *list, const char *name) {
    Process *proc = searchProcessByName(list, name);
    if (!proc) { printf("'%s' not found.\n", name); return; }
    list->total_memory -= proc->memory;
    if (proc == list->head && proc == list->tail) {
        list->head = list->tail = NULL;
    } else if (proc == list->head) {
        list->head = proc->next; list->head->prev = NULL;
    } else if (proc == list->tail) {
        list->tail = proc->prev; list->tail->next = NULL;
    } else {
        proc->prev->next = proc->next; proc->next->prev = proc->prev;
    }
    list->count--; printf("Removed %s (PID %d)\n", proc->name, proc->pid);
    free(proc);
}

Process *searchProcessByPID(ProcessList *list, int pid) {
    Process *curr = list->head; while (curr) { if (curr->pid == pid) return curr; curr = curr->next; }
    return NULL;
}

Process *searchProcessByName(ProcessList *list, const char *name) {
    Process *curr = list->head; while (curr) { if (!strcmp(curr->name, name)) return curr; curr = curr->next; }
    return NULL;
}

void displayProcessDetails(Process *proc) {
    printf("\nPID: %d | Name: %s | Priority: %d | CPU: %dms | State: %s | Memory: %d bytes\n",
           proc->pid, proc->name, proc->priority, proc->cpu_time, proc->state, proc->memory);
}

void displayAllProcesses(ProcessList *list) {
    if (!list->count) { printf("No processes.\n"); return; }
    printf("\n=== PROCESSES (%d total, %d bytes) ===\n", list->count, list->total_memory);
    printf("PID | Name       | Pri | CPU | State    | Mem\n");
    printf("----+------------+-----+-----+----------+----\n");
    Process *curr = list->head; while (curr) {
        printf("%3d | %-10s | %3d | %3d | %-8s | %3d\n",
               curr->pid, curr->name, curr->priority, curr->cpu_time, curr->state, curr->memory);
        curr = curr->next;
    }
}

void freeProcessList(ProcessList *list) {
    Process *curr = list->head; while (curr) {
        Process *next = curr->next; free(curr); curr = next;
    }
    free(list);
}
