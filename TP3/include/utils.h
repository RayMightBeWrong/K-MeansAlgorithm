#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <iostream>

using namespace std;


void init(int N, int K, float *px, float *py, float *cx, float *cy);
int  attributeClusters(int N, int K, int THREADS, float *px, float *py, 
			float *cx, float *cy, int *point_cluster);
void attributeInitialClusters(int N, int K, int THREADS, float *px, float *py, 
				float *cx, float *cy, int *point_cluster);
void rearrangeCluster(int N, int K, int THREADS, float *px, float *py, 
			float *cx, float *cy, int *point_cluster, int *size);
void kmeans(int N, int K, int THREADS, float *px, float *py, 
		float *cx, float *cy, int *point_cluster, int *size);

void printInfo(int N, int K, float *cx, float *cy, int *size, int iterations);


#endif
