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

int findCluster(P point){
	float dist_min = 2; 
	int min = -1;

	for(int j = 0; j < K; j++){
		float dist, d1, d2;
		d1 = clusters[j]-> centroid-> x - point-> x;
		d2 = clusters[j]-> centroid-> y - point-> y;
		dist = pow(d1, 2) + pow(d2, 2);
			
		if (dist < dist_min){
			dist_min = dist;
			min = j;
		}
	}

	return min;
}


void attributeInitialClusters(){
	for(int i = 0; i < N; i++){
		points[i]-> cluster = findCluster(points[i]);
	}
}

int attributeClusters(){
	int changed = 0;

	for(int i = 0; i < N; i++){
		int cluster = findCluster(points[i]);
		if (!changed && cluster != points[i]-> cluster){
			changed = 1;
			points[i]-> cluster = cluster;	
		}
		else if (cluster != points[i]-> cluster){
			points[i]-> cluster = cluster;	
		}
	}

	return changed;
}

void rearrangeCluster(){
	int size[K];
	float x[K], y[K];
	for(int i = 0; i < K; i++){
		size[i] = 0;
		x[i] = 0;
		y[i] = 0;
	}

	for(int i = 0; i < N; i++){
		size[points[i]-> cluster]++;
		x[points[i]-> cluster] += points[i]-> x;
		y[points[i]-> cluster] += points[i]-> y;
	}

	for(int i = 0; i < K; i++){
		clusters[i]-> centroid-> x = x[i] / size[i];
		clusters[i]-> centroid-> y = y[i] / size[i];
	}
}

void algorithm(){
	iterations = 1;

	attributeInitialClusters();
	while(1){
		rearrangeCluster();
		if (attributeClusters() == 1){
			// continuar algoritmo	
		}
		else
			break;
		iterations++;
	}

	for(int i = 0; i < K; i++){
		printf("[%d]: %.3f, %.3f", i, clusters[i]-> centroid-> x, clusters[i]-> centroid-> y);
	}
}
