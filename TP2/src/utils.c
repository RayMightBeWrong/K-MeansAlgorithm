#include "../include/utils.h"

// function that generates random float values for N points
// and assigns the first K as centroids
void init(int N, int K, float *px, float *py, float *cx, float *cy){
	srand(10);
	for(int i = 0; i < N; i++){
		px[i] = (float) rand() / RAND_MAX;
		py[i] = (float) rand() / RAND_MAX;
	}

	for(int i = 0; i < K; i++){
		cx[i] = px[i];
		cy[i] = py[i];
	}
}

// finds the most appropriate cluster for a given point based on euclidean distance
int findCluster(int K, float px, float py, float *cx, float *cy){
	float dist_min = 2; 
	int min = -1;

	for(int j = 0; j < K; j++){
		float dist = (cx[j] - px) * (cx[j] - px) + (cy[j] - py) * (cy[j] - py);

		if (dist < dist_min){
			dist_min = dist;
			min = j;
		}
	}

	return min;
}

// associates points to a cluster for the 1st time in the algorithm
void attributeInitialClusters(int N, int K, int THREADS, float *px, float *py, 
				float *cx, float *cy, int *point_cluster){
	for(int i = 0; i < N; i++)
		point_cluster[i] = findCluster(K, px[i], py[i], cx, cy);
}

// associates points to a cluster 
int attributeClusters(int N, int K, int THREADS, float *px, float *py, 
			float *cx, float *cy, int *point_cluster){
	int changed = 0;

	// find the most appropriate cluster to a given point
	// if it's different from its current one, change to another 'for' loop
	int task;
#pragma omp parallel
#pragma omp for private(task) 
	for(int i = 0; i < N; i++){
		task = omp_get_thread_num() * (N / THREADS);
		int cluster = findCluster(K, px[i], py[i], cx, cy);
		if (cluster != point_cluster[i]){
			changed = 1;
			point_cluster[i] = cluster;
		}
		task ++;
	}

	return changed;
}

// calculates the centroids of each cluster
void rearrangeCluster(int N, int K, int THREADS, float *px, float *py, 
				float *cx, float *cy, int *point_cluster, int *size){
	// size keeps track of how much points are in each cluster
	/* x and y contain the sum of x and y values (respectively) 
	   of the points that belong to the cluster */
	float x[K], y[K];

	for(int i = 0; i < K; i++){
		size[i] = 0;
		x[i] = 0;
		y[i] = 0;
	}

	for(int i = 0; i < N; i++){
		size[point_cluster[i]]++;
		x[point_cluster[i]] += px[i];
		y[point_cluster[i]] += py[i];
	}

	// calculate a centroid's new value
	for(int i = 0; i < K; i++){
		cx[i] = x[i] / size[i];
		cy[i] = y[i] / size[i];
	}
}

// executes k-means algorithm and returns how many iterations were made
void algorithm(int N, int K, int THREADS, float *px, float *py, 
		float *cx, float *cy, int *point_cluster, int *size){
	int i;

	attributeInitialClusters(N, K, THREADS, px, py, cx, cy, point_cluster);
	for(i = 0; i < 20; i++){
		rearrangeCluster(N, K, THREADS, px, py, cx, cy, point_cluster, size);
		if (attributeClusters(N, K, THREADS, px, py, cx, cy, point_cluster) == 0)
			break;
	}

	printInfo(N, K, cx, cy, size, i);
}

// prints information about the clusters
void printInfo(int N, int K, float *cx, float *cy, int *size, int iterations){
	printf("N = %d, K = %d\n", N, K);

	for(int i = 0; i < K; i++)
		printf("[%d]: (%.3f, %.3f) : Size: %d\n", i, cx[i], cy[i], size[i]);

	printf("Iterations: %d\n", iterations);
}
