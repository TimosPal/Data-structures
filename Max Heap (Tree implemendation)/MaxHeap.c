#include "MaxHeap.h"

#include <stdlib.h>

MaxHeap_Node* MaxHeapHelperAdd(MaxHeap_Node* node,void* value,MaxHeap* heap,int digit);
MaxHeap_Node* FindLeafe(MaxHeap_Node* node, MaxHeap* heap, int digit);
void MaxHeapHelperDestroy(MaxHeap_Node* node);
void HeapifyDown(MaxHeap_Node* node,MaxHeap* heap);

MaxHeap_Node* MaxHeap_NodeNew(void* value){
    MaxHeap_Node* node = malloc(sizeof(MaxHeap_Node));
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

void MaxHeap_Init(MaxHeap* heap,int (*cmpFunc)(void*,void*)){
    heap->head = NULL;
    heap->nodesCount = 0;
    heap->currTotalUpperBound = 0;
    heap->currLeafeLevelUpperBound = 0;
    heap->height = 0;
    heap->cmpFunc = cmpFunc;
}

void UpdateHeightUP(MaxHeap* heap){ //Used for updating the height when adding.
    if(heap->nodesCount > heap->currTotalUpperBound){ //upper bound.
        heap->currLeafeLevelUpperBound *= 2;
        heap->currTotalUpperBound += heap->currLeafeLevelUpperBound;
        heap->height++;
    }
}

void UpdateHeightDown(MaxHeap* heap){ ///Used for updating the height when popping.
    if(heap->nodesCount <= (heap->currTotalUpperBound - heap->currLeafeLevelUpperBound)){ //lower bound.
        heap->currTotalUpperBound -= heap->currLeafeLevelUpperBound;
        heap->currLeafeLevelUpperBound /= 2;
        heap->height--;
    }
}

void MaxHeap_Add(MaxHeap* heap,void* value){
    heap->nodesCount++;
    if(heap->head == NULL){
        heap->height = 1;
        heap->currTotalUpperBound = 1;
        heap->currLeafeLevelUpperBound = 1;
        heap->head = MaxHeap_NodeNew(value);
    }else {
        UpdateHeightUP(heap);
        MaxHeapHelperAdd(heap->head, value, heap,heap->height - 1);
    }
}

MaxHeap_Node* MaxHeapHelperAdd(MaxHeap_Node* node,void* value,MaxHeap* heap,int digit){
    //Insert the node at the most left position.
    MaxHeap_Node* childNode;
    int choiceBit = heap->nodesCount & (0b1 << (digit - 1)); //Decides what path to take down the tree.
    if (choiceBit == 0) { //Left.
        if(digit > 1)
            childNode = MaxHeapHelperAdd(node->left, value, heap, digit - 1);
        else
            node->left = childNode = MaxHeap_NodeNew(value);

    } else { //Right.
        if(digit > 1)
            childNode = MaxHeapHelperAdd(node->right, value, heap, digit - 1);
        else
            node->right = childNode = MaxHeap_NodeNew(value);
    }

    //Heapify.
    if(heap->cmpFunc(childNode->value,node->value) > 0){
        //Swap nodes so the bigger one is at the top.
        void* temp = childNode->value;
        childNode->value = node->value;
        node->value = temp;
    }

    return node;
}

void MaxHeap_Pop(MaxHeap* heap,void** value) {
    heap->nodesCount--;
    *value = heap->head->value;

    if(heap->nodesCount == 0){
        heap->height = 0;
        heap->currLeafeLevelUpperBound = 0;
        heap->currTotalUpperBound = 0;

        free(heap->head);
        heap->head = NULL;
    }else{
        MaxHeap_Node* node = FindLeafe(heap->head, heap, heap->height - 1);
        heap->head->value = node->value;
        free(node);

        UpdateHeightDown(heap);
        HeapifyDown(heap->head,heap);
    }
}

void HeapifyDown(MaxHeap_Node* node,MaxHeap* heap){
    if(node->left != NULL){
        MaxHeap_Node* maxNode = node->left;
        if(node->right != NULL){ //Find max.
            if(heap->cmpFunc(node->right->value,node->left->value) >= 0)
                maxNode = node->right;
        }

        //Swap.
        if(heap->cmpFunc(maxNode->value,node->value) > 0) {
            void *temp = maxNode->value;
            maxNode->value = node->value;
            node->value = temp;
            HeapifyDown(maxNode,heap);
        }
    }
}

MaxHeap_Node* FindLeafe(MaxHeap_Node* node, MaxHeap* heap, int digit){
    MaxHeap_Node* childNode;
    int choiceBit = (heap->nodesCount + 1) & (0b1 << (digit - 1)); //Decides what path to take down the tree.
    if (choiceBit == 0) { //Left.
        if(digit > 1) {
            return FindLeafe(node->left, heap, digit - 1);
        }else{
            childNode = node->left;
            node->left = NULL;
            return childNode;
        }
    } else { //Right.
        if(digit > 1) {
            return FindLeafe(node->right, heap, digit - 1);
        }else{
            childNode = node->right;
            node->right = NULL;
            return childNode;
        }
    }
}

void MaxHeap_Destroy(MaxHeap* heap){
    MaxHeapHelperDestroy(heap->head);
}

void MaxHeapHelperDestroy(MaxHeap_Node* node){
    if(node == NULL)
        return;
    MaxHeapHelperDestroy(node->left);
    MaxHeapHelperDestroy(node->right);
    free(node);
}