#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

void init(int N, int K, float *px, float *py, float *cx, float *cy);
int  findCluster(int K, float px, float py, float *cx, float *cy);
int  attributeClusters(int N, int K, float *px, float *py, float *cx, float *cy, int *point_cluster);
void attributeInitialClusters(int N, int K, float *px, float *py, 
				float *cx, float *cy, int *point_cluster);
void rearrangeCluster(int N, int K, float *px, float *py, 
			float *cx, float *cy, int *point_cluster, int *size);
void algorithm(int N, int K, float *px, float *py, float *cx, float *cy, int *point_cluster, int *size);

void printInfo(int N, int K, float *cx, float *cy, int *size, int iterations);

#endif
