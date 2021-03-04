#include "Hash.h"

#include <stdlib.h>
#include <assert.h>

Bucket* Bucket_New(int size){
    //The next node pointer of the list should be accounted in the size of the bucket.
    int maxPairs = (size - (int)(sizeof(Node*) + 2*sizeof(int))) / (int)sizeof(KeyValuePair);
    assert(maxPairs > 0); //At least 32 bytes per bucket.

    Bucket* bucket = malloc(sizeof(Bucket));
    bucket->pairs = malloc(sizeof(KeyValuePair) * maxPairs);
    bucket->maxPairs = maxPairs;
    bucket->pairsCounter = 0;
    return bucket;
}

void Hash_Init(Hash* hash, int hashSize, int bucketSize, unsigned int (*hashFunction)(void*), bool (*cmpFunction)(void*, void*)){
    hash->hashSize = hashSize;
    hash->bucketSize = bucketSize;
    hash->hashFunction = hashFunction;
    hash->cmpFunction = cmpFunction;
    hash->buckets = malloc(hashSize * sizeof(List));

    //We create lists containing 1 empty bucket for each slot on the array.
    //This way addition doesn't have to handle creating said lists making code easier to read.
    for(int i = 0; i < hashSize; i++) {
        List_Init(&hash->buckets[i]);
        Bucket* newBucket = Bucket_New(hash->bucketSize);
        List_AddValue(&hash->buckets[i], newBucket, 0);
    }

    List_Init(&hash->unorderedPairs);
}

void Hash_Add(Hash* hash,void* key,void* value){
    int index = hash->hashFunction(key) % hash->hashSize;
    List* bucketList = &hash->buckets[index];
    Bucket* bucket = (Bucket*)(List_GetNode(*bucketList,0)->value);
    if(bucket->pairsCounter + 1 > bucket->maxPairs){ //It does not fit inside the bucket.
        //We create a new one.
        bucket = Bucket_New(hash->bucketSize);
        List_AddValue(bucketList, bucket, 0);
    }

    KeyValuePair* currPair = &(bucket->pairs[bucket->pairsCounter]);
    currPair->key = key;
    currPair->value = value;
    bucket->pairsCounter++;

    List_Append(&hash->unorderedPairs, currPair);
}

bool Hash_GetValue(Hash hash,void* key,void** value){
    int index = hash.hashFunction(key) % hash.hashSize;
    List* bucketList = &hash.buckets[index];

    Node* currNode = List_GetNode(*bucketList,0);
    while(currNode != NULL) {
        Bucket *bucket = (Bucket*)currNode->value;
        for (int i = 0; i < bucket->pairsCounter; i++) {
            KeyValuePair currPair = bucket->pairs[i];
            if(hash.cmpFunction(currPair.key,key)) {
                *value = currPair.value;
                return true;
            }
        }

        currNode = currNode->next;
    }

    return false;
}

void BucketFreeMethod(void* value){
    free(((Bucket*)value)->pairs);
    free(value);
}

void Hash_FreeValues(Hash hash,void (*freeMethod)(void*)){
    Node* currNode = hash.unorderedPairs.head;
    while(currNode != NULL){
        freeMethod(((KeyValuePair*)currNode->value)->value);
        currNode = currNode->next;
    }
}

void Hash_Destroy(Hash* hash){
    for (int i = 0; i < hash->hashSize; i++) {
        List* currList = &hash->buckets[i];
        List_FreeValues(*currList,BucketFreeMethod);
        List_Destroy(currList);
    }

    List_Destroy(&hash->unorderedPairs);
    free(hash->buckets);
}