#include "../include/utils.h"

// function that generates random float values for N points
// and assigns the first K points as centroids
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
	#pragma omp parallel for num_threads(THREADS)
	for(int i = 0; i < N; i++)
		point_cluster[i] = findCluster(K, px[i], py[i], cx, cy);
}


// associates points to a cluster 
int attributeClusters(int N, int K, int THREADS, float *px, float *py, 
			float *cx, float *cy, int *point_cluster){
	int changed = 0;
	int cluster;

	#pragma omp parallel for num_threads(THREADS) private(cluster)
	for(int i = 0; i < N; i++){
		cluster = findCluster(K, px[i], py[i], cx, cy);
		if (cluster != point_cluster[i]){
			changed = 1;
			point_cluster[i] = cluster;
		}
	}

	return changed;
}


// calculates the centroids of each cluster
void rearrangeCluster(int N, int K, int THREADS, float *px, float *py, 
				float *cx, float *cy, int *point_cluster, int *size){
	// x and y contain the sum of x and y values (respectively) 
	// of the points that belong to the cluster
	float x[K], y[K];
	
	// local_x, local_y and local_size are proxies
	// for the variables x, y and size (respectively)
	// to avoid data races 
	float local_x[K], local_y[K];
	int local_size[K];

	for(int i = 0; i < K; i++){
		size[i] = 0;
		x[i] = 0;
		y[i] = 0;
		local_size[i] = 0;
		local_x[i] = 0;
		local_y[i] = 0;
	}

	#pragma omp parallel num_threads(THREADS) firstprivate(local_size, local_x, local_y)
	{
		#pragma omp for nowait
		for(int i = 0; i < N; i++){
			local_size[point_cluster[i]]++;
			local_x[point_cluster[i]] += px[i];
			local_y[point_cluster[i]] += py[i];
		}

		#pragma omp critical 
		{ 
			// synchronization of x, y and size values
			// with the values of the local thread variables' values
			for(int i = 0; i < K; i++){
				size[i] += local_size[i];
				x[i] += local_x[i];
				y[i] += local_y[i];
			}
		}
	}

	// calculate a centroid's new value
	for(int i = 0; i < K; i++){
		cx[i] = x[i] / size[i];
		cy[i] = y[i] / size[i];
	}
}


// executes k-means algorithm
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
