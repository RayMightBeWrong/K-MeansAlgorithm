#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define N 10000000
#define K 4

typedef struct point *P;
typedef struct cluster *C;

void init(P *points, C *clusters);
int  findCluster(P point, C *clusters);
int  attributeClusters(P *points, C *clusters);
void attributeInitialClusters(P *points, C *clusters);
void rearrangeCluster(P *points, C *clusters);
void algorithm(P *points, C *clusters, int *iterations);

void printClusters(C *clusters);

#endif
