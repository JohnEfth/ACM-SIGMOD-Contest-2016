#include "../headers/graph_manager.h"

#define THREADS 1

GraphManager::GraphManager()
{
	graph=new Graph();
}

void GraphManager::make_graph(FILE* fp)
{
	uint32_t node1,node2;
	char* token;
	int i;

	if(fp!=NULL){
		char line[20];
		while(fgets(line,sizeof line,fp)!=NULL){
			token = strtok(line," \t");
			i = 0;
			if(strcmp(line,"S") == 0){
				break;
			}
			while(token!=NULL){
				
				if(i == 0){
					node1 = atoi(token);
				}
				else{
					node2 = atoi(token);
				}

				i++;	
				token = strtok(NULL," \t");

			}
			
			graph->insert(node1,node2,0);	
		}
	}
	else{
		cout<<"File for making Graph does not exist"<<endl;
		exit(1);
	}
}

void GraphManager::find_results(FILE* fp_wl)
{
	uint32_t node1,node2;
	char* token = NULL;
	char action,prevAction;
	uint32_t counter=0;	
	dynamic=false;
	int i,results_table_size,version=0,*tempResults,size_resTable_counter=0,task_counter=0, minus_one_count=0;;
	Job the_job;


	JobScheduler JobSc(THREADS,graph);   // Creates the Job Scheduler

	results_table_size=1023;
	JobSc.results=(int*) malloc((results_table_size+1)*sizeof(int));
	
	prevAction='R';

	if(fp_wl!=NULL)
	{
		char line[20];
		fgets(line,sizeof line,fp_wl);
		
		if(strncmp(line,"DYNAMIC",7) == 0)
		{
			dynamic=true;
			
			my_wcc = graph->findWCC();
			

		}
		else
		{
			// Creating Strongly Connected Components and Hypergraph
					
			scc=new SCC(graph->graphSize());
			hyper=new HyperGraph();
			
			graph->tarjan(scc,hyper);          
			
			srand(time(NULL));
			
			grail= (Grail**)malloc (sizeof(Grail*)*GRAILS);

			pthread_t *tids = new pthread_t[GRAILS];
			Parameters *parameters = new Parameters[GRAILS];
			int error;
			

			for(int i=0; i<GRAILS; i++)
			{
				parameters[i].graphManager = this;
				parameters[i].grail_id = i;
				if ((error = pthread_create(&tids[i], NULL,thread_grail,(void *)&parameters[i]))) /* New thread */
				{ 
			        printf("pthread_create , error: %d", error); 
			        exit(1); 
			    }
			    
			}

			for (int i = 0; i < GRAILS; ++i)
			{
				pthread_join(tids[i],0);
				
			}

			delete [] tids;
			delete [] parameters;

			JobSc.initialize_grail_scc(grail[0],scc);
		}

		while(fgets(line,sizeof line,fp_wl)!=NULL)
		{
			token = strtok(line," \t");
			i = 0;
			
			while(token!=NULL)
			{
				if(i==0){
					action = token[0];
				}
				else if(i == 1){
					node1 = atoi(token);
				}
				else{
					node2 = atoi(token);
				}

				i++;	
				token = strtok(NULL," \t");

			}
			
			
			if(action == 'A')
			{
				if(prevAction == 'Q')
				{
					version++;
				}
				prevAction='A';

				if(graph->insert(node1,node2,version)!=-1)
				{
					
					if(my_wcc->insertNewEdge(node1,node2) == -1)
					{
						counter++;
					}
					
				}
			}
			else if(action == 'Q')
			{
				

				prevAction='Q';

				if(size_resTable_counter > results_table_size)   // If we need a bigger table  I realloc
				{
					
					results_table_size=(results_table_size+1)<<1;   // Multiple by 2

					tempResults = (int*)realloc(JobSc.results,(results_table_size)*sizeof(int));
			        if(tempResults!=NULL)
			        {
			            JobSc.results= tempResults;

			            results_table_size--;  // I do minus 1 because the table starts from 0 
			        }
			        else
			        {
			            free(JobSc.results);
			            cerr<<"Error reallocating memory"<<endl;
			            exit(1);
			        }
				}

				if(dynamic == false)
				{
					
					if( Grailquestions(scc->getIdBelongsToComponent(node1),scc->getIdBelongsToComponent(node2)) )  // Maybe
					{
						
						the_job.from=node1;
						the_job.to=node2;
						the_job.result=size_resTable_counter;
						the_job.version=version;
						the_job.dynamic=dynamic;

						JobSc.submit_job(the_job);
						task_counter++;
					}
					else // No
					{
						minus_one_count++;
						JobSc.results[size_resTable_counter]=-1;
					}
						
				}
				else
				{
					if(my_wcc->question(node1,node2) == 1)
					{

						the_job.from=node1;
						the_job.to=node2;
						the_job.result=size_resTable_counter;
						the_job.version=version;
						the_job.dynamic=dynamic;

						JobSc.submit_job(the_job);
						task_counter++;
					}
					else
					{
						JobSc.results[size_resTable_counter]=-1;
					}
				}

				size_resTable_counter++;
			}
			else
			{
				
				JobSc.all_tasks=task_counter;
				JobSc.execute_all_jobs();       // Start executing all Jobs
				

				for (i = 0; i < size_resTable_counter; ++i)
				{
					cout<<JobSc.results[i]<<endl;
				}

				size_resTable_counter=0;
				task_counter=0;
				JobSc.tasks_done=0;
			}
		}
	}
	
	free(JobSc.results);
}

bool GraphManager::Grailquestions(uint32_t comp1,uint32_t comp2)
{
	bool answer = true;
	for(int i=0; i<GRAILS; i++)
	{
		answer=grail[i]->question(comp1,comp2);
		if(answer == false)
		{
			break;
		}
	}
	return answer;
}

GraphManager::~GraphManager()
{
	
	if(dynamic == false)
	{
		for(int i=0; i<GRAILS; i++)
		{
			delete(grail[i]);
		}
		delete(hyper);
		delete(scc);	
		free(grail);
	}
	else
	{
		delete(my_wcc);
	}

	delete(graph);
}

void *GraphManager::thread_grail(void *params)
{

	Parameters *parameters = (Parameters *)params;
	int k = parameters->grail_id;

	GraphManager* GraphMan= parameters->graphManager;
	GraphMan->make_grail(k);
}

void GraphManager::make_grail(int k){
	
	
	grail[k]= new Grail(scc->getComponentsCount());   // Making Grail Index
	grail[k]->createindex(hyper , k);
}

