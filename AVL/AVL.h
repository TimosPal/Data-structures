#ifndef AVL_H
#define AVL_H

#include <stdbool.h>

typedef struct Tree_Node{
    struct Tree_Node* left;
    struct Tree_Node* right;
    void* value;
    int maxHeight;
}Tree_Node;

typedef struct AVL{
    Tree_Node* head;
    int (*cmpFunc)(void*,void*);
}AVL;

void AVL_Init(AVL* avl,int (*cmpFunc)(void*,void*));
void AVL_Add(AVL* avl,void* value);
void AVL_Destroy(AVL* avl);
void AVL_FreeValues(AVL avl, void (*freeMethod)(void*));

bool AVL_Get(AVL avl,void* key,void** value);

#endif
