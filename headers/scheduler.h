#include <pthread.h> 
#include <cinttypes>
#include "graph.h"
#include "GenericDLList.h"

class Job{
public:
	uint32_t from;
	uint32_t to;
	int version;   
	int result;
	bool dynamic;
};


class JobScheduler{

private:
	int execution_threads; 		  // number of execution threads
	GenericDLList <Job> queue; 	  // queue with jobs/tasks
	pthread_t* tids;			  // Table with threads
	pthread_mutex_t queue_mtx;    // For Locking the Queue
	// pthread_mutex_t done_mtx;     // For Locking the tasks_done

	pthread_cond_t empty_queue;   // Threads will wait here
	pthread_cond_t done_cond;     // graph_manager will wait here
	bool available;
	bool finished;

	// void *parameters;

	Graph* graph;
	SCC* scc;
	Grail* grail;
	iSearch* isearch_table;

public:
	int* results;
	int tasks_done;              // When the tasks are done then graph_manager can write to the file
	int all_tasks;

	JobScheduler(int et,Graph* giveGraph);
	static void *thread_guru(void *argp);
	void submit_job(Job& j);
	void execute_all_jobs();
	void gurufunc(int thread_id);

	void initialize_grail_scc(Grail* giveGrail,SCC* giveScc);

	~JobScheduler();


};

class threadParameters{
	public:
		JobScheduler *Schd;
		int thread_id;
};
