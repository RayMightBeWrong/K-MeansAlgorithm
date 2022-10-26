#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define N 10000000
#define K 4

typedef struct point *P;

void init(P *points, P *clusters);
int  findCluster(P point, P *clusters);
int  attributeClusters(P *points, P *clusters, int *point_cluster);
void attributeInitialClusters(P *points, P *clusters, int *point_cluster);
void rearrangeCluster(P *points, P *clusters, int *point_cluster);
void algorithm(P *points, P *clusters, int *point_cluster, int *iterations);

void printClusters(P *clusters);

#endif
