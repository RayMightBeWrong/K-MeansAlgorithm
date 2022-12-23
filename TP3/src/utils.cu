#include "../include/utils.h"

using namespace std;









cudaEvent_t start, stop;

void checkCUDAError (const char *msg) {
	cudaError_t err = cudaGetLastError();
	if( cudaSuccess != err) {
		cerr << "Cuda error: " << msg << ", " << cudaGetErrorString( err) << endl;
		exit(-1);
	}
}

// These are specific to measure the execution of only the kernel execution - might be useful
void startKernelTime (void) {
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start);
}

void stopKernelTime (void) {
	cudaEventRecord(stop);

	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);

	cout << endl << "Basic profiling: " << milliseconds << " ms have elapsed for the kernel execution" << endl << endl;
}


















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
inline int findCluster(int K, float px, float py, float *cx, float *cy){
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

// associates points to a cluster 
int attributeClusters(int N, int K, int THREADS, float *px, float *py, 
			float *cx, float *cy, int *point_cluster){
	int changed = 0;
	int cluster;
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

	for(int i = 0; i < K; i++){
		cx[i] = x[i] / size[i];
		cy[i] = y[i] / size[i];
	}
}

__global__ 
void attributeCluster(float *cx, float *cy, float *px, float *py, 
			int *point_cluster, int *changed, const int N, const int K){
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id < N){
		float x = px[id];
		float y = py[id];
		
		//int changed = 0;
		int cluster = -1;
		float dist_min = 2; 

		for(int j = 0; j < K; j++){
			float dist = (cx[j] - x) * (cx[j] - x) + (cy[j] - y) * (cy[j] - y);

			if (dist < dist_min){
				dist_min = dist;
				cluster = j;
			}
		}


		if (cluster != point_cluster[id]){
			point_cluster[id] = cluster;
			if (*changed == 0)
				atomicAdd(changed, 1);
		}
	}
} 

int clustersChanged(int blocks, int *changed){
	int res = 0;

	for(int i = 0; i < blocks; i++){
		if (changed[i] != 0){
			res = 1;
			break;
		}
	}

	return res;
}

// executes k-means algorithm and returns how many iterations were made
void kmeans(int N, int K, int THREADS, float *px, float *py, 
		float *cx, float *cy, int *point_cluster, int *size){

	int blocks = N/THREADS + 1; 
	float *dcx, *dcy, *dpx, *dpy;
	int *dpoint_cluster, *dchanged, changed[1];
	memset(point_cluster, 0, N * sizeof(int));

	cudaMalloc((void**) &dcx, K * sizeof(float));
	cudaMalloc((void**) &dcy, K * sizeof(float));
	cudaMalloc((void**) &dpx, N * sizeof(float));
	cudaMalloc((void**) &dpy, N * sizeof(float));
	cudaMalloc((void**) &dpoint_cluster, N * sizeof(int));
	cudaMalloc((void**) &dchanged, sizeof(int));
	checkCUDAError("malloc");

	cudaMemcpy(dcx, cx, K * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dcy, cy, K * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dpx, px, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dpy, py, N * sizeof(float), cudaMemcpyHostToDevice);
	checkCUDAError("cpy");

	int i;
	for(i = 0; i < 20; i++){
		cudaMemcpy(dpoint_cluster, point_cluster, N * sizeof(int), cudaMemcpyHostToDevice);
		cudaMemset(dchanged, 0, sizeof(int));
		//startKernelTime ();
		attributeCluster <<< blocks, THREADS >>> (dcx, dcy, dpx, dpy, dpoint_cluster, dchanged, N, K);
		//stopKernelTime ();

		cudaMemcpy(changed, dchanged, sizeof(int), cudaMemcpyDeviceToHost);
		if (*changed == 0)
			break;

		cudaMemcpy(point_cluster, dpoint_cluster, N * sizeof(int), cudaMemcpyDeviceToHost);
		cudaMemcpy(cx, dcx, K * sizeof(float), cudaMemcpyDeviceToHost);
		cudaMemcpy(cy, dcy, K * sizeof(float), cudaMemcpyDeviceToHost);
		rearrangeCluster(N, K, THREADS, px, py, cx, cy, point_cluster, size);

		cudaMemcpy(dcx, cx, K * sizeof(int), cudaMemcpyHostToDevice);
		cudaMemcpy(dcy, cy, K * sizeof(int), cudaMemcpyHostToDevice);
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
