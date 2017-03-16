#include "../headers/iSearch.h"



using namespace std;

SearchNode::SearchNode(int id,int lvl): id(id), level(lvl)
{
	// this->next=NULL;
}

void SearchNode::print()
{
	cout<<"ID: "<<this->id<<" , Level: "<<this->level<<"\n";
}


iSearch::iSearch()
{
	vfrom=NULL;
	vto=NULL;
	rfrom=(SearchNode*) malloc(ROW_SIZE*sizeof(SearchNode));
	rto=(SearchNode*) malloc(ROW_SIZE*sizeof(SearchNode));

	head_rfrom=0;
	tail_rfrom=0;
	head_rto=0;
	tail_rto=0;

	current_size_from=ROW_SIZE;
	current_size_to=ROW_SIZE;
	current_size_visited=0;

	version=0;
}

void iSearch::initialize(uint32_t graph_size)
{
	uint32_t* temp=NULL;
	head_rfrom=0;
	tail_rfrom=0;
	head_rto=0;
	tail_rto=0;

	version++;

	if(current_size_visited < graph_size)
	{
		current_size_visited = graph_size;

		temp=(uint32_t*) realloc(vfrom,(current_size_visited)*sizeof(uint32_t));
		if(temp!=NULL)
       		vfrom = temp;
        else{
            free(vfrom);
            cerr<<"Error reallocating memory"<<endl;
            exit(1);
        }
        temp=(uint32_t*) realloc(vto,(current_size_visited)*sizeof(uint32_t));
		if(temp!=NULL)
       		vto = temp;
        else{
            free(vto);
            cerr<<"Error reallocating memory"<<endl;
            exit(1);
        }

    	memset(vfrom,0,current_size_visited*sizeof(uint32_t));  // Initialize the visited
		memset(vto,0,current_size_visited*sizeof(uint32_t));
        
	}
}

void iSearch::add(uint32_t id,int level,bool from)
{
	SearchNode* temp;

	if(from)
	{
		if((tail_rfrom+1) == current_size_from)
		{
			current_size_from <<= 1;

			temp=(SearchNode*) realloc(rfrom,(current_size_from)*sizeof(SearchNode));
			if(temp!=NULL)
           		rfrom = temp;
	        else{
	            free(rfrom);
	            cerr<<"Error reallocating memory"<<endl;
	            exit(1);
	        }
		}
		rfrom[tail_rfrom].set_id(id);
		rfrom[tail_rfrom].set_level(level);
		tail_rfrom++;
	}
	else
	{
		if((tail_rto+1) == current_size_to)
		{
			current_size_to <<= 1;

			temp=(SearchNode*) realloc(rto,(current_size_to)*sizeof(SearchNode));
			if(temp!=NULL)
           		rto = temp;
	        else{
	            free(rto);
	            cerr<<"Error reallocating memory"<<endl;
	            exit(1);
	        }
		}
		rto[tail_rto].set_id(id);
		rto[tail_rto].set_level(level);
		tail_rto++;
	}
}
		

void iSearch::remove(bool from)
{
	if(from)
	{
		if(head_rfrom != tail_rfrom)
		{
			head_rfrom++;
		}
	}
	else
	{
		if(head_rto != tail_rto)
		{
			head_rto++;
		}
	}
}

uint32_t iSearch::get_head_id(bool from)
{
	if(from)
	{
		return rfrom[head_rfrom].getid();
	}
	else
	{
		return rto[head_rto].getid();
	}
}
int iSearch::get_head_lvl(bool from)
{
	if(from)
	{
		return rfrom[head_rfrom].getlevel();
	}
	else
	{
		return rto[head_rto].getlevel();
	}
}

void iSearch::update(uint32_t position,bool from)
{
	if(from)
	{
		vfrom[position]=version;
	}
	else
	{
		vto[position]=version;
	}
}

bool iSearch::isEmpty(bool from)
{
	if(from)
	{
		if(head_rfrom == tail_rfrom)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(head_rto == tail_rto)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool iSearch::look(uint32_t position, bool from)
{
	if(from)
	{
		if(vfrom[position] == version)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(vto[position] == version)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

iSearch::~iSearch()
{
	free(vfrom);
	free(vto);
	free(rfrom);
	free(rto);

}