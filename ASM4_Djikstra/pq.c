/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Martin Hill
 * Email: hillmart@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
	struct dynarray* dynarray;
};

struct pq_node{
	void* value;
	int priority;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq));
	pq->dynarray = dynarray_create();
	return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	dynarray_free(pq->dynarray);
	free(pq);
	return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if (dynarray_size(pq->dynarray) == 0){
		return 1;
	}
	return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	struct pq_node* node = (struct pq_node*) malloc(sizeof(struct pq_node));
    node->value = value;
    node->priority = priority;
    
    dynarray_insert(pq->dynarray, node);
    int idx = dynarray_size(pq->dynarray) - 1;
    
    while (idx > 0) {
        int parent_idx = (idx - 1) / 2;
        struct pq_node* parent = dynarray_get(pq->dynarray, parent_idx);
        if (parent->priority < node->priority) {
            break;
        }
        dynarray_set(pq->dynarray, idx, parent);
        dynarray_set(pq->dynarray, parent_idx, node);
        idx = parent_idx;
    }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	struct pq_node* first = dynarray_get(pq->dynarray, 0);
	return first->value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	struct pq_node* first = dynarray_get(pq->dynarray, 0);
	return first->priority;
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
   if (pq_isempty(pq)) {
        return NULL;
    }
    
    struct pq_node* first_node = dynarray_get(pq->dynarray, 0);
    void* first_val = first_node->value;
    
    struct pq_node* last_node = dynarray_get(pq->dynarray, dynarray_size(pq->dynarray) - 1);
    dynarray_set(pq->dynarray, 0, last_node);
    dynarray_remove(pq->dynarray, dynarray_size(pq->dynarray) - 1);
    
    int idx = 0;
    while (idx < dynarray_size(pq->dynarray)) {
        int left_child_idx = 2 * idx + 1;
        int right_child_idx = 2 * idx + 2;
        
        if (left_child_idx >= dynarray_size(pq->dynarray)) {
            break;
        }
        
        int min_child_idx = left_child_idx;
        if (right_child_idx < dynarray_size(pq->dynarray)) {
            struct pq_node* left_child = dynarray_get(pq->dynarray, left_child_idx);
            struct pq_node* right_child = dynarray_get(pq->dynarray, right_child_idx);
            if (right_child->priority < left_child->priority) {
                min_child_idx = right_child_idx;
            }
        }
        
        struct pq_node* parent = dynarray_get(pq->dynarray, idx);
        struct pq_node* min_child = dynarray_get(pq->dynarray, min_child_idx);
        if (parent->priority <= min_child->priority) {
            break;
        }
        
        dynarray_set(pq->dynarray, idx, min_child);
        dynarray_set(pq->dynarray, min_child_idx, parent);
        idx = min_child_idx;
    }
    
    free(first_node);
    return first_val;	
}
