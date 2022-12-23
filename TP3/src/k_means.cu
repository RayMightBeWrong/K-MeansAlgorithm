#include "../include/utils.h"

int main(int argc, char **argv){
	// N is the number of points 
	// K is the number of clusters and gets the value of the environment variable CP_CLUSTERS 
	int N = atoi(argv[1]);
	int K = atoi(argv[2]);
	
	// px contains the x values of the points
	// py contains the y values of the points
	float *px = (float*) malloc(sizeof(float) * N);
	float *py = (float*) malloc(sizeof(float) * N);
	
	// cx contains the x values of every cluster's centroids 
	// cy contains the y values of every cluster's centroids 
	float *cx = (float*) malloc(sizeof(float) * K);
	float *cy = (float*) malloc(sizeof(float) * K);

	// keeps track of which cluster a point belongs yo
	int *point_cluster = (int*) malloc(sizeof(int) * N);
	
	// keeps track of which cluster a point belongs yo
	int *cluster_size = (int*) malloc(sizeof(int) * K);

	// init gives random values to points and clusters
	// algorithm executes the k-means algorithm and prints the information out
	init(N, K, px, py, cx, cy);

	if (argc == 4){
		int THREADS = atoi(argv[3]);
		kmeans(N, K, THREADS, px, py, cx, cy, point_cluster, cluster_size);
	}
	else
		kmeans(N, K, 1, px, py, cx, cy, point_cluster, cluster_size);
		

	return 0;
}
