#include "../headers/ComponentWCC.h"


using namespace std;


ComponentWCC::ComponentWCC(uint32_t comp_id,uint32_t num_of_nodes){	//max id's in a component = num_of_nodes, that means there is one Wcc
	component_id = comp_id;
	included_nodes_count = 0;
	size = 32;
	included_nodes_ids = (uint32_t*)malloc(32*sizeof(uint32_t));
	
}
ComponentWCC::~ComponentWCC(){
	if(included_nodes_ids!=NULL){
		free(included_nodes_ids);
	}
	included_nodes_ids = NULL;
	
}


void ComponentWCC::add_node(uint32_t nodeId){	//Adds node with id nodeId in first unused position of the component
	uint32_t* temp_array;
	
	if(included_nodes_count<size){
		included_nodes_ids[included_nodes_count] = nodeId;
		included_nodes_count++;	
		return;
	}
	else{
		temp_array = (uint32_t*)realloc(included_nodes_ids,2*size*(sizeof(uint32_t)));
		if(temp_array!=NULL){
			included_nodes_ids = temp_array;
			size = 2*size;
			included_nodes_ids[included_nodes_count] = nodeId;
			included_nodes_count++;
			return;		
		}
		else{
			cout<<"Error reallocing in add_node()"<<endl;
			exit(1);
		}
		// cout<<"included_nodes_count: "<<included_nodes_count<<" size: "<<size<<" comp Id: "<<component_id<<endl;
		// cout<<"something goes wrong! "<<nodeId<<endl;
		exit(1);
	}	
	return;
}

void ComponentWCC::init(uint32_t comp_id){
	component_id = comp_id;
	included_nodes_count = 0;
	size = 2;
	included_nodes_ids = (uint32_t*)malloc(size*sizeof(uint32_t));
	return;
}

void ComponentWCC::print_ids(){
	uint32_t i;
	cout<<"--CC--"<<endl;
	for(i=0; i<included_nodes_count; i++){
		cout<<"node: "<<included_nodes_ids[i]<<endl;
	}
	return;
}

void ComponentWCC::delete_component()
{
	if(included_nodes_ids!=NULL)
		free(included_nodes_ids);
	included_nodes_ids = NULL;
}