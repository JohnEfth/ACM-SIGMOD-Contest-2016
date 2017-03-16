#include "../headers/buffer.h"
#include "../headers/index.h"
#include "../headers/GenericDLList.h"
#include "../headers/SCC.h"

class HyperGraph{
	buffer outbuf;
	Index  out;
	uint32_t edges;

public:
	HyperGraph(){edges = 0;}
	void insert(uint32_t, uint32_t);
	void printGraph();
	bool edgeExists(uint32_t nodeId1,uint32_t nodeId2);
	uint32_t gimmecursizeout(){return out.getCurrentSize();};
	uint32_t getEdges(){return edges;}

	bool isUsed(uint32_t nodeId){return out.isUsed(nodeId);}; // I look at the incoming Index just for the Grail
	uint32_t getallneis(uint32_t nodeId){return out.getallnei(nodeId);}; // For Grail
	int getlhead(uint32_t nodeId){return out.getListHead(nodeId);}; // For Grail to get the head of the neightboors
	list_node* gimmelnode(uint32_t nodeId){return outbuf.getListNode(nodeId);}; // For Grail to get pointer to the head

};