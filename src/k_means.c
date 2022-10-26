#include "../include/utils.h"

int main(){
	int iterations;
	P *points = malloc(sizeof(P) * N);
	P *clusters = malloc(sizeof(P) * K);
	int *point_cluster = malloc(sizeof(int) * N);

	init(points, clusters);
	algorithm(points, clusters, point_cluster, &iterations);

	printf("N = %d, K = %d\n", N, K);
	printClusters(clusters);
	printf("Iterations: %d\n", iterations);

	return 0;
}
