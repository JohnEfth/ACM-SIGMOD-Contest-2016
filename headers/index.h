#include <iostream>

#include "../headers/DynHash.h"

#define SIZE 512

using namespace std;


class NodeIndex{
	public:
		int head;
		int tail;
		bool inUse;
		uint32_t allneightboors;
		DynHash LaHash;
		NodeIndex();
		~NodeIndex();
};	

class Index{
		NodeIndex* table;
		uint32_t current_size;	//initially SIZE
		uint32_t num_of_nodes;
	public:
		Index();
		~Index();
		void init_index();	//init table fields
		int insertNode(uint32_t nodeId);
		int getListHead(uint32_t nodeId);
		int getListTail(uint32_t nodeId);
		int getnumofnodes(){return this->num_of_nodes;}
		int setListHead(uint32_t nodeId,int value);
		int setListTail(uint32_t nodeId,int value);
		void setInUseTrue(uint32_t nodeId);	//sets inUse field as "true" after insertion
		void setInUseFalse(uint32_t nodeId);
		bool isUsed(uint32_t nodeId);	//checks if this node is used
		uint32_t getCurrentSize(){return current_size;};
		void print_values();
		void setHashTable(uint32_t nodeId1,uint32_t ptr,uint32_t cell);
		uint32_t getHashTable(uint32_t nodeId,uint32_t cell);
		uint32_t hashFunction(uint32_t nodeId1,uint32_t nodeId2);
		uint32_t getHashTableSize(uint32_t nodeId){return table[nodeId].LaHash.getDtableSize();};
		void duplicateHashTable(uint32_t nodeId){table[nodeId].LaHash.duplicatetable();};
		void createHashTable(uint32_t nodeId);
		void plusplusneightboors(uint32_t nodeId){ if(nodeId < current_size) table[nodeId].allneightboors++; }
		uint32_t getallnei(uint32_t nodeId){ return table[nodeId].allneightboors;}	
};





//NodeIndex* createNodeIndex();