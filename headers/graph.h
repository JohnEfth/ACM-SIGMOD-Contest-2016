#include "../headers/grail.h"
#include "../headers/iSearch.h"
#include "../headers/WCC.h"


class Graph
{
	private:
		buffer inbuf;
		buffer outbuf;
		Index in;
		Index out;
		uint32_t total_nodes;
		void update(uint32_t position,char* table);  // Updates the tables that hold the nodes that are visited
		bool look(uint32_t position,char* table);   // Looks to a table if the requested node is visited
	public:
		Graph();
		uint32_t gimmecursizeout(){return out.getCurrentSize();};
		uint32_t gimmecursizein(){return in.getCurrentSize();};
		int grailsearch(uint32_t from, uint32_t to,Grail* grail,SCC* scc,iSearch& isearch);
		int search(uint32_t from, uint32_t to,int current_version,iSearch& isearch);
		int insert(uint32_t, uint32_t,int);
		void printGraph();
		bool edgeExists(uint32_t nodeId1,uint32_t nodeId2);

		void tarjan(SCC* scc , HyperGraph* hyperGraph);
		uint32_t getnextnei(uint32_t currentnode,uint32_t* neis, list_node** listnodeptrs);
		uint32_t graphSize(){return out.getCurrentSize();}
		int estimateShortestPathSCC(SCC* scc , uint32_t from , uint32_t to,iSearch& isearch);

		uint32_t getTotalNodes(){return total_nodes;}	//returns the no. of discrete graph nodes 
		void ask_exist(uint32_t nodeId);
		WCC* findWCC();

		
		~Graph();
};

