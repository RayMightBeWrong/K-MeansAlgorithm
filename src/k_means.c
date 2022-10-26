#include "../include/utils.h"

int main(){
	int iterations;
	float *px = malloc(sizeof(float) * N);
	float *py = malloc(sizeof(float) * N);
	float *cx = malloc(sizeof(float) * K);
	float *cy = malloc(sizeof(float) * K);
	int *point_cluster = malloc(sizeof(int) * N);

	init(px, py, cx, cy);
	algorithm(px, py, cx, cy, point_cluster, &iterations);

	printf("N = %d, K = %d\n", N, K);
	printClusters(cx, cy);
	printf("Iterations: %d\n", iterations);

	return 0;
}
