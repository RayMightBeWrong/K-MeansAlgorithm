#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <iostream>
#include <chrono>

using namespace std;

#define K 32

void init(int N, float *px, float *py, float *cx, float *cy);
void rearrangeCluster(float *cx, float *cy, float *x, float *y, int *size);
void kmeans(const int N, const int THREADS, float *px, float *py, 
		float *cx, float *cy, int *point_cluster);

void printInfo(int N, float *cx, float *cy, int *size, int iterations);


#endif
