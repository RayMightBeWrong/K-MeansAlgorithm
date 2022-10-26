#include "../include/utils.h"

int main(){
	int iterations;
	P *points = malloc(sizeof(P) * N);
	C *clusters = malloc(sizeof(C) * K);

	init(points, clusters);
	algorithm(points, clusters, &iterations);

	printf("N = %d, K = %d\n", N, K);
	printClusters(clusters);
	printf("Iterations: %d\n", iterations);

	return 0;
}
