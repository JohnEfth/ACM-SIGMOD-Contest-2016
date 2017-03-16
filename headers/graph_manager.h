#include "scheduler.h"

#define GRAILS 5



class GraphManager{

	private:
		WCC* my_wcc;
		SCC* scc;     
		HyperGraph* hyper; 
		Grail** grail;
		Graph* graph;
		
		bool dynamic;              // False / True
		bool Grailquestions(uint32_t comp1,uint32_t comp2);
	public:
		GraphManager();
		void make_graph(FILE* fp);
		void find_results(FILE* fp_wl);

		static void *thread_grail(void *params);
		void make_grail(int k);

		~GraphManager();

};


class Parameters{
	public:
		GraphManager *graphManager;
		int grail_id;
};
