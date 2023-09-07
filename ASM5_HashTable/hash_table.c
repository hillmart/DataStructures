/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Martin Hill   
 * Email: hillmart@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "dynarray.h"
#include "list.h"
#include "hash_table.h"


struct ht_node{
    void* key;
    void* value;
};

/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */
struct ht{
    struct dynarray* da;
    int size;
    int capacity;
    struct ht_node* dummy;
};

/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* ht = malloc(sizeof(struct ht));
    ht->da = dynarray_create();
    ht->size = 0; 
    ht->capacity = 2;
    ht->dummy = malloc(sizeof(struct ht_node));
    ht->dummy->key = malloc(sizeof(int));
    ht->dummy->value = malloc(sizeof(int));
    *(int*)ht->dummy->key = -1;
    *(int*)ht->dummy->value = -1;
    return ht;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    for (int i = 0; i < ht->capacity; i++){
        if (dynarray_get(ht->da, i) != NULL){
            struct ht_node* node = (struct ht_node*)dynarray_get(ht->da, i);
            // if (node->key != NULL) {
            //     free(node->key);
            // }
            // if (node->value != NULL) {
            //     free(node->value);
            // }
            free(node);
        }
    }
    dynarray_free(ht->da);
    free(ht->dummy->key);
    free(ht->dummy->value);    
    free(ht->dummy);    
    free(ht);
    return;
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){
    if (ht->size > 0){
        return 0;
    }
    return 1;
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 */
int ht_size(struct ht* ht){
    return ht->size;
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash algorithm is totally up to you. Make sure to consider
 * Determinism, Uniformity, and Speed when design the hash algorithm
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element to be stored
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
    return *(int*)key % ht->capacity;
}


/*
 * This function should insert a given element into a hash table with a
 * specified key.  Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){
    // // Calculate the hash value for the key
    int hash_value = ht_hash_func(ht, key, convert);

    // // Check if the key already exists in the dynamic array
    struct ht_node* node = dynarray_get(ht->da, hash_value);
    if (ht->size > 0){
        while (node != NULL && node->key != NULL && convert(node->key) != -1) {
            //printf("%d \n", convert(node->key));
            if (convert(node->key) == convert(key)){
                //printf("entered same value\n");
                node->value = value;
                return;
            }
            hash_value = (hash_value + 1) % ht->capacity;
            node = dynarray_get(ht->da, hash_value);
        }
    }

    // // If the key does not exist in the dynamic array, create a new node
    struct ht_node* new_node = malloc(sizeof(struct ht_node));
    new_node->key = key;
    new_node->value = value;

    // Insert the new node into the dynamic array
    free(dynarray_get(ht->da, hash_value));
    dynarray_set(ht->da, hash_value, new_node);
    //printf("inserted key %d\n", *(int*)node->key);
    // Increment the size of the hash table
    ht->size++;

    // Check the load factor and resize the dynamic array if necessary
    double load_factor = (double) ht->size / ht->capacity;
    if (load_factor >= 0.75) {
        // Double the array capacity
        ht->capacity *= 2;

        // Create new dynamic array and rehash nodes
        struct dynarray* new_da = dynarray_create();
        for (int i = 0; i < ht->capacity; i++) {
            dynarray_insert(new_da, NULL);
        }
        for (int i = 0; i < ht->capacity / 2; i++) {
            struct ht_node* old_node = dynarray_get(ht->da, i);
            if (old_node != NULL && old_node->key != NULL && convert(old_node->key) != -1) {
                int new_hash_value = ht_hash_func(ht, old_node->key, convert);
                while (dynarray_get(new_da, new_hash_value) != NULL) {
                    new_hash_value = (new_hash_value + 1) % ht->capacity;
                }
                dynarray_set(new_da, new_hash_value, old_node);
            }
        }
        dynarray_free(ht->da);
        ht->da = new_da;
    }
}

/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
   int hash_value = ht_hash_func(ht, key, convert);
    struct ht_node* node = dynarray_get(ht->da, hash_value);
    int counter = 0;
    while (node != NULL && node->key != NULL && convert(node->key)!=-1){
        if(convert(node->key) == convert(key)){
            return node->value; 
        }
        hash_value++;
        hash_value = hash_value%ht->capacity;
        node = dynarray_get(ht->da, hash_value);
        if (counter++ > ht->capacity){
            break;
        }
    }
    return NULL;
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){
    int hash_value = ht_hash_func(ht, key, convert);
    struct ht_node* node = dynarray_get(ht->da, hash_value);
    //printf("%d\n",convert(node->key));
    //printf("%d\n",convert(key));
    while (node != NULL && node->key != NULL){
        //printf("%d\n",convert(node->key));
        //printf("%d\n",convert(key));
        if(convert(node->key) == convert(key)){
            //printf("entered if\n");
            // free(node->key);
            // free(node->value);
            //free(node);
            node = ht->dummy;
            ht->size--;
            return; 
        }
        hash_value++;
        hash_value = hash_value%ht->capacity;
        node = dynarray_get(ht->da, hash_value);
    }
    return;
}