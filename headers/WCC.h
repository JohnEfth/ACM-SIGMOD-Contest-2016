#include <unistd.h>
#include <iostream>
#include <climits>
#include <stdio.h>
#include <string.h>

#include "../headers/ComponentWCC.h"

class WCC
{	
	private:
		ComponentWCC* components;
		uint32_t components_array_size;	//size of components array. equal to num_of_nodes before dynamic adds
		uint32_t components_count;	//how many CC's exist
		uint32_t* id_belongs_to_component;
		uint32_t id_belongs_size;
		uint32_t* index;
		uint32_t current_index_size;
		uint32_t total_graph_nodes; //total num of nodes of the graph
		uint32_t* islands;
		uint32_t island_counter;
		uint32_t island_table_size;
		uint32_t checking_counter;
	public:
		void assign_compId(uint32_t nodeId,uint32_t	comp_id);	
		void updateCount(){components_count++;return;}	
		void check_components();
		void print_components();
		void init_component(uint32_t compId);	//to init a component
		uint32_t getTotalWcc(){return this->components_count;}
		void check_comps(uint32_t max_index_size);
		void informIndex(uint32_t max_index_size);
		void checkIndex();
		int insertNewEdge(uint32_t nodeId1, uint32_t nodeId2);
		void updateIndex(uint32_t compId1,uint32_t compId2);
		void add_to_components(uint32_t compId,uint32_t nodeId);
		int question(uint32_t node1,uint32_t node2);
		uint32_t get_check_counter(){return checking_counter;}
		void create_islands();
		void destroy_islands();

		WCC(uint32_t num_of_nodes,uint32_t max_index_size);
		~WCC();
};