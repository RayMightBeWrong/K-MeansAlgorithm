#include "../include/utils.h"


void init(float *px, float *py, float *cx, float *cy){
	srand(10);
	for(int i = 0; i < N; i++){
		px[i] = (float) rand() / RAND_MAX;
		py[i] = (float) rand() / RAND_MAX;
	}

#pragma omp simd
	for(int i = 0; i < K; i++){
		cx[i] = px[i];
		cy[i] = py[i];
	}
}

int findCluster(float px, float py, float *cx, float *cy){
	float dist_min = 2; 
	int min = -1;

	for(int j = 0; j < K; j++){
		float dist, d1, d2;
		d1 = cx[j] - px;
		d2 = cy[j] - py;
		dist = d1 * d1 + d2 * d2;

		if (dist < dist_min){
			dist_min = dist;
			min = j;
		}
	}

	return min;
}


void attributeInitialClusters(float *px, float *py, float *cx, float *cy, int *point_cluster){
	for(int i = 0; i < N; i++)
		point_cluster[i] = findCluster(px[i], py[i], cx, cy);
}

int attributeClusters(float *px, float *py, float *cx, float *cy, int *point_cluster){
	int changed = 0;

	int i;
	for(i = 0; !changed && i < N; i++){
		int cluster = findCluster(px[i], py[i], cx, cy);
		if (cluster != point_cluster[i]){
			changed = 1;
			point_cluster[i] = cluster;
		}
	}

	for(; i < N; i++){
		int cluster = findCluster(px[i], py[i], cx, cy);
		point_cluster[i] = cluster;	
	}

	return changed;
}

void rearrangeCluster(float *px, float *py, float *cx, float *cy, int *point_cluster){
	int size[K];
	float x[K], y[K];

#pragma omp simd
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

#pragma omp simd
	for(int i = 0; i < K; i++){
		cx[i] = x[i] / size[i];
		cy[i] = y[i] / size[i];
	}
}

void algorithm(float *px, float *py, float *cx, float *cy, int *point_cluster, int *iterations){
	*iterations = 1;

	attributeInitialClusters(px, py, cx, cy, point_cluster);
	while(1){
		rearrangeCluster(px, py, cx, cy, point_cluster);
		if (attributeClusters(px, py, cx, cy, point_cluster) == 0)
			break;
		*iterations += 1;
	}

}

void printClusters(float *cx, float *cy){
	for(int i = 0; i < K; i++){
		printf("[%d]: %.3f, %.3f\n", i, cx[i], cy[i]);
	}
}
