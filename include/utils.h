#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 10000000
#define K 4

typedef struct point *P;
typedef struct cluster *C;

int iterations;
P *points;
C *clusters;

void init();
int  findCluster(P point);
int  attributeCluster(P point);
void attributeInitialClusters();
void rearrangeCluster();
void algorithm();

#endif
