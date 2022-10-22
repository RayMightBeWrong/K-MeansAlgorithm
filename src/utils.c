#include "../include/utils.h"

struct point{
	float x;
	float y;
	int cluster;
};

struct cluster{
	P centroid;
};

void init(){
	points = malloc(sizeof(P) * N);	
	clusters = malloc(sizeof(C) * K);
	for(int i = 0; i < N; i++)
		points[i] = malloc(sizeof(struct point));
	for(int i = 0; i < K; i++){
		clusters[i] = malloc(sizeof(struct cluster));
		clusters[i]-> centroid = malloc(sizeof(struct point));
	}

	srand(10);
	for(int i = 0; i < N; i++){
		points[i]-> x = (float) rand() / RAND_MAX;
		points[i]-> y = (float) rand() / RAND_MAX;
	}
	for(int i = 0; i < K; i++){
		clusters[i]-> centroid-> x = points[i]-> x;
		clusters[i]-> centroid-> y = points[i]-> y;
	}
}

void beginAlgorithm(){
	for(int i = 0; i < N; i++){
		attributeCluster(points[i]);
	}
}

void attributeCluster(P point){
	float dist_min = 2; 
	int j_min = -1;

	for(int j = 0; j < K; j++){
		float dist, d1, d2;
		d1 = clusters[j]-> centroid-> x - point-> x;
		d2 = clusters[j]-> centroid-> y - point-> y;
		dist = pow(d1, 2) + pow(d2, 2);
			
		if (dist < dist_min){
			dist_min = dist;
			j_min = j;
		}
	}
	point-> cluster = j_min;
}

void algorithm(){
	int changes;
	iterations = 0;
	beginAlgorithm();
	do{
		changes = 0;
		iterations++;
	}
	while(changes != 0);
}
