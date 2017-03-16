#include "../headers/scheduler.h"


JobScheduler::JobScheduler(int et,Graph* giveGraph):execution_threads(et) , graph(giveGraph)
{
	int error;
	finished = false;

	pthread_mutex_init(&queue_mtx,NULL);
	pthread_cond_init(&empty_queue,NULL);
	pthread_cond_init(&done_cond,NULL);


	tids=new pthread_t[execution_threads];     //Initialize space for the threads' ids 
	threadParameters *threadParams= NULL;

	isearch_table= new iSearch[execution_threads];

	for(int i=0; i<execution_threads; i++)
	{	
		threadParams = new threadParameters();
		threadParams->Schd= this;
		threadParams->thread_id = i;
		if ((error = pthread_create(&tids[i], NULL,thread_guru,(void *)threadParams))) /* New thread */
		{ 
	        printf("pthread_create , error: %d", error); 
	        exit(1); 
	    }
	    threadParams = NULL;
	    
	}

	

	
	scc=NULL;
	grail=NULL;
	tasks_done=0;

}

void JobScheduler::submit_job(Job& j)
{

  	queue.pushBack(j);  // Put the job at the end of the queue
 	
}

void JobScheduler::execute_all_jobs()
{
	int error;

	if((error = pthread_mutex_lock(&queue_mtx))) 	// Lock Queue Mutex in order to take a job
	{ 
        printf("pthread_mutex_lock , error: %d", error); 
        printf("IN MAIN \n");
    	exit(1); 
	}
    
    available=true;
    
    pthread_cond_broadcast(&empty_queue);

    while(tasks_done < all_tasks)  // Checking if tasks are done
    {
    	pthread_cond_wait(&done_cond,&queue_mtx);
    }
    
    if ((error = pthread_mutex_unlock(&queue_mtx))) /* I release the mutex */
	{ 
    	printf("pthread_mutex_unlock , error: %d", error); 
    	exit(1); 
	}
}

void *JobScheduler::thread_guru(void *params)
{

	threadParameters *parameters = (threadParameters *)params;
	int k = parameters->thread_id;

	JobScheduler* JobSched=parameters->Schd;
	
	delete parameters;
	
	JobSched->gurufunc(k);

	
}

void JobScheduler::gurufunc(int thread_id)
{
	int error;
	Job my_job;

	while(finished != true)
	{
		if((error = pthread_mutex_lock(&queue_mtx))) 	// Lock Queue Mutex in order to take a job
		{ 
	        printf("pthread_mutex_lock , error: %d", error); 
	        printf("IN THREAD %ld for GET Job\n",pthread_self() );
	    	exit(1); 
		}
	    
	    while((queue.getListSize() == 0) || (available == 0))  // Checking if the Queue has Jobs
	    {
	    	
	    	if(finished == true)
	    	{
	    		if ((error = pthread_mutex_unlock(&queue_mtx))) /* I release the mutex */
				{ 
			    	printf("pthread_mutex_unlock , error: %d", error); 
			    	exit(1); 
				}

				pthread_exit(NULL);
	    	}

	    	pthread_cond_wait(&empty_queue,&queue_mtx);	
	    }
	    
	    my_job=queue.popFront();
	    
	    if(queue.getListSize() == 0)
	    {
	    	available=false;
	    }

	    if ((error = pthread_mutex_unlock(&queue_mtx))) /* I release the mutex */
		{ 
	    	printf("pthread_mutex_unlock , error: %d", error); 
	    	exit(1); 
		}
	    
		if(my_job.dynamic == false)
		{
			if(scc->getIdBelongsToComponent(my_job.from) == scc->getIdBelongsToComponent(my_job.to)) // If they belong to the same SCC
			{
				results[my_job.result]=graph->estimateShortestPathSCC(scc,my_job.from,my_job.to , isearch_table[thread_id]);
				
				if(results[my_job.result] == -1)
				{
					cout<<"ERROR we got -1 from estimate";
					cout<<" "<<my_job.from<<" -> "<<my_job.to<<endl;
				}
			}
			else
			{			
				results[my_job.result]=graph->grailsearch(my_job.from,my_job.to,grail,scc , isearch_table[thread_id]);
			}
		}
		else
		{
			results[my_job.result]=graph->search(my_job.from,my_job.to,my_job.version , isearch_table[thread_id]);
		}
		
		if(( error = pthread_mutex_lock(&queue_mtx) )) 	// Lock Tasks Mutex
		{ 
	        printf("pthread_mutex_lock , error: %d", error);
	        printf("IN THREAD %ld for task\n",pthread_self() ); 
	    	exit(1); 
		}
	    
	    tasks_done++;                          // +1 One to the tasks done 

	    
	    pthread_cond_broadcast(&done_cond);    // Broadcast for the Graph Manager that waits

	    pthread_cond_broadcast(&empty_queue);

	    if (( error = pthread_mutex_unlock(&queue_mtx) )) /* Release the mutex */
		{ 
	    	printf("pthread_mutex_unlock , error: %d", error); 
	    	exit(1); 
		}
	}
	pthread_exit(NULL);
}



void JobScheduler::initialize_grail_scc(Grail* giveGrail,SCC* giveScc)
{
	grail=giveGrail;
	scc=giveScc;
}

JobScheduler::~JobScheduler()
{
	int error;
	if(( error = pthread_mutex_lock(&queue_mtx) )) 	// Lock Tasks Mutex
	{ 
        printf("pthread_mutex_lock , error: %d", error); 
    	exit(1); 
	}
    
   	finished=true;

    pthread_cond_broadcast(&empty_queue);

    if (( error = pthread_mutex_unlock(&queue_mtx) )) /* Release the mutex */
	{ 
    	printf("pthread_mutex_unlock , error: %d", error); 
    	printf("IN Destructor \n");
    	exit(1); 
	}

	for (int i = 0; i < execution_threads; ++i)
	{
		pthread_join(tids[i],0);
	}

	pthread_mutex_destroy(&queue_mtx);
	pthread_cond_destroy(&empty_queue);
	
	pthread_cond_destroy(&done_cond);

	delete [] tids;
	delete [] isearch_table;


}