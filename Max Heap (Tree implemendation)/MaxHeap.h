#ifndef MAXHEAP_H
#define MAXHEAP_H

typedef struct MaxHeap_Node{
    struct MaxHeap_Node* left;
    struct MaxHeap_Node* right;
    void* value;
}MaxHeap_Node;

typedef struct MaxHeap{
    MaxHeap_Node* head;
    int height;
    int nodesCount; //total count of nodes.
    int currTotalUpperBound; //total upper bound of nodes.
    int currLeafeLevelUpperBound; //upper bound of the last leafe level.
    int (*cmpFunc)(void*,void*);
}MaxHeap;

void MaxHeap_Init(MaxHeap* heap,int (*cmpFunc)(void*,void*));
void MaxHeap_Add(MaxHeap* heap,void* value);
void MaxHeap_Destroy(MaxHeap* heap);
void MaxHeap_Pop(MaxHeap* heap,void** value);

#endif