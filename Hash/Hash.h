#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

#include "List.h"

typedef struct Hash{
    List* buckets;
    List unorderedPairs;
    unsigned int (*hashFunction)(void*);
    bool (*cmpFunction)(void*,void*);

    int hashSize;
    int bucketSize;
}Hash;

typedef struct KeyValuePair{
    void* key;
    void* value;
}KeyValuePair;

typedef struct Bucket{
    int maxPairs;
    int pairsCounter;
    KeyValuePair* pairs;
}Bucket;

void Hash_Init(Hash* hash, int hashSize, int bucketSize, unsigned int (*hashFunction)(void*), bool (*cmpFunction)(void*, void*)); //Creates an empty hash of size hashSize.

bool Hash_GetValue(Hash hash,void* key,void** value); //Returns a value based on a key.

void Hash_Add(Hash* hash,void* key,void* value); //Add value to the hash based on the hash function provided.
void Hash_Destroy(Hash* hash); //Destroys the hash freeing all the allocated memory.
void Hash_FreeValues(Hash hash,void (*freeMethod)(void*)); //Frees the values inside the buckets according to the provided function.

#endif
