#include "../include/utils.h"

int main(){
	// px contains the x values of the points
	// py contains the y values of the points
	float *px = malloc(sizeof(float) * N);
	float *py = malloc(sizeof(float) * N);
	
	// cx contains the x values of every cluster's centroids 
	// cy contains the y values of every cluster's centroids 
	float *cx = malloc(sizeof(float) * K);
	float *cy = malloc(sizeof(float) * K);

	// keeps track of which cluster a point belongs yo
	int *point_cluster = malloc(sizeof(int) * N);

	// init gives random values to points and clusters
	// algorithm executes the k-means algorithm
	init(px, py, cx, cy);
	int iterations = algorithm(px, py, cx, cy, point_cluster);

	printf("N = %d, K = %d\n", N, K);
	printClusters(cx, cy);
	printf("Iterations: %d\n", iterations);

	return 0;
}
