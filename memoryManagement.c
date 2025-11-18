#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "memoryManagement.h"

node* root = NULL;
int ADDRESS_TRACKER = 0;
int MEMORY_TRACKER = 0;

void free_tree(node *root) {  // FIXED: Now properly declared
    if (!root) return;
    if (!root->lthread) free_tree(root->left);
    if (!root->rthread) free_tree(root->right);
    if (root->owner) root->owner = NULL;  // Clear process ownership
    free(root);
}

node *CreateNode(int size, Process *owner) {
    if (size <= 0) return NULL;
    node *pnode = malloc(sizeof(node));
    if (!pnode) return NULL;
    pnode->allocated = true; pnode->address = ADDRESS_TRACKER;
    pnode->value = size; pnode->occupied = size; pnode->count = 1;
    pnode->lthread = pnode->rthread = true; pnode->left = pnode->right = NULL;
    pnode->owner = owner;
    ADDRESS_TRACKER += size; MEMORY_TRACKER += size;
    return pnode;
}

node *inorderSuccessor(node *ptr) {
    if (!ptr) return NULL; if (ptr->rthread) return ptr->right;
    ptr = ptr->right; while (ptr && !ptr->lthread) ptr = ptr->left; return ptr;
}

int insert_or_edit(int size, Process *owner) {
    if (getAvailableMemory() < size) return -1;
    node *pnode = CreateNode(size, owner); if (!pnode) return -1;
    MEMORY_TRACKER -= size;

    if (!root) { root = pnode; MEMORY_TRACKER += size; return 0; }

    node *curr = root, *parent = NULL;
    while (curr) {
        parent = curr;
        if (size == curr->value) {
            curr->allocated = true; curr->count++; curr->owner = owner;
            free(pnode); MEMORY_TRACKER += size; return 0;
        } else if (size < curr->value) {
            if (!curr->lthread) curr = curr->left; else break;
        } else {
            if (!curr->allocated) {
                curr->allocated = true; curr->count++; curr->owner = owner;
                free(pnode); MEMORY_TRACKER += size; return 0;
            }
            if (!curr->rthread) curr = curr->right; else break;
        }
    }

    if (size < parent->value) {
        pnode->left = parent->left; pnode->right = parent;
        parent->lthread = false; parent->left = pnode;
    } else {
        pnode->right = parent->right; pnode->left = parent;
        parent->rthread = false; parent->right = pnode;
    }
    MEMORY_TRACKER += size; return 0;
}

bool allocForProcess(int size, Process *proc) {
    return insert_or_edit(size, proc) == 0;
}

bool deallocForProcess(Process *proc) {
    if (!proc || proc->memory <= 0) return false;
    int size = proc->memory; proc->memory = 0;
    
    node *curr = root;
    while (curr) {
        if (curr->owner == proc) {
            if (curr->count > 1) {
                curr->count--; curr->owner = NULL;
            } else {
                curr->allocated = false; curr->count = 0; curr->owner = NULL;
            }
            MEMORY_TRACKER -= size; return true;
        }
        if (size < curr->value) { if (!curr->lthread) curr = curr->left; else break; }
        else { if (!curr->rthread) curr = curr->right; else break; }
    }
    return false;
}

void displayMemory() {
    if (!root) { printf("Memory empty.\n"); return; }
    printf("\n=== MEMORY TREE (%d/%d used) ===\n", getUsedMemory(), MAX_MEM);
    node *curr = root; while (curr && curr->lthread) curr = curr->left;
    while (curr) {
        printf("Addr:%4d | Size:%4d | Owner:%-15s | Status:%s\n",
               curr->address, curr->value, curr->owner ? curr->owner->name : "FREE",
               curr->allocated ? "ALLOC" : "FREE");
        curr = inorderSuccessor(curr);
    }
}

int getAvailableMemory() { return MAX_MEM - MEMORY_TRACKER; }
int getUsedMemory() { return MEMORY_TRACKER; }

int to_allocate(int suggested) {
    int size; printf("Size (suggested %d): ", suggested);
    return (scanf("%d", &size) == 1 && size > 0) ? size : suggested;
}

int to_deallocate() {
    int size; printf("Deallocate size: ");
    return (scanf("%d", &size) == 1 && size > 0) ? size : 0;
}
