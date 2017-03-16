#include <stdlib.h>
#include <string.h>
#include <climits>

#include "../headers/grail.h"

Grail::Grail(uint32_t size)
{
	this->table_size=size;
	this->table=(GrailIndex*) malloc(size*sizeof(GrailIndex));
}

void Grail::printgrail(char* filename)
{
	FILE* fp=fopen(filename,"w");
	for(uint32_t i=0; i<table_size; i++)
	{
		fprintf(fp,"Node: %d ( %d , %d )\n",i,table[i].getmin_rank(),table[i].getrank());
	}
}

uint32_t Grail::getNextVirtualNode(int &grail_id , uint32_t &virtual_ptr){
	if(grail_id % 5 == 0)
		return virtual_ptr;

	else if(grail_id % 5 == 1)
		return (table_size-1) - virtual_ptr;

	else if(grail_id % 5 == 2){
		if(virtual_ptr < (table_size/2) )
			return ((table_size/2 -1) - virtual_ptr);
		else
			return virtual_ptr;
		
	}
	else if(grail_id % 5 == 3){
		if(virtual_ptr < (table_size/2) )
			return virtual_ptr;
		else
			return ((table_size - 1) - (virtual_ptr - table_size/2) );

	}
	else{    // (grail_id & 5 == 4)
		if(virtual_ptr < (table_size/2) )
			return virtual_ptr + (table_size/2);
		else{
			if((virtual_ptr - (table_size/2)) >= ( table_size/2 ) )
				return virtual_ptr;
			else 
				return virtual_ptr - (table_size/2);	 
		} 

	}

}

void Grail::createindex(HyperGraph* thegraph , int grail_id)
{
	uint32_t *neis,*prev,*mins;
	char *visited;
	uint32_t unvisitpoint;  // Pointer to the unvisited nodes , it will help me to go through the nodes' list only once
	uint32_t globalrank=1;   // Counter for the rank
	uint32_t currentnode,nextnei,tempmin,virtual_ptr; 
	list_node **listnodeptrs; 

	virtual_ptr = 0;
	
	unvisitpoint = getNextVirtualNode(grail_id , virtual_ptr);

	neis=(uint32_t*) malloc(table_size*sizeof(uint32_t));   // I will keep in which neightboor I am in every node
	visited=(char*) malloc(table_size*sizeof(char));      	// I will keep the visited nodes
	prev=(uint32_t*) malloc(table_size*sizeof(uint32_t));   // I will keep each node its previous	
	mins=(uint32_t*) malloc(table_size*sizeof(uint32_t));	// I will keep for every node the minimum rank
	listnodeptrs=(list_node**) malloc(table_size*sizeof(list_node*));	// I will keep pointers to the current list_node

	memset(neis,0,sizeof(uint32_t)*table_size);  // Initialize neightboors to 0
	memset(visited,0,table_size);   // Initialize visited to 0
	memset(mins,0,sizeof(uint32_t)*table_size);   // Initialize visited to 0

	while(virtual_ptr < table_size)
	{
		prev[unvisitpoint]=unvisitpoint; // It doesn't have any previous

		currentnode=unvisitpoint;

		nextnei=getnextnei(currentnode,thegraph,neis,listnodeptrs);
		
		while((nextnei != currentnode) || (currentnode != prev[currentnode]))   // If the node has no neightboors and is the root
		{		
			
			if(nextnei == currentnode)
			{
				if(mins[currentnode] == 0) 							// If the node has no neightboors it will have mins = 0 
				{
					table[currentnode].setrank(globalrank,globalrank);   // Set the Index
					tempmin=globalrank;
				}
				else
				{
					table[currentnode].setrank(mins[currentnode],globalrank); // Set the Index
					tempmin=mins[currentnode];
				}

				if(mins[prev[currentnode]] == 0)   // Update the min_rank of the previous node
				{
					mins[prev[currentnode]] = tempmin;
				}
				else if( mins[prev[currentnode]] > tempmin)
				{
					mins[prev[currentnode]] = tempmin;
				}

				update(currentnode,visited); // This node now is visited
				globalrank++;
				currentnode=prev[currentnode];
				nextnei=getnextnei(currentnode,thegraph,neis,listnodeptrs);
				
				continue;
			}
			else if(look(nextnei,visited)) // If the next node is visited
			{
				if(mins[currentnode] == 0)
				{
					mins[currentnode]=table[nextnei].getmin_rank();
				}
				else if(mins[currentnode] > table[nextnei].getmin_rank())
				{
					mins[currentnode]=table[nextnei].getmin_rank();					
				}

				nextnei=getnextnei(currentnode,thegraph,neis,listnodeptrs);
				
				continue;   // I continue to the next neightboor this path is searched
			}

			prev[nextnei]=currentnode;
			currentnode=nextnei;							// I move to the next node
			nextnei=getnextnei(currentnode,thegraph,neis,listnodeptrs); 
			
 		}

 		// For the root
 		if(mins[currentnode] == 0) 
 		{
			table[currentnode].setrank(globalrank,globalrank);   // Set the Index
 		}
		else
		{
			table[currentnode].setrank(mins[currentnode],globalrank); // Set the Index
		}
		update(currentnode,visited); // This node now is visited
		globalrank++;

		do // move to the next unvisited node
		{
			virtual_ptr++;
			if(virtual_ptr >= table_size)    // If I pass the table's size I get out
				break;
			unvisitpoint = getNextVirtualNode(grail_id , virtual_ptr);
			
		}while (look(unvisitpoint,visited) != 0);
	}


	free(listnodeptrs);
	free(prev);
	free(mins);
	free(visited);
	free(neis);
}

Grail::~Grail()
{
	free(table);
	
}

void GrailIndex::setrank (uint32_t givenmin,uint32_t givenrank )
{
	this->min_rank=givenmin;
	this->rank=givenrank;
}

void Grail::update(uint32_t position,char* table)
{
	int byte,bit;
	char mask;

	byte=position/CHAR_BIT;
	bit=position%CHAR_BIT;

	mask=0x01;          // I initialize the mask as 0000 0001 
	mask=mask<<bit;		// I move the 1 to the approrpiate position

	table[byte]= table[byte] | mask;   // I do OR in order to make the 0 -> 1

}
bool Grail::look(uint32_t position,char* table)
{
	int byte,bit;
	char mask,res;

	byte=position/CHAR_BIT;
	bit=position%CHAR_BIT;

	mask=0x01;
	res=table[byte];        // I take the appropriate cells
	res=res>>bit;			// I put the wanted Bit first

	res=res & mask;			// With And I will see if it is 1 or 0

	if(res == 0x01)
	{
		return true;
	}
	else if(res == 0x00)
	{
		return false;
	}
	else
	{
		cout<<"Problem comparing the mask at Graph->look\n";
		return false;
	}	
}

uint32_t Grail::getnextnei(uint32_t currentnode,HyperGraph* thegraph,uint32_t* neis, list_node** listnodeptrs)
{
	if(thegraph->isUsed(currentnode) == false)  // If this node is not used it means it doen't have neightboors
	{
		return currentnode;
	}
	else
	{
		if(neis[currentnode] == 0)  // If it is the first 
		{
			listnodeptrs[currentnode]=thegraph->gimmelnode(thegraph->getlhead(currentnode)); // I set the pointer to the first list node
			
			neis[currentnode]++;
			return listnodeptrs[currentnode]->getNeighbor(0);
		}
		else
		{
			neis[currentnode]++;
			if(neis[currentnode] <= listnodeptrs[currentnode]->getNumOfNeighbors()) // If it is in the current list node
			{
				return listnodeptrs[currentnode]->getNeighbor(neis[currentnode] - 1);
			}
			else
			{
				if(listnodeptrs[currentnode]->getHasNext() == true) 
				{
					neis[currentnode]=1;
					listnodeptrs[currentnode]=thegraph->gimmelnode(listnodeptrs[currentnode]->getNextListNode());

					return listnodeptrs[currentnode]->getNeighbor(0);
				}
				else // If it doesn't have next this is node has no more neightboors to explore
				{
					return currentnode;
				}
			}
		}
	}
}

bool Grail::question(uint32_t comp1,uint32_t comp2)
{
	if(table[comp1].getmin_rank() <= table[comp2].getmin_rank())
	{
		if(table[comp1].getrank() >= table[comp2].getrank())
		{
			return true;
		}
	}
	return false;					
						
}