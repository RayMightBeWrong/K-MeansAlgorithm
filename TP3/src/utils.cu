#include "../include/utils.h"

using namespace std;


// function that generates random float values for N points
// and assigns the first K as centroids
void init(int N, float *px, float *py, float *cx, float *cy){
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


__global__ 
void attributeCluster(float *cx, float *cy, float *px, float *py, 
			int *point_cluster, int *changed, const int N){
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id < N){
		__shared__ float lcx[K];
		__shared__ float lcy[K];
		float x = px[id];
		float y = py[id];

		int lid = threadIdx.x; 
		if (lid < K){
			lcx[lid] = cx[lid];
			lcy[lid] = cy[lid];
		}
		__syncthreads();
		
		int cluster = -1;
		float dist_min = 2; 

		for(int j = 0; j < K; j++){
			float dist = (lcx[j] - x) * (lcx[j] - x) + (lcy[j] - y) * (lcy[j] - y);

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


__global__
void calcClusterValues(const int N, const int THREADS, const float *px, const float *py, 
			float *x, float *y, int *size, const int *point_cluster){

	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id < N){
		int lid = threadIdx.x;
		__shared__ int lsize[K];
		__shared__ float lx[K];
		__shared__ float ly[K];

		if (lid < K){
			lsize[lid] = 0;
			lx[lid] = 0;	
			ly[lid] = 0;
		}
		__syncthreads();

		int cluster = point_cluster[id];
		atomicAdd(&lx[cluster], px[id]);
		atomicAdd(&ly[cluster], py[id]);
		atomicAdd(&lsize[cluster], 1);
		__syncthreads();

		if (lid < K){
			atomicAdd(&size[lid], lsize[lid]);
			atomicAdd(&x[lid], lx[lid]);
			atomicAdd(&y[lid], ly[lid]);
		}
		__syncthreads();
	}
}

// calculates the centroids of each cluster
void rearrangeCluster(float *cx, float *cy, float *x, float *y, int *size){
	for(int i = 0; i < K; i++){
		cx[i] = x[i] / size[i];
		cy[i] = y[i] / size[i];
	}
}

// executes k-means algorithm and returns how many iterations were made
void kmeans(const int N, const int THREADS, float *px, float *py, 
		float *cx, float *cy, int *point_cluster){

	int blocks = N/THREADS + 1; 
	float *dcx, *dcy, *dpx, *dpy;
	int *dpoint_cluster, *dchanged, changed[1], *dsize;
	int *size = (int*) malloc(K * sizeof(int));
	float *x = (float*) malloc(K * sizeof(float)), 
	      *y = (float*) malloc(K * sizeof(float));
	memset(point_cluster, 0, N * sizeof(int));

	cudaMalloc((void**) &dcx, K * sizeof(float));
	cudaMalloc((void**) &dcy, K * sizeof(float));
	cudaMalloc((void**) &dsize, K * sizeof(int));
	cudaMalloc((void**) &dpx, N * sizeof(float));
	cudaMalloc((void**) &dpy, N * sizeof(float));
	cudaMalloc((void**) &dpoint_cluster, N * sizeof(int));
	cudaMalloc((void**) &dchanged, sizeof(int));

	cudaMemcpy(dpx, px, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dpy, py, N * sizeof(float), cudaMemcpyHostToDevice);

	int i;
	for(i = 0; i < 20; i++){
		cudaMemcpy(dcx, cx, K * sizeof(int), cudaMemcpyHostToDevice);
		cudaMemcpy(dcy, cy, K * sizeof(int), cudaMemcpyHostToDevice);
		cudaMemset(dchanged, 0, sizeof(int));
		attributeCluster <<< blocks, THREADS >>> (dcx, dcy, dpx, dpy, dpoint_cluster, dchanged, N);
		cudaDeviceSynchronize();

		cudaMemcpy(changed, dchanged, sizeof(int), cudaMemcpyDeviceToHost);
		if (*changed == 0)
			break;

		cudaMemset(dsize, 0, K * sizeof(int));
		cudaMemset(dcx, 0, K * sizeof(int));
		cudaMemset(dcy, 0, K * sizeof(int));
		calcClusterValues <<< blocks, THREADS >>> (N, THREADS, dpx, dpy, dcx, dcy, dsize, dpoint_cluster);
		cudaDeviceSynchronize();

		cudaMemcpy(x, dcx, K * sizeof(float), cudaMemcpyDeviceToHost);
		cudaMemcpy(y, dcy, K * sizeof(float), cudaMemcpyDeviceToHost);
		cudaMemcpy(size, dsize, K * sizeof(float), cudaMemcpyDeviceToHost);
		
		rearrangeCluster(cx, cy, x, y, size);
	}
	
	printInfo(N, cx, cy, size, i);
}

// prints information about the clusters
void printInfo(int N, float *cx, float *cy, int *size, int iterations){
	printf("N = %d, K = %d\n", N, K);

	for(int i = 0; i < K; i++)
		printf("[%d]: (%.3f, %.3f) : Size: %d\n", i, cx[i], cy[i], size[i]);

	printf("Iterations: %d\n", iterations);
}
