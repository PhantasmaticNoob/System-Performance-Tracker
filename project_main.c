/* gcc project_main_final.c process-list.c memoryManagement.c functions.c Deadlock.c */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> 
#include "Deadlock.h"
#include "functions.h"
#include "process-list.h"
#include "memoryManagement.h"


extern int ADDRESS_TRACKER;
extern int MEMORY_TRACKER;
extern int MAX_MEM;

int main(){
    bool keep_going = true;
    while (keep_going)
    {
         printf("\n");
         printf("\n=========================================================\n");
        printf("what functionality would you like to view?:\n1. Memory Management\n2. Process List\n\
3.CPU Management\n4. Deadlock detection\n5. Quit -");
        
        int inpt;
            if (feof(stdin)) {
                clearerr(stdin);
    }
            int check1 = scanf("%d", &inpt);
            if(check1 !=1){
                int ch;
                while (( ch = getchar()) != '\n' && ch != EOF);
                printf("invalid input\n");
            }

        switch (inpt)
            {
            case 1:{
                
                        bool flag = true;
                        while(flag){
                            printf("\n");
                            printf("\n");
                            printf("choose:\n1. Allocate\n2. Deallocate\n\
3.Display\n4. Quit (Memory Management) -");
                            int n;
                            if (feof(stdin)) {
                                clearerr(stdin);
                    }
                            int check1 = scanf("%d", &n);
                            if(check1 !=1){
                                int ch;
                                while (( ch = getchar()) != '\n' && ch != EOF);
                                printf("invalid input\n");
                            }
                        
                            switch (n)
                            {
                            case 1:
                                printf("%d", MEMORY_TRACKER);
                                printf("\nmemory available: %d\n", (MAX_MEM-MEMORY_TRACKER )>=0? MAX_MEM-MEMORY_TRACKER: 0);
                                alloc();
                                break;
                            case 2:
                                printf("currently occupied memory:%d\n" ,MEMORY_TRACKER>0? MEMORY_TRACKER: 0);
                                dealloc(0);
                                break;
                            case 3:
                                display();
                                break;
                            case 4:
                                flag = false;
                                break;
                            
                            default:
                                printf("invalid choice\n");
                                break;
                            }
                            }
                        
                    
                    break;
                        }
            case 2:
                        {
                        ProcessList *plist = createProcessList();
                        int choice, subChoice, pid, priority, cpu_time;
                        char name[50], state[20];
                        bool cont = true;

                        while (cont) {
                            displayMenu();
                            scanf("%d", &choice);
                            getchar(); // consume newline

                            switch (choice) {
                                case 1:
                                    printf("Enter PID: "); scanf("%d", &pid);
                                    printf("Enter Name: "); scanf(" %[^\n]", name);
                                    printf("Enter Priority (1-10): "); scanf("%d", &priority);
                                    printf("Enter CPU Time (ms): "); scanf("%d", &cpu_time);
                                    printf("Enter State (RUNNING/READY/BLOCKED/TERMINATED): "); scanf(" %[^\n]", state);
                                    addProcess(plist, pid, name, priority, cpu_time, state);
                                    break;

                                case 2:
                                    printf("\nDelete Process:\n");
                                    printf("1. By PID\n");
                                    printf("2. By Name\n");
                                    printf("Enter your choice: ");
                                    scanf("%d", &subChoice);
                                    getchar();

                                    if (subChoice == 1) {
                                        printf("Enter PID to delete: ");
                                        scanf("%d", &pid);
                                        removeProcessByPID(plist, pid);
                                    } else if (subChoice == 2) {
                                        printf("Enter Name to delete: ");
                                        scanf(" %[^\n]", name);
                                        removeProcessByName(plist, name);
                                    } else {
                                        printf("Invalid delete option.\n");
                                    }
                                    break;

                                case 3:
                                    printf("\nSearch Process:\n");
                                    printf("1. By PID\n");
                                    printf("2. By Name\n");
                                    printf("Enter your choice: ");
                                    scanf("%d", &subChoice);
                                    getchar();

                                    if (subChoice == 1) {
                                        printf("Enter PID to search: ");
                                        scanf("%d", &pid);
                                        Process *found = searchProcessByPID(plist, pid);
                                        if (found) displayProcessDetails(found);
                                        else printf("Process not found.\n");
                                    } else if (subChoice == 2) {
                                        printf("Enter Name to search: ");
                                        scanf(" %[^\n]", name);
                                        Process *found = searchProcessByName(plist, name);
                                        if (found) displayProcessDetails(found);
                                        else printf("Process not found.\n");
                                    } else {
                                        printf("Invalid search option.\n");
                                    }
                                    break;

                                case 4:
                                    displayAllProcesses(plist);
                                    break;

                                case 5:
                                    freeProcessList(plist);
                                    printf("Exiting...\n");
                                    cont = false;
                                    break;

                                default:
                                    printf("Invalid choice. Try again.\n");
                            }
                        }

                        break;
                    }


            case 3: 
                    {
                        int n , k;
                        float threshold;

                        Heap maxHeap = {.size = 0};
                        Heap minHeap = {.size = 0};
                        while(1)
                        {
                            printf("Enter number of CPU usage samples: ");
                            int result = scanf("%d", &n);

                            if(result !=1)
                            {
                                printf("Invalid input! Please enter a positive integer.\n");
                                while (getchar() != '\n');  // clear input buffer
                                continue;
                            }

                            int ch = getchar();
                            if (ch != '\n' && ch != ' ')
                            {
                                while (ch != '\n' && ch != EOF) ch = getchar();
                                printf("Invalid input! Please enter a whole number (no decimals).\n");
                                continue;
                            }
                            if (n <= 0)
                            {
                                printf("Number of samples must be greater than 0.\n");
                                continue;
                            }
                            break;
                        }

                        float arr[n];

                        printf("\nEnter CPU usage values in %% (0-100):\n");
                        for (int i = 0; i < n; i++)
                        {
                            float value;
                            while(1)
                            {
                                printf("Sample %d: ", i + 1);
                                if (scanf("%f", &value) != 1)
                                {
                                    printf("Invalid input! Please enter a numeric value.\n");
                                    while (getchar() != '\n'); // clear buffer
                                    continue;
                                }

                                if (value < 0 || value > 100)
                                {
                                    printf("Invalid input! CPU usage must be between 0 and 100.\n");
                                }

                                else
                                {
                                    arr[i] = value;
                                    insertMax(&maxHeap , value);
                                    insertMin(&minHeap , value);
                                    break;
                                }
                            }
                        }
                        
                        printf("\nData recorded successfully!\n");
                        printf("\n----- CPU USAGE REPORT -----\n");
                        printf("Highest CPU usage: %.2f%%\n", getMax(&maxHeap));
                        printf("Lowest CPU usage : %.2f%%\n", getMin(&minHeap));
                        printf("Average CPU usage: %.2f%%\n", getAverage(arr, n));

                        printf("\nEnter CPU usage threshold (0-100): ");
                        while(scanf("%f", &threshold)!=1 || threshold < 0 || threshold > 100 )
                        {
                            printf("Invalid threshold! Must be a number between 0 and 100.\n");
                            while (getchar() != '\n'); // clear buffer
                            printf("Enter CPU usage threshold again: ");
                        }

                        printf("\nChecking for spikes beyond %.2f%%...\n", threshold);
                        checkThreshold(arr, n, threshold);
                        

                        printf("\nEnter how many top CPU spikes to display: ");
                        while (scanf("%d", &k) != 1)
                        {
                        printf("Invalid input! Please enter a positive integer.\n");
                        while (getchar() != '\n');
                        printf("Enter how many top CPU spikes to display: ");
                        }
                        
                        displayTopK(maxHeap, k);

                        printf("\n-----------------------------\n");

                        break;
                    }


            case 4:
                        {
                            graph g;
                            int choice, v, e, u, v2;
                            bool going_on = true;

                            while (going_on) {
                                printf("\n=========================\n");
                                printf(" DEADLOCK DETECTION \n");
                                printf("=========================\n");
                                printf("1. Acyclic graph (example)\n");
                                printf("2. Cyclic graph (example)\n");
                                printf("3. Custom input\n");
                                printf("4. Add vertex\n");
                                printf("5. Add edge\n");
                                printf("6. Detect deadlock\n");
                                printf("7. Exit\n");
                                printf("Enter choice: ");
                                if(scanf("%d", &choice) != 1) {
                                    printf("Invalid input! Please enter a number.\n");
                                    while(getchar() != '\n');
                                    continue;
                                }
                                

                                if (choice < 1 || choice > 7) {
                                printf("Invalid choice! Please enter 1-7 only.\n");
                                continue;
                        }
                                switch (choice) {
                                case 1:
                                    initGraph(&g, 4);
                                    addEdge(&g, 0, 1);
                                    addEdge(&g, 1, 2);
                                    addEdge(&g, 2, 3);
                                    printf("\n Acyclic Graph (No Deadlock)\n");
                                    displayGraph(&g);
                                    if (detectCycle(&g))
                                        printf("\n DEADLOCK DETECTED (cycle)! System is UNSAFE.\n");
                                    else
                                        printf("\n Graph is Acyclic (Safe)\n");
                                    break;

                                case 2:
                                    initGraph(&g, 4);
                                    addEdge(&g, 0, 1);
                                    addEdge(&g, 1, 2);
                                    addEdge(&g, 2, 0); // cycle created
                                    printf("\n--- Cyclic Graph ---\n");
                                    displayGraph(&g);
                                    if (detectCycle(&g))
                                        printf("\nDEADLOCK DETECTED (cycle)! System is UNSAFE.\n");
                                    else
                                        printf("\n Graph is Acyclic (Safe)\n");
                                    break;

                                case 3:
                                    printf("Enter number of vertices (v >= 2): ");
                                    fflush(stdin);
                                    if (scanf("%d", &v)  != 1) {
                                        printf("Invalid input for vertices!\n");
                                        while (getchar() != '\n'); // clear invalid input
                                        continue;
                                    }

                                    if(v < 2){
                                        printf("Deadlock detection requires atleast 2 processes (vertices).");
                                        break;
                                    }

                                    initGraph(&g, v);

                                    printf("Enter number of edges: ");
                                    if (scanf("%d", &e)!=1 || e > v*v ||e < 0 ) {
                                        printf("Invalid number of edges");
                                        while (getchar() != '\n'); // clear invalid input
                                        continue;
                                    }
                                    if(e==0){
                                        printf("No edges-> graph is acyclic. \n");
                                        displayGraph(&g);
                                        printf("Safe Graph\n");
                                        break;
                                    }

                                    for (int i = 0; i < e; i++) {
                                        printf("Enter edge (u v): ");
                                        if (scanf("%d %d", &u, &v2) != 2 || u<0 || u>=v || v2<0 || v2>=v) {
                                            printf(" Invalid edge! Range 0 to %d.\n", v-1);
                                            while (getchar()!='\n'); i--; continue;
                                        }
                                        addEdge(&g, u, v2);
                                    }

                                    displayGraph(&g);

                                    if (detectCycle(&g))
                                        printf("\n Cycle detected!\n");
                                    else
                                        printf("\n Graph is Acyclic (Safe)\n");
                                    break;
                                case 4:
                                    addVertex(&g);
                                    displayGraph(&g);
                                    break;
                                case 5:
                                    printf("Add edge (u,v): ");
                                    scanf("%d %d", &u,&v2);
                                    addEdge(&g,u,v2);
                                    displayGraph(&g);
                                    break;
                                case 6:
                                    if(g.vertices < 2) {
                                        printf("Cannot detect deadlock- need atleast 2 vertices!\n");
                                    } else {
                                        if (detectCycle(&g)) {
                                        printf("\n DEADLOCK DETECTED! System is UNSAFE.\n");
                                        printf("   Circular wait exists between processes.\n");
                                    } else {
                                        printf("\n NO DEADLOCK FOUND! System is SAFE.\n");
                                        printf("   No circular wait detected.\n");
                                        }
                                    }
                                    break;
                                case 7:
                                    going_on = false;
                                   break;

                                default:
                                    printf("Invalid choice!\n");
                                }
                            }

                            break;
                        }          
            
            case 5:
            {
                        printf("Exitting...\n");
                        keep_going = false;
                        break;
            }

            default:{
                printf("invalid choice!\n");
                break;

            }
            
            }
        
    }

    return 0;
}
