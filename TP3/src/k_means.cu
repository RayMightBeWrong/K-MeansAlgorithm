#include "../include/utils.h"


cudaEvent_t start, stop;

void checkCUDAError (const char *msg) {
        cudaError_t err = cudaGetLastError();
        if( cudaSuccess != err) {
                cerr << "Cuda error: " << msg << ", " << cudaGetErrorString( err) << endl;
                exit(-1);
        }
}

// These are specific to measure the execution of only the kernel execution - might be useful
void startKernelTime () {
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        cudaEventRecord(start);
}

void stopKernelTime () {
        cudaEventRecord(stop);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        cudaEventElapsedTime(&milliseconds, start, stop);

        cout << endl << "Basic profiling: " << milliseconds << " ms have elapsed for the kernel execution" << endl << endl;
}



int main(int argc, char **argv){
	// N is the number of points 
	int N = atoi(argv[1]);
	
	// px contains the x values of the points
	// py contains the y values of the points
	float *px = (float*) malloc(sizeof(float) * N);
	float *py = (float*) malloc(sizeof(float) * N);
	
	// cx contains the x values of every cluster's centroids 
	// cy contains the y values of every cluster's centroids 
	float *cx = (float*) malloc(sizeof(float) * K);
	float *cy = (float*) malloc(sizeof(float) * K);

	// keeps track of which cluster a point belongs to
	int *point_cluster = (int*) malloc(sizeof(int) * N);

	// init gives random values to points and clusters
	// algorithm executes the k-means algorithm and prints the information out
	init(N, px, py, cx, cy);

	if (argc == 3){
		int THREADS = atoi(argv[2]);
		kmeans(N, THREADS, px, py, cx, cy, point_cluster);
	}

	return 0;
}
