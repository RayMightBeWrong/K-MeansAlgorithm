#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define N 10000000
#define K 4

void init(float *px, float *py, float *cx, float *cy);
int  findCluster(float px, float py, float *cx, float *cy);
int  attributeClusters(float *px, float *py, float *cx, float *cy, int *point_cluster);
void attributeInitialClusters(float *px, float *py, float *cx, float *cy, int *point_cluster);
void rearrangeCluster(float *px, float *py, float *cx, float *cy, int *point_cluster);
int  algorithm(float *px, float *py, float *cx, float *cy, int *point_cluster);

void printClusters(float *cx, float *cy);

#endif
