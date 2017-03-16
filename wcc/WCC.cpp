#include "../headers/WCC.h"

using namespace std;


WCC::WCC(uint32_t num_of_nodes,uint32_t max_index_size){	
	id_belongs_to_component = (uint32_t*)malloc(max_index_size*sizeof(uint32_t));
	memset(id_belongs_to_component,UINT32_MAX,sizeof(uint32_t)*max_index_size);
	id_belongs_size = max_index_size;

	index = (uint32_t*)malloc(max_index_size*sizeof(uint32_t));
	current_index_size = max_index_size;

	components_array_size = 8192;
	components =(ComponentWCC*) malloc(components_array_size*sizeof(ComponentWCC));	
	

	components_count = 0;
	
	total_graph_nodes = num_of_nodes;
	checking_counter =0;
}

WCC::~WCC(){

	free(id_belongs_to_component);
	free(index);
	uint32_t i;

	for(i=0; i<components_count; i++){
		components[i].delete_component();
	}
	free(components);
	destroy_islands();
}

//check for realloc at components array
void WCC::check_components(){
	ComponentWCC* temp_array;
	if(components_count >= components_array_size){
		temp_array = (ComponentWCC*)realloc(components,(2*components_array_size*sizeof(ComponentWCC)));
		
		if(temp_array!=NULL){
			components = temp_array;
			components_array_size = 2*components_array_size;
		}
		else{
			cout<<"Error occured during realloc"<<endl;
			exit(1);
		}
	}
	return;
}


void WCC::add_to_components(uint32_t comp_id,uint32_t nodeId){
	check_components();
	components[comp_id].add_node(nodeId);	
	return;
}
void WCC::init_component(uint32_t compId){
	check_components();
	components[compId].init(compId);
	return;
}


void WCC::checkIndex(){
	uint32_t i;
	for(i=0; i<current_index_size; i++){
		if(index[i]!=UINT32_MAX){
			cout<<"id "<<i<<" comp: "<<index[i]<<endl;
		}
	}
	return;
}
void WCC::informIndex(uint32_t max_index_size){
	memcpy(index,id_belongs_to_component,max_index_size*sizeof(uint32_t));
	return;
}

void WCC::print_components(){
	uint32_t i;
	for(i=0; i<components_count; i++){
		cout<<"CC: "<<i<<" has nodes:"<<components[i].get_included_nodes_count()<<endl;
		components[i].print_ids();
	}
	
	return;	
}


void WCC::check_comps(uint32_t maxnum){
	uint32_t i;
	for(i=0; i<maxnum; i++){
		if(id_belongs_to_component[i]!=UINT32_MAX){	//for every existing node
			cout<<"Node: "<<i<<" CompID: "<<id_belongs_to_component[i]<<endl;
		}
	}
	return;
}


int WCC::insertNewEdge(uint32_t nodeId1,uint32_t nodeId2)
{
	uint32_t i,compId1,compId2,multiplier,who_reallocs;
	int flag = 0;
	uint32_t* temp_array;
	uint32_t newsize;

	if(!(nodeId1< id_belongs_size && nodeId2<id_belongs_size)) {	
		
		if(nodeId1 > id_belongs_size){
			flag++;
			who_reallocs = nodeId1;
		}
		if(nodeId2 > id_belongs_size){
			flag++;
			who_reallocs = nodeId2;
		}
		if(flag == 2){	                       // Both wants realloc so I need the max of them
			if(nodeId1>nodeId2){
				who_reallocs = nodeId1;
			}
			else{
				who_reallocs = nodeId2;
			}
		}
		multiplier = 1;
		while(who_reallocs > (id_belongs_size<<multiplier)-1){
			multiplier +=1;
		}	
		temp_array = (uint32_t*)realloc(id_belongs_to_component,(id_belongs_size<<multiplier)*sizeof(uint32_t));
		
		if(temp_array!=NULL){
			id_belongs_to_component = temp_array;
			
		}
		else{
			cout<<"Error occured during realloc"<<endl;
			exit(1);
		}
		for(i=id_belongs_size; i< id_belongs_size<<multiplier; i++){
			id_belongs_to_component[i] = UINT32_MAX;
		}
		id_belongs_size = id_belongs_size<<multiplier;
	}	


	compId1=id_belongs_to_component[nodeId1];
	compId2=id_belongs_to_component[nodeId2];


	if(compId1 == UINT32_MAX && compId2 == UINT32_MAX)
	{
		checking_counter++;
		total_graph_nodes+=2;
		
		check_components();

		init_component(components_count);
		add_to_components(components_count,nodeId1);
		add_to_components(components_count,nodeId2);
		
		assign_compId(nodeId1,components_count);
		assign_compId(nodeId2,components_count);

		if(components_count >= island_table_size)   // We check if the islands table needs realloc because of new component creation
		{
		    newsize=island_table_size<<1;
			temp_array = (uint32_t*)realloc(islands,(newsize)*sizeof(uint32_t));
			
			if(temp_array!=NULL){
				islands = temp_array;
			}
			else{
				cout<<"Error occured during realloc"<<endl;
				exit(1);
			}
			for(i=island_table_size; i< newsize; i++){
				islands[i] = UINT32_MAX;
			}
			island_table_size=newsize;
		}

		updateCount();

	}
	else if(compId1 != UINT32_MAX && compId2!=UINT32_MAX)	//o node1 anhksei se component kai o node2 to idio
	{
		if(compId1 == compId2)
			return 0;

		if(islands[compId1] != islands[compId2])
		{
			if(islands[compId1] == UINT32_MAX)     // Check if one of them doesn't belong to an island already
			{
				checking_counter++;
				islands[compId1]=islands[compId2];
			}
			else if(islands[compId2] == UINT32_MAX)
			{
				checking_counter++;
				islands[compId2]=islands[compId1];
			}
			else  									// Worst case --> I have to connect the two islands
			{
				checking_counter++;
				island_counter++;
				uint32_t isl1,isl2;
				isl1 = islands[compId1];
				isl2 = islands[compId2];

				for(uint32_t i=0; i<island_table_size; i++)
				{
					if(islands[i] == isl1 || islands[i] == isl2)
					{
						islands[i]= island_counter;
					}
				}
				return -1;
			}
		}
		else if(islands[compId1] == UINT32_MAX && islands[compId2] == UINT32_MAX)   // Here I create a new island
		{
			checking_counter++;
			islands[compId1]=island_counter;
			islands[compId2]=island_counter;
			island_counter++;
		}
	}
	else if(compId1 != UINT32_MAX && compId2==UINT32_MAX) 
	{
		checking_counter++;
		assign_compId(nodeId2,compId1);
		add_to_components(compId1,nodeId2);
	}
	else if(compId1 == UINT32_MAX && compId2!=UINT32_MAX)
	{	
		checking_counter++;
		assign_compId(nodeId1,compId2);
		add_to_components(compId2,nodeId1);
	}
	

	return 0;
}



//assigns the index values of nodes that belong to compId2 = compId1
void WCC::updateIndex(uint32_t compId1,uint32_t compId2){
	uint32_t i;
	
	for(i=0; i<current_index_size; i++){
		if(index[i] == compId2){
			index[i] = compId1;
		}
	}
	
	return;
}

void WCC::assign_compId(uint32_t nodeId,uint32_t comp_id){
	uint32_t* temp_array;
	if(nodeId>=id_belongs_size){
		//need to realloc the id_belongs to component
		uint32_t multiplier = 1;
		while(nodeId>(id_belongs_size<<multiplier)-1){
			multiplier +=1;
		}
		temp_array = (uint32_t*)realloc(id_belongs_to_component,(id_belongs_size<<multiplier)*sizeof(uint32_t));
		if(temp_array!=NULL){
			id_belongs_to_component = temp_array;
		}
		else{
			cout<<"Error reallocing in assign_compId"<<endl;
			exit(1);
		}

		for(uint32_t i= id_belongs_size; i< (id_belongs_size<<multiplier); i++){
			id_belongs_to_component[i] = UINT32_MAX;
		}

		id_belongs_size = id_belongs_size<<multiplier;

	}
	id_belongs_to_component[nodeId] = comp_id;
	return;
}

void WCC::create_islands()
{
	islands=(uint32_t*) malloc(components_count*sizeof(uint32_t));  // Create and Initialize islands
	memset(islands,UINT32_MAX,sizeof(uint32_t)*components_count);
	island_counter=0;
	island_table_size=components_count;
	return;
}

void WCC::destroy_islands()
{
	if(island_table_size > 0)
		free(islands);
}

int WCC::question(uint32_t nodeId1,uint32_t nodeId2){
	uint32_t compId1,compId2;
	compId1 = id_belongs_to_component[nodeId1];
	compId2 = id_belongs_to_component[nodeId2];
	if(compId1 != UINT32_MAX && compId2 != UINT32_MAX)
	{	
		if(compId1 == compId2 || (islands[compId1] == islands[compId2])) {
		return 1;
		}
	}

	return 0;
}