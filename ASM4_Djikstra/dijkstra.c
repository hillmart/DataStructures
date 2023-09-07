/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name:
 * Email:
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);
	
	/*
	 * Write your code here to find the
	 * least-cost paths from node 0 to all other nodes in the graph.  Make sure
	 * to print out the least-cost paths you find at the end.
	 *
	 * FIXME: 
 	 */
	int number = n_nodes;
	int a, b, cost;
	int (*adj_matrix)[number]= calloc(number, sizeof(*adj_matrix));
	int (*costs) = malloc(number * sizeof(*costs));
	int (*prev) = malloc(number * sizeof(*prev));

	for (int i = 0; i < number; i++){
		costs[i] = INT_MAX;
		prev[i] = -1;
	}

	for (int i = 0; i < n_edges; i++){
		fscanf(file, " %d %d %d ", &a, &b, &cost);
		adj_matrix[a][b] = cost;
	}

	struct pq* pq = pq_create();

	pq_insert(pq, START_NODE, 0);
	costs[START_NODE] = 0;

	while (pq_isempty(pq) != 1){
		int N = pq_remove_first(pq);

		for (int N_i = 0; N_i < number; N_i++){
			if (adj_matrix[N][N_i] != 0){
				int c_i = adj_matrix[N][N_i];
				if (costs[N] + c_i < costs[N_i]){
					costs[N_i] = costs[N] + c_i;
					prev[N_i] = N;
					pq_insert(pq, (void*)N_i, costs[N_i]);
				}
			}
		}
	}

	for (int i = 0; i < number; i++){
		printf("Node %d: Cost=%d, Path=%d", i, costs[i], i);
		int prev_node = prev[i];
		while (prev_node != -1){
			printf("<-%d", prev_node);
			prev_node = prev[prev_node];
		}
		printf("\n");
	}

	fclose(file);
	free(costs);
    free(prev);
    pq_free(pq);
    free(adj_matrix);

  return 0;
}
