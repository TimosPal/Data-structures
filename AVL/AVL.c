#include "AVL.h"

#include <stdlib.h>
#include <assert.h>

Tree_Node* AddHelper(Tree_Node* node, void* value, int (*cmpFunc)(void*, void*));
Tree_Node* Rotate(Tree_Node* node1,bool rotateLeft);
void DestroyHelper(Tree_Node* node);
void FreeValuesHelper(Tree_Node* node,void (*freeMethod)(void*));
bool GetHelper(AVL avl,Tree_Node* node,void* key,void** value);

#define MAX(x,y) ((x) > (y)) ? (x) : (y)

Tree_Node* Tree_NodeNew(void* value){
    Tree_Node* node = malloc(sizeof(Tree_Node));
    node->value = value;
    node->maxHeight = 1;
    node->left = node->right = NULL;
    return node;
}

void AVL_Init(AVL* avl,int (*cmpFunc)(void*,void*)){
    avl->head = NULL;
    avl->cmpFunc = cmpFunc;
}

void AVL_Destroy(AVL* avl ){
    DestroyHelper(avl->head);
}

void DestroyHelper(Tree_Node* node){
    if(node == NULL)
        return;
    DestroyHelper(node->left);
    DestroyHelper(node->right);
    free(node);
}

void AVL_FreeValues(AVL avl, void (*freeMethod)(void*)){
    FreeValuesHelper(avl.head,freeMethod);
}

void FreeValuesHelper(Tree_Node* node,void (*freeMethod)(void*)){
    if(node == NULL)
        return;
    FreeValuesHelper(node->left,freeMethod);
    FreeValuesHelper(node->right,freeMethod);
    freeMethod(node->value);
}

bool AVL_Get(AVL avl,void* key,void** value){
    return GetHelper(avl,avl.head,key,value);
}

bool GetHelper(AVL avl,Tree_Node* node,void* key,void** value){
    if(node == NULL){
        return false;
    }

    int result = avl.cmpFunc(key,node->value);
    if(result == 0){
        *value = node->value;
        return true;
    }else if(result > 0){
        return GetHelper(avl,node->right,key,value);
    }else{
        return GetHelper(avl,node->left,key,value);
    }
}

void AVL_Add(AVL* avl,void* value){
    //We update the head node for possible changes to the root.
    avl->head = AddHelper(avl->head,value,avl->cmpFunc);
}

int GetNodeMaxHeight(Tree_Node* node){
    return (node == NULL) ? 0 : node->maxHeight;
}

void UpdateHeight(Tree_Node *node){
    node->maxHeight = MAX(GetNodeMaxHeight(node->left), GetNodeMaxHeight(node->right)) + 1;
}

int GetBalanceFactor(Tree_Node* node){
    return GetNodeMaxHeight(node->left) - GetNodeMaxHeight(node->right);
}

Tree_Node* AddHelper(Tree_Node* node, void* value, int (*cmpFunc)(void*, void*)){
    if(node == NULL) {
        return Tree_NodeNew(value);
    }else if(cmpFunc(value,node->value) < 0){
        node->left = AddHelper(node->left,value,cmpFunc);
    }else if(cmpFunc(value,node->value) > 0) {
        node->right = AddHelper(node->right, value, cmpFunc);
    }else{ //Value is a duplicate so what should happen is decided here.
        assert(false); //Duplicates are not allowed since they are stored inside the node's duplicate's list.
    }

    //the new maxHeight of the node is the maximum height of the two children.
    //Balancing factor should be {-1,0,1} for the tree to be balanced.
    //If not we have to apply appropriate rotations.
    UpdateHeight(node);
    int balanceFactor = GetBalanceFactor(node);

    //We have 4 cases LL LR RL RR depending on the balance factor.
    if(balanceFactor > 1) { //Left side longer -> L
        if (cmpFunc(value, node->left->value) < 0) { //the value was added to the left -> LL.
            return Rotate(node,false);
        } else if (cmpFunc(value, node->left->value) > 0) { //the value was added to the right -> LR.
            node->left = Rotate(node->left,true);
            return Rotate(node,false);
        }
    }else if(balanceFactor < -1) { //Right side longer -> R
        if (cmpFunc(value, node->right->value) > 0) { //the value was added to the right -> RR.
            return Rotate(node,true);
        } else if (cmpFunc(value, node->right->value) < 0) { //the value was added to the left -> RL.
            node->right = Rotate(node->right,false);
            return Rotate(node,true);
        }
    }

    return node;
}

Tree_Node* Rotate(Tree_Node* node1,bool rotateLeft){
    Tree_Node* oldNode2 = (rotateLeft) ? node1->right : node1->left;
    Tree_Node* oldNode2Lchild = (rotateLeft) ? oldNode2->left : oldNode2->right;

    if(rotateLeft){
        oldNode2->left = node1;
        node1->right = oldNode2Lchild;
    }else{
        oldNode2->right = node1;
        node1->left = oldNode2Lchild;
    }

    UpdateHeight(node1);
    UpdateHeight(oldNode2);
    return oldNode2;
}