#include "../headers/HyperGraph.h"

class GrailIndex
{	
	private:
		uint32_t min_rank;
		uint32_t rank;

	public:
		GrailIndex();
		void setrank (uint32_t givenmin,uint32_t givenrank ); // I set the rank and the min_rank of this node
		uint32_t getrank(){return this->rank;}
		uint32_t getmin_rank(){return this->min_rank;}
		~GrailIndex();
};

class Grail
{
	private:
		GrailIndex* table;
		uint32_t table_size;
		// uint32_t *virtualnodes;
		
		void update(uint32_t position,char* table);
		bool look(uint32_t position,char* table);
		uint32_t getnextnei(uint32_t currentnode,HyperGraph* thegraph,uint32_t *neis,list_node** listnodeptrs);
		uint32_t getNextVirtualNode(int &grail_id , uint32_t &virtual_ptr);

	public:
		Grail(uint32_t size);
		void createindex(HyperGraph* thegraph ,  int grail_id);
		void printgrail(char* filename);
		bool question(uint32_t comp1,uint32_t comp2);
		~Grail();

};

