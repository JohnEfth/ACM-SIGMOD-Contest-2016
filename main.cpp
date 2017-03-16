#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>

#include "headers/graph_manager.h"

using namespace std;

int insertions = 0;

int main(int argc,char **argv){
	FILE *fp,*fconfig;
	char dataset[50],workload[50];
	GraphManager graph_manager;

	fconfig=fopen("config.txt","r");
	fscanf(fconfig,"%s",dataset);
	fscanf(fconfig,"%s",workload);
	
	fp = fopen(dataset,"r");
	
	
	graph_manager.make_graph(fp);

	
	fclose(fp);

	FILE *fp_wl;
	fp_wl = fopen(workload,"r");

	graph_manager.find_results(fp_wl);

		
		
		
		
	fclose(fp_wl);
	fclose(fconfig);
		
	
	return 0;
}
