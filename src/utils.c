#include "../include/utils.h"

struct point{
	float x;
	float y;
};

void init(P *points, P *clusters){
	for(int i = 0; i < N; i++)
		points[i] = malloc(sizeof(struct point));
	for(int i = 0; i < K; i++){
		clusters[i] = malloc(sizeof(struct point));
	}

	srand(10);
	for(int i = 0; i < N; i++){
		points[i]-> x = (float) rand() / RAND_MAX;
		points[i]-> y = (float) rand() / RAND_MAX;
	}
	for(int i = 0; i < K; i++){
		clusters[i]-> x = points[i]-> x;
		clusters[i]-> y = points[i]-> y;
	}
}

int findCluster(P point, P *clusters){
	float dist_min = 2; 
	int min = -1;

	for(int j = 0; j < K; j++){
		float dist, d1, d2;
		d1 = clusters[j]-> x - point-> x;
		d2 = clusters[j]-> y - point-> y;
		dist = d1 * d1 + d2 * d2;

		if (dist < dist_min){
			dist_min = dist;
			min = j;
		}
	}

	return min;
}


void attributeInitialClusters(P *points, P *clusters, int *point_cluster){
	for(int i = 0; i < N; i++){
		point_cluster[i] = findCluster(points[i], clusters);
	}
}

int attributeClusters(P *points, P *clusters, int *point_cluster){
	int changed = 0;

	for(int i = 0; i < N; i++){
		int cluster = findCluster(points[i], clusters);
		if (!changed && cluster != point_cluster[i]){
			changed = 1;
			point_cluster[i] = cluster;	
		}
		else if (cluster != point_cluster[i]){
			point_cluster[i] = cluster;	
		}
	}

	return changed;
}

void rearrangeCluster(P *points, P *clusters, int *point_cluster){
	int size[K];
	float x[K], y[K];
	for(int i = 0; i < K; i++){
		size[i] = 0;
		x[i] = 0;
		y[i] = 0;
	}

	for(int i = 0; i < N; i++){
		size[point_cluster[i]]++;
		x[point_cluster[i]] += points[i]-> x;
		y[point_cluster[i]] += points[i]-> y;
	}

	for(int i = 0; i < K; i++){
		clusters[i]-> x = x[i] / size[i];
		clusters[i]-> y = y[i] / size[i];
	}
}

void algorithm(P *points, P *clusters, int *point_cluster, int *iterations){
	*iterations = 1;

	attributeInitialClusters(points, clusters, point_cluster);
	while(1){
		rearrangeCluster(points, clusters, point_cluster);
		if (attributeClusters(points, clusters, point_cluster) == 0)
			break;
		*iterations += 1;
	}

}

void printClusters(P *clusters){
	for(int i = 0; i < K; i++){
		printf("[%d]: %.3f, %.3f\n", i, clusters[i]-> x, clusters[i]-> y);
	}
}
