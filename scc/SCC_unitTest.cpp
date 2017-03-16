#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "../headers/SCC.h"
#include "../headers/graph.h"


using namespace std;

int insertions = 0;

int main(int argc,char **argv){
	
	Graph* graph = new Graph();

	graph->insert(0, 1);
	graph->insert(0, 3);
	graph->insert(1, 2);
	graph->insert(1, 4);
	graph->insert(2, 0);
	graph->insert(2, 6);
	graph->insert(3, 2);
	graph->insert(3, 4);
	graph->insert(4, 5);
	graph->insert(4, 6);
	graph->insert(5, 6);
	graph->insert(5, 7);
	graph->insert(5, 8);
	graph->insert(5, 9);
	graph->insert(6, 4);
	graph->insert(7, 9);
	graph->insert(8, 9);
	graph->insert(9, 8);



	SCC scc(graph->graphSize());
		
	HyperGraph hyper;

	graph->tarjan(scc , hyper);
	int ret;
	ret = graph->estimateShortestPathSCC(scc , 0 , 2);
	assert(ret == 2);

	ret = graph->estimateShortestPathSCC(scc , 1 , 3);
	assert(ret == 3);

	ret = graph->estimateShortestPathSCC(scc , 0 , 4);
	assert(ret == -1);

	ret = graph->estimateShortestPathSCC(scc , 4 , 6);
	assert(ret == 1);
	
	ret = graph->estimateShortestPathSCC(scc , 9 , 8);
	assert(ret == 1);

	ret = graph->estimateShortestPathSCC(scc , 0 , 6);
	assert(ret == -1);

	
	delete(graph);

	return 0;
}	