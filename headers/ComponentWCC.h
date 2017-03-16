#include <unistd.h>
#include <iostream>
#include <climits>
#include <stdio.h>
#include <string.h>

class ComponentWCC{
	private:
		uint32_t component_id;
		uint32_t included_nodes_count;
		uint32_t* included_nodes_ids;
		uint32_t size;
		
	public:
		void add_node(uint32_t nodeId);
		void print_ids();
		uint32_t get_included_nodes_count(){return this->included_nodes_count;}
		uint32_t get_included_id(uint32_t id){return this->included_nodes_ids[id];}
		void init(uint32_t comp_id);
		void delete_component();
		ComponentWCC(uint32_t id,uint32_t num_of_nodes);
		~ComponentWCC();
};
