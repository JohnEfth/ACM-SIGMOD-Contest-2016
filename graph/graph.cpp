#include <iostream>
#include <climits>
#include <string.h>
#include <unistd.h>

#include "../headers/graph.h"

using namespace std;


Graph::Graph()
{
	total_nodes = 0;
	
}

int Graph::grailsearch(uint32_t from, uint32_t to,Grail* grail,SCC* scc,iSearch& isearch)
{
	if(from == to)
	{
		return 0;
	}
	uint32_t maxnum,i,id,bufid,nei,numofnei,maxnum1,neifrom,neito,fromcompId,tocompId;

	list_node* lnode;
	
	bool exists,next,answer;
	int curfromlvl,curtolvl,newlvl,path;

	maxnum1=this->in.getCurrentSize();  // Number of indexes from George
	maxnum=this->out.getCurrentSize();

	if(maxnum < maxnum1)      // I get the max of the current_sizes
		maxnum=maxnum1;

	isearch.initialize(maxnum);

	isearch.add(from,0,1);
	isearch.add(to,0,0);
	
	isearch.update(from,1);
	isearch.update(to,0);

	curfromlvl=0;
	curtolvl=0;

	neifrom=out.getallnei(from);
	neito=in.getallnei(to);
	
	fromcompId=scc->getIdBelongsToComponent(from);
	tocompId=scc->getIdBelongsToComponent(to);

	while ((isearch.isEmpty(1)!=true) && (isearch.isEmpty(0)!=true))
	{
		if(neifrom < neito)
		{
			neifrom=0;  // Initialize the new sum of neightboors
			while(isearch.get_head_lvl(1) <= curfromlvl)  // Until I finish with all the nodes of the current level
			{
				id=isearch.get_head_id(1);      // I get the id of the first node from the Row
				if(out.isUsed(id) == false)
				{
					isearch.remove(1);
					if(isearch.isEmpty(1) == true)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				bufid=out.getListHead(id);       // I get the index of the buffer for this node
				
				do     // I start exporing node's neightboors
				{	

					lnode=outbuf.getListNode(bufid);      //I get the pointer to the list_node from the buffer
					numofnei=lnode->getNumOfNeighbors(); 	//I get the number of neighboors for this list
			
					for(i=0; i<numofnei; i++) 
					{
						nei=lnode->getNeighbor(i);
						
						exists=isearch.look(nei,1);		//0 if not visited till now 1 else
						answer=grail->question(scc->getIdBelongsToComponent(nei),tocompId);  // Chck if grail says no

						if((exists == 0) && (answer == 1))  // If I have not visited him
						{
							isearch.update(nei,1);      					// I set visited
							newlvl=isearch.get_head_lvl(1) + 1;   // I set the level
							
							isearch.add(nei,newlvl,1);


							exists=isearch.look(nei,0);					
							if(exists == 1)							// If the node exists on the other visit table I found the trace
							{
								
								path=curtolvl+curfromlvl+1;
								 
								return path;				
							}
						}
						else if(answer == 0)
						{
							isearch.update(nei,1);
						}

						neifrom+=out.getallnei(nei);
					}
					next=lnode->getHasNext();
					bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
				}while (next == 1);

				isearch.remove(1);    // I have added to the Row all the node's neightbors so now I delete it to get another head
				
				if(isearch.isEmpty(1) == true)
				{
					return -1;       // No more edges to search there is no path
				}
			}
			
			curfromlvl++;
		}
		else
		{
			// while for the incoming nodes
			neito=0;   // Initialize the new sum of neightboors
			while((isearch.get_head_lvl(0) <= curtolvl) )
			{
				id=isearch.get_head_id(0);      // I get the id of the first node from the Row
				if(in.isUsed(id) == false)
				{
					isearch.remove(0);
					if(isearch.isEmpty(0) == true)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				bufid=in.getListHead(id);       // I get the index of the buffer for these node

				do     // I start exporing node's neightboors
				{			

					lnode=inbuf.getListNode(bufid);      //I get the pointer to the list_node from the buffer
					numofnei=lnode->getNumOfNeighbors(); 	//I get the number of neighboors for this list
					
					for(i=0; i<numofnei; i++) 
					{

						nei=lnode->getNeighbor(i);
						
						exists=isearch.look(nei,0);
						answer=grail->question(fromcompId,scc->getIdBelongsToComponent(nei));  // Chck if grail says no


						if((exists == 0) && (answer == 1))  // If I have not visited him
						{
							isearch.update(nei,0);      					// I set visited
							newlvl=isearch.get_head_lvl(0) + 1;   // I set the level
							
							isearch.add(nei,newlvl,0);						// I add it to the Row

							exists=isearch.look(nei,1);					
							if(exists == 1)							// If the node exists on the other visit table I found the trace
							{
								path=curtolvl+curfromlvl+1;    
								  
								return path;
							}
						}
						else if(answer == 0)
						{
							isearch.update(nei,0);   
						}
						neito+=in.getallnei(nei);  // calculating all the neightboors of the next loop
					}
					next=lnode->getHasNext();
					bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
				}while (next == 1);

				isearch.remove(0);    // I have added to the Row all the node's neighbors so now I delete it
				if(isearch.isEmpty(0) == true)
				{
					return -1;        // No more edges to search there is no path 
				}
			}
			
			
			curtolvl++;         // on the next loop I will search the next level
		}
	}


	return -1;
}

void Graph::update(uint32_t position,char* table)
{
	int byte,bit;
	char mask;

	byte=position/CHAR_BIT;
	bit=position%CHAR_BIT;

	mask=0x01;          // I initialize the mask as 0000 0001 
	mask=mask<<bit;		// I move the 1 to the approrpiate position

	table[byte]= table[byte] | mask;   // I do OR in order to make the 0 -> 1

}
bool Graph::look(uint32_t position,char* table)
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


int Graph::insert(uint32_t nodeId1,uint32_t nodeId2,int version)
{
	if(nodeId1 < gimmecursizeout()){	
		if(edgeExists(nodeId1,nodeId2)){
			
			return -1;
		}
	}

	uint32_t hash_value;	//position in hash table
	int ret;
	uint32_t ptr1,ptr2;
	ret = out.insertNode(nodeId1);
	
	if(ret == 0){
		if(nodeId1<gimmecursizein()){
			if(in.isUsed(nodeId1) ==false){
				total_nodes++;
			}
		}
		else{
			total_nodes++;
		}
		ptr1 = outbuf.allocNewNode();
		
		out.createHashTable(nodeId1);
		out.setHashTable(nodeId1,ptr1,0);
		out.setListHead(nodeId1,ptr1);
		
		out.setInUseTrue(nodeId1);	//table[nodeId1] is now used

		out.plusplusneightboors(nodeId1);  // +1 to the neightboors

		ptr2 = outbuf.insertNeighbor(ptr1,nodeId2,version);
	}
	else if(ret == -1){
		cout<<"Unacceptable nodeId"<<endl;
		return -1;
	}
	else{

		out.plusplusneightboors(nodeId1);  // +1 to the neightboors

		hash_value = out.hashFunction(nodeId1,nodeId2);
		ptr1 = out.getHashTable(nodeId1,hash_value);
		ptr2 = outbuf.insertNeighbor(ptr1,nodeId2,version);
		
		if(ptr1!=ptr2){	//allocated new list node.must update hash table
			uint32_t offset;
			list_node* tempNode1 = outbuf.getListNode(ptr1);
			offset = tempNode1->getLocalDepth();
			
			if(offset > out.getHashTableSize(nodeId1)){	//check if hash table needs reallocation
				out.duplicateHashTable(nodeId1);
				hash_value = out.hashFunction(nodeId1,nodeId2);
			}

		
			out.setHashTable(nodeId1,ptr2,hash_value);	//delete me when aposxoliasw ta panw katw
			

			//update hash indexes to point to correct list nodes
			int k = 0;
			offset = offset/2;
			
			for(uint32_t i=hash_value+offset; i<out.getHashTableSize(nodeId1); i+=offset){
				if(out.getHashTable(nodeId1,i) == ptr1){
					if(k%2 == 0)
					{
						out.setHashTable(nodeId1,ptr1,i);
					}
					else
					{
						out.setHashTable(nodeId1,ptr2,i);
					}
					k++;
				}
			}
			k=0;
			for(uint32_t i=hash_value-offset; i<out.getHashTableSize(nodeId1); i-=offset){
				if(out.getHashTable(nodeId1,i) == ptr1){
					if(k%2 == 0)
					{
						
						out.setHashTable(nodeId1,ptr1,i);
					}
					else
					{
						
						out.setHashTable(nodeId1,ptr2,i);
					}
					k++;
				}
			}
			uint32_t nei,hashVal,ptr,retPtr;

			for(int i=0;i<N;i++)
			{
				nei = tempNode1->getNeighbor(i);
				uint32_t nei_version = tempNode1->getEdgeProperty(i);
				hashVal = out.hashFunction(nodeId1,nei);
				ptr = out.getHashTable(nodeId1,hashVal);
				retPtr = outbuf.insertNeighbor(ptr,nei,nei_version);		
			}
			hashVal = out.hashFunction(nodeId1,nodeId2);
			ptr = out.getHashTable(nodeId1,hashVal);
			retPtr = outbuf.insertNeighbor(ptr,nodeId2,version);
			
			if(ptr != retPtr)
			{
				
				cout<<"Warning !!! in reallocation of neighbors"<<endl;
				return -1;
			}
		}
		
	}
	uint32_t ptr,tail;
	ret = in.insertNode(nodeId2);
	if(ret == 0)
	{
		if(nodeId2<gimmecursizeout()){
			if(out.isUsed(nodeId2) ==false){
				total_nodes++;
			}
		}
		else{
			total_nodes++;
		}
		ptr = inbuf.allocNewNode();
		in.setListHead(nodeId2,ptr);
		in.setListTail(nodeId2,ptr);
		in.setInUseTrue(nodeId2);	//inUse is now true

		in.plusplusneightboors(nodeId2); // +1 one to the neightboors

		inbuf.insertNeighborNoHash(ptr,nodeId1,version);
	}
	else if(ret == -1){
		cout<<"Unacceptable nodeId"<<endl;
		return -1;
	}
	else{

		in.plusplusneightboors(nodeId2);  // +1 to the neightboors
		tail = in.getListTail(nodeId2);
		uint32_t tail2 = inbuf.insertNeighborNoHash(tail,nodeId1,version);
		if(tail2 != tail)
		{
			in.setListTail(nodeId2,tail2);
		}
	}
	return 0;
}



void Graph::printGraph(){
	int ret;
	uint32_t i;
	// bool used;
	list_node* n;
	uint32_t j;
	uint32_t num_neighbors;
	cout<<"~~~ In Struct ~~~"<<endl;
	for(i=0; i<in.getCurrentSize(); i++){	
		ret = in.getListHead(i);
		if(in.isUsed(i)){
			cout<<"NODE: "<<i<<endl;
			int ptr = ret;
			while(1){
				n = inbuf.getListNode(ptr);
				num_neighbors = n->getNumOfNeighbors();
	
				for(j= 0; j<num_neighbors; j++){
					
					cout<<n->getNeighbor(j)<<" ";
				}
				if(!n->getHasNext()){
					break;
				}
				
				ptr=n->getNextListNode();
			}
			cout<<endl;
			
		}
		cout<<endl;
	}
	cout<<"~~~ Out Struct ~~~"<<endl;
	for(i=0; i<out.getCurrentSize(); i++){	
		ret = out.getListHead(i);
		if(out.isUsed(i)){
			cout<<"NODE: "<<i<<endl;
			int ptr = ret;
			while(1){
				n = outbuf.getListNode(ptr);
				num_neighbors = n->getNumOfNeighbors();
				
				for(j= 0; j<num_neighbors; j++){
					
					cout<<n->getNeighbor(j)<<" ";
				}
				if(!n->getHasNext()){
					break;
				}		
				ptr=n->getNextListNode();
			}
			cout<<endl;
			
		}
		cout<<endl;
	}
}

bool Graph::edgeExists(uint32_t nodeId1, uint32_t nodeId2){
	
	if(!out.isUsed(nodeId1)){
		return false;
	}
	uint32_t hash_value = out.hashFunction(nodeId1,nodeId2);
	uint32_t ptr = out.getHashTable(nodeId1,hash_value);
	list_node* tempNode = outbuf.getListNode(ptr);
	for(uint32_t i = 0; i<tempNode->getNumOfNeighbors(); i++){
		if(tempNode->getNeighbor(i) == nodeId2){
			return true;
		}
	}
	return false;
}

Graph::~Graph()
{
	// cout<<"Graph destroyed\n";
}



uint32_t min(uint32_t x , uint32_t y){
	return ( x < y ? x : y );
}


uint32_t Graph::getnextnei(uint32_t currentnode,uint32_t* neis, list_node** listnodeptrs)
{
	if(out.isUsed(currentnode) == false)  // If this node is not used it means it doen't have neightboors
	{
		return currentnode;
	}
	else
	{
		if(neis[currentnode] == 0)  // If it is the first 
		{
			listnodeptrs[currentnode]=outbuf.getListNode(out.getListHead(currentnode)); // I set the pointer to the first list node
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
					listnodeptrs[currentnode]=outbuf.getListNode(listnodeptrs[currentnode]->getNextListNode());
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

void Graph::tarjan(SCC* scc , HyperGraph* hyperGraph){
	//Tarjan Info
	int indexCounter;
	GenericDLList <uint32_t> sccSet;
	uint32_t nodeCount;
	uint32_t *index;
	uint32_t *lowlink;
	bool *onStack;
	uint32_t sccCount;


	uint32_t *neis,*prev;
	uint32_t unvisitpoint=0;  // Pointer to the unvisited nodes , it will help me to go through the nodes' list only once
	uint32_t currentnode,nextnei; 
	list_node **listnodeptrs; 

	indexCounter = 0;
	sccCount = 0;
	nodeCount = out.getCurrentSize();
	index = new uint32_t[nodeCount];
	lowlink = new uint32_t[nodeCount];
	onStack = new bool[nodeCount];

	memset(index,UINT32_MAX,nodeCount * sizeof(uint32_t));   //all nodes are not visited


	neis=new uint32_t [nodeCount];   // I will keep in which neightboor I am in every node
	prev=new uint32_t [nodeCount];   // I will keep each node its previous	
	listnodeptrs=(list_node**) malloc(nodeCount*sizeof(list_node*));	// I will keep pointers to the current list_node

	memset(neis,0,sizeof(uint32_t)*nodeCount);  // Initialize neightboors to 0

	while(unvisitpoint < nodeCount){
		
		prev[unvisitpoint]=unvisitpoint; // It doesn't have any previous

		currentnode=unvisitpoint;

		index[currentnode] = indexCounter;
		lowlink[currentnode] = indexCounter;
		indexCounter ++;
		sccSet.pushFront(currentnode);
		onStack[currentnode] = true;

		nextnei=getnextnei(currentnode,neis,listnodeptrs);

		while((nextnei != currentnode) || (currentnode != prev[currentnode]))   // If the node has no neightboors OR if is the root
		{		
		
			if(nextnei == currentnode)
			{
				//current node has no neighbors
				//Check for new SCC
				if(lowlink[currentnode] == index[currentnode]){

					//~~~~~~~~~~~~~~~NEW SCC ~~~~~~~~~~~~~~
					uint32_t compId = sccCount;	
				
					scc->initComponent(compId);
					scc->setComponentsCount(scc->getComponentsCount()+1);

					uint32_t j;
					do{
						j = sccSet.popFront();
						onStack[j] = false;
		
						scc->addNodeToComponent(compId , j);
						scc->setIdBelongsToComponent(j,compId);

					}while(j != currentnode);
	
					sccCount++;
					uint32_t bufid,nodeId, numofnei , neiCompId ,nei;
					list_node *lnode;
					bool next;
					Component *comp_ptr =  scc->getComponent(compId);
					if(comp_ptr != NULL){
						for(uint32_t i = 0; i<comp_ptr->getIncludedNodesCount(); i++){
							nodeId = comp_ptr->getIncludedNodeId(i);
							bufid = out.getListHead(nodeId);       // I get the index of the buffer for this node
							do     // I start exporing node's neighbors
							{	
								lnode = outbuf.getListNode(bufid);  
								numofnei = lnode->getNumOfNeighbors(); 

								
								for(uint32_t n=0; n<numofnei; n++) 
								{
									nei=lnode->getNeighbor(n);
									neiCompId = scc->getIdBelongsToComponent(nei); 
									if( neiCompId == compId)
										continue; //edge inside component , no need to add edge to hypergraph
									else
										hyperGraph->insert(compId , neiCompId);
								}
								

								next=lnode->getHasNext();
								bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
							}while (next == 1);
							

							
						}
						
					}	
				}

				lowlink[prev[currentnode]] = min(lowlink[prev[currentnode]] , lowlink[currentnode]); 
				currentnode=prev[currentnode];
				nextnei=getnextnei(currentnode,neis,listnodeptrs);
				continue;
			}
			else if(index[nextnei] != UINT32_MAX) // If the next node is visited
			{
				//neighbor alrady visited
				if(onStack[nextnei])
					lowlink[currentnode] = min(lowlink[currentnode] , index[nextnei]); 

				nextnei=getnextnei(currentnode,neis,listnodeptrs);
				continue;   // I continue to the next neightboor this path is searched
			}
			else if(index[nextnei] == UINT32_MAX){    //next node not visited

				index[nextnei] = indexCounter;
				lowlink[nextnei] = indexCounter;
				indexCounter ++;
				sccSet.pushFront(nextnei);
				onStack[nextnei] = true;


				if(!out.isUsed(nextnei)){
					//  ~~~~ NEW SCC ~~~~~~~~~~
					uint32_t compId = sccCount;	

					scc->initComponent(compId);
					scc->setComponentsCount(scc->getComponentsCount()+1);

					uint32_t j;
					do{
						j = sccSet.popFront();
						onStack[j] = false;
					
						scc->addNodeToComponent(compId , j);
						scc->setIdBelongsToComponent(j,compId);

					}while(j != nextnei);

					sccCount++;

					lowlink[currentnode] = min(lowlink[nextnei] , lowlink[currentnode]); 
				
					nextnei=getnextnei(currentnode,neis,listnodeptrs);
					continue;   // I continue to the next neightboor this path is searched
				}




				lowlink[currentnode] = min(lowlink[nextnei] , lowlink[currentnode]); 
			}

			

			prev[nextnei]=currentnode;
			currentnode=nextnei;							// I move to the next node
			nextnei=getnextnei(currentnode,neis,listnodeptrs); 
		}
		

			// For the root  
			//Check for new SCC
			if(lowlink[currentnode] == index[currentnode]){

				// ~~~~ NEW SCC ~~~~~~~~~~~~~~

				uint32_t compId = sccCount;	

				scc->initComponent(compId);
				scc->setComponentsCount(scc->getComponentsCount()+1);

				uint32_t j;
				do{
					j = sccSet.popFront();
					onStack[j] = false;
	
					scc->addNodeToComponent(compId , j);
					scc->setIdBelongsToComponent(j,compId);

				}while(j != currentnode);

				sccCount++;
				uint32_t bufid,nodeId, numofnei , neiCompId ,nei;
				list_node *lnode;
				bool next;
				Component *comp_ptr =  scc->getComponent(compId);
				if(comp_ptr != NULL){
					for(uint32_t i = 0; i<comp_ptr->getIncludedNodesCount(); i++){
						nodeId = comp_ptr->getIncludedNodeId(i);
						bufid = out.getListHead(nodeId);       // I get the index of the buffer for this node
						do     // I start exporing node's neighbors
						{	
							lnode = outbuf.getListNode(bufid);  

							numofnei = lnode->getNumOfNeighbors(); 	       

							for(uint32_t n=0; n<numofnei; n++) 
							{
								nei=lnode->getNeighbor(n);
								neiCompId = scc->getIdBelongsToComponent(nei); 
								if( neiCompId == compId)
									continue; //edge inside component , no need to add edge to hypergraph
								else
									hyperGraph->insert(compId , neiCompId);
							}

							next=lnode->getHasNext();
							bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
						}while (next == 1);


						
					}
				}	
			}

		do // move to the next unvisited node
		{
			unvisitpoint++;
			if(unvisitpoint == nodeCount)    // If I pass the table's size I get out
				break;
	
		}while ((index[unvisitpoint] != UINT32_MAX) || !out.isUsed(unvisitpoint));	
	}
		
	free(listnodeptrs);

	delete [] prev;
	delete [] neis;
	delete [] index;
	delete [] lowlink;
	delete [] onStack;
}

 
int Graph::estimateShortestPathSCC(SCC* scc , uint32_t from , uint32_t to,iSearch& isearch)
{
	//check that node ids belong to the same component
	uint32_t compId , neiCompId;
	if((compId = scc->getIdBelongsToComponent(from)) != scc->getIdBelongsToComponent(to))
	{
		
		return -1;
	}
	if(from == to)
	{
		return 0;
	}
 
	uint32_t maxnum,i,id,bufid,nei,numofnei,maxnum1,neifrom,neito;

	list_node* lnode;
	
	bool exists,next;
	int curfromlvl,curtolvl,newlvl,path;

	maxnum1=this->in.getCurrentSize();  // Number of indexes from George
	maxnum=this->out.getCurrentSize();

	if(maxnum < maxnum1)      // I get the max of the current_sizes
		maxnum=maxnum1;

	isearch.initialize(maxnum);

	isearch.add(from,0,1);
	isearch.add(to,0,0);
	
	isearch.update(from,1);
	isearch.update(to,0);

	curfromlvl=0;
	curtolvl=0;

	neifrom=out.getallnei(from);
	neito=in.getallnei(to);
	
	while ((isearch.isEmpty(1)!=true) && (isearch.isEmpty(0)!=true))
	{
		if(neifrom < neito)
		{
			neifrom=0;  // Initialize the new sum of neightboors
			while(isearch.get_head_lvl(1) <= curfromlvl)  // Until I finish with all the nodes of the current level
			{
				id=isearch.get_head_id(1);      // I get the id of the first node from the Row
				if(out.isUsed(id) == false)
				{
					isearch.remove(1);
					if(isearch.isEmpty(1) == true)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				bufid=out.getListHead(id);       // I get the index of the buffer for this node
				
				do     // I start exporing node's neightboors
				{	

					lnode=outbuf.getListNode(bufid);      //I get the pointer to the list_node from the buffer
					numofnei=lnode->getNumOfNeighbors(); 	//I get the number of neighboors for this list
			
					for(i=0; i<numofnei; i++) 
					{
						nei=lnode->getNeighbor(i);
						neiCompId=scc->getIdBelongsToComponent(nei);
						
						exists=isearch.look(nei,1);		//0 if not visited till now 1 else
						

						if((exists == 0) && (neiCompId == compId))  // If I have not visited him
						{
							isearch.update(nei,1);      					// I set visited
							newlvl=isearch.get_head_lvl(1) + 1;   // I set the level
							
							isearch.add(nei,newlvl,1);


							exists=isearch.look(nei,0);					
							if(exists == 1)							// If the node exists on the other visit table I found the trace
							{
								path=curtolvl+curfromlvl+1;
								 
								return path;				
							}
						}
						else if(neiCompId != compId)
						{
							isearch.update(nei,1);
						}

						neifrom+=out.getallnei(nei);
					}
					next=lnode->getHasNext();
					bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
				}while (next == 1);

				isearch.remove(1);    // I have added to the Row all the node's neightbors so now I delete it to get another head
				
				if(isearch.isEmpty(1) == true)
				{
					return -1;       // No more edges to search there is no path
				}
			}
			
			curfromlvl++;
		}
		else
		{
			// while for the incoming nodes
			neito=0;   // Initialize the new sum of neightboors
			while((isearch.get_head_lvl(0) <= curtolvl) )
			{
				id=isearch.get_head_id(0);      // I get the id of the first node from the Row
				if(in.isUsed(id) == false)
				{
					isearch.remove(0);
					if(isearch.isEmpty(0) == true)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				bufid=in.getListHead(id);       // I get the index of the buffer for these node

				do     // I start exporing node's neightboors
				{			

					lnode=inbuf.getListNode(bufid);      //I get the pointer to the list_node from the buffer
					numofnei=lnode->getNumOfNeighbors(); 	//I get the number of neighboors for this list
					
					for(i=0; i<numofnei; i++) 
					{

						nei=lnode->getNeighbor(i);
						neiCompId=scc->getIdBelongsToComponent(nei);

						exists=isearch.look(nei,0);
						

						if((exists == 0) && (neiCompId == compId))  // If I have not visited him
						{
							isearch.update(nei,0);      					// I set visited
							newlvl=isearch.get_head_lvl(0) + 1;   // I set the level
							
							isearch.add(nei,newlvl,0);						// I add it to the Row

							exists=isearch.look(nei,1);					
							if(exists == 1)							// If the node exists on the other visit table I found the trace
							{
								path=curtolvl+curfromlvl+1;    								
								  
								return path;
							}
						}
						else if(neiCompId != compId)
						{
							isearch.update(nei,0);   
						}
						neito+=in.getallnei(nei);  // calculating all the neightboors of the next loop
					}
					next=lnode->getHasNext();
					bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
				}while (next == 1);

				isearch.remove(0);    // I have added to the Row all the node's neighbors so now I delete it
				if(isearch.isEmpty(0) == true)
				{
					return -1;        // No more edges to search there is no path 
				}
			}
			
			
			curtolvl++;         // on the next loop I will search the next level
		}
	}


	return -1;
}


//-------------------------------------------------------------------------------------------------------------------------

int Graph::search(uint32_t from, uint32_t to,int current_version, iSearch& isearch)
{

	if(from == to)
	{
		return 0;
	}
	uint32_t maxnum,i,id,bufid,nei,numofnei,maxnum1,neifrom,neito;
	list_node* lnode;
	
	bool exists,next;
	int curfromlvl,curtolvl,newlvl,path,nei_version;

	maxnum1=this->in.getCurrentSize();  // Number of indexes from George
	maxnum=this->out.getCurrentSize();

	if(maxnum < maxnum1)      // I get the max of the current_sizes
		maxnum=maxnum1;

	isearch.initialize(maxnum);

	isearch.add(from,0,1);
	isearch.add(to,0,0);
	
	isearch.update(from,1);
	isearch.update(to,0);

	curfromlvl=0;
	curtolvl=0;

	neifrom=out.getallnei(from);
	neito=in.getallnei(to);
	
	while ((isearch.isEmpty(1)!=true) && (isearch.isEmpty(0)!=true))
	{
		if(neifrom < neito)
		{
			neifrom=0;  // Initialize the new sum of neightboors
			while(isearch.get_head_lvl(1) <= curfromlvl)  // Until I finish with all the nodes of the current level
			{
				id=isearch.get_head_id(1);      // I get the id of the first node from the Row
				if(out.isUsed(id) == false)
				{
					isearch.remove(1);
					if(isearch.isEmpty(1) == true)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				bufid=out.getListHead(id);       // I get the index of the buffer for this node
				
				do     // I start exporing node's neightboors
				{	

					lnode=outbuf.getListNode(bufid);      //I get the pointer to the list_node from the buffer
					numofnei=lnode->getNumOfNeighbors(); 	//I get the number of neighboors for this list
			
					for(i=0; i<numofnei; i++) 
					{
						nei=lnode->getNeighbor(i);
						
						exists=isearch.look(nei,1);		//0 if not visited till now 1 else
						nei_version=lnode->getEdgeProperty(i);

						if((exists == 0) && (nei_version <= current_version))  // If I have not visited him
						{
							isearch.update(nei,1);      					// I set visited
							newlvl=isearch.get_head_lvl(1) + 1;   // I set the level
							
							isearch.add(nei,newlvl,1);


							exists=isearch.look(nei,0);					
							if(exists == 1)							// If the node exists on the other visit table I found the trace
							{
								
								path=curtolvl+curfromlvl+1;
								 
								return path;				
							}
						}

						neifrom+=out.getallnei(nei);
					}
					next=lnode->getHasNext();
					bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
				}while (next == 1);

				isearch.remove(1);    // I have added to the Row all the node's neightbors so now I delete it to get another head
				
				if(isearch.isEmpty(1) == true)
				{
					return -1;       // No more edges to search there is no path
				}
			}
			
			curfromlvl++;
		}
		else
		{
			// while for the incoming nodes
			neito=0;   // Initialize the new sum of neightboors
			while((isearch.get_head_lvl(0) <= curtolvl) )
			{
				id=isearch.get_head_id(0);      // I get the id of the first node from the Row
				if(in.isUsed(id) == false)
				{
					isearch.remove(0);
					if(isearch.isEmpty(0) == true)
					{
						break;
					}
					else
					{
						continue;
					}
				}
				bufid=in.getListHead(id);       // I get the index of the buffer for these node

				do     // I start exporing node's neightboors
				{			

					lnode=inbuf.getListNode(bufid);      //I get the pointer to the list_node from the buffer
					numofnei=lnode->getNumOfNeighbors(); 	//I get the number of neighboors for this list
					
					for(i=0; i<numofnei; i++) 
					{

						nei=lnode->getNeighbor(i);
						
						exists=isearch.look(nei,0);
						nei_version=lnode->getEdgeProperty(i);

						if((exists == 0) && (nei_version <= current_version))  // If I have not visited him
						{
							isearch.update(nei,0);      					// I set visited
							newlvl=isearch.get_head_lvl(0) + 1;   // I set the level
							
							isearch.add(nei,newlvl,0);						// I add it to the Row

							exists=isearch.look(nei,1);					
							if(exists == 1)							// If the node exists on the other visit table I found the trace
							{
								path=curtolvl+curfromlvl+1;    
								  
								return path;
							}
						}


						neito+=in.getallnei(nei);  // calculating all the neightboors of the next loop
					}
					next=lnode->getHasNext();
					bufid=lnode->getNextListNode();   // after for-loop I set as bufid the next list node that is about the same node
				}while (next == 1);

				isearch.remove(0);    // I have added to the Row all the node's neighbors so now I delete it
				if(isearch.isEmpty(0) == true)
				{
					return -1;        // No more edges to search there is no path 
				}
			}
			
			
			curtolvl++;         // on the next loop I will search the next level
		}
	}


	return -1;
}

WCC* Graph::findWCC(){
	uint32_t maxnum,maxnum1,out_size,in_size,cells,i,id,out_num_neighbors,in_num_neighbors,nei;
	char *visited;
	int flag_out,flag_in,out_bufid,in_bufid; //or path 
	GenericDLList <uint32_t> queue;
	bool exists,next;
	list_node *out_list_node,*in_list_node;
	uint32_t nei_in_id,nei_out_id;		//both 3 vars used to optimize bfs
	list_node* nei_in_lnode,*nei_out_lnode;
	uint32_t first_zero_value =0,indicator ,temp , comp_id = 0,nei_in_neighbors,nei_out_neighbors;

	int num_of_nodes = getTotalNodes(), counter =0;
	//Compute total nodes so i can malloc wisely
	
	
	maxnum1=this->in.getCurrentSize();  // Number of indexes from George
	maxnum=this->out.getCurrentSize();
	out_size = maxnum;
	in_size = maxnum1; 

	if(maxnum < maxnum1)      // I get the max of the current_sizes
		maxnum=maxnum1;
	cells=(maxnum/CHAR_BIT) + 1 ;    // I will be using 1 bit for 1 node (not a whole char)
	visited=(char*) malloc(cells*sizeof(char));
	WCC* my_wcc = new WCC(num_of_nodes,maxnum);
	memset(visited,0,cells);	//at start all nodes are unvisited


	for(i=0; i<maxnum; i++){
		if(in.isUsed(i) ==true || out.isUsed(i) == true){
			temp = i;
			break;
		}	
	}
	indicator = i;	//indicates from which node I'll start the search in visited
	queue.pushBack(temp);
	my_wcc->assign_compId(i,comp_id);	//peirazei to id_belongs_to_components

	update(i,visited);
	counter++;

	my_wcc->init_component(comp_id);
	my_wcc->add_to_components(comp_id,i);
	
	while(first_zero_value!= UINT32_MAX)
	{
		while(queue.getListSize() > 0)
		{
			id = queue.getHead();
		
			flag_out=0;
			flag_in =0;
			if(id>out_size){
				flag_out = 1;
			}
			else if(out.isUsed(id) == false){
				flag_out = 1;
			}
			if(id>in_size){
				flag_in = 1;
			}
			else if(in.isUsed(id) == false){
				flag_in = 1;
			}

			if(flag_out == 0){
				out_bufid = out.getListHead(id);
		
				do{
					out_list_node = outbuf.getListNode(out_bufid);
					out_num_neighbors = out_list_node->getNumOfNeighbors();
					for(i=0; i<out_num_neighbors; i++){
						nei = out_list_node->getNeighbor(i);	//to id tou geitona : nei
						nei_in_id = in.getListHead(nei);
						nei_in_lnode = inbuf.getListNode(nei_in_id);
						nei_in_neighbors = nei_in_lnode->getNumOfNeighbors();
						exists = look(nei,visited);
						if((!out.isUsed(nei) && nei_in_lnode->getHasNext() == 0) && (exists ==0) && (nei_in_neighbors == 1 )){
							
							update(nei,visited);
							counter++;
							my_wcc->assign_compId(nei,comp_id);
							my_wcc->add_to_components(comp_id,nei);
							
						}
						else{
							

							if((exists == 0) && ((out.isUsed(nei) == true) || (in.isUsed(nei) == true))){
								update(nei,visited);
								counter++;
								temp = nei;
								queue.pushBack(temp);
								my_wcc->assign_compId(nei,comp_id);
								my_wcc->add_to_components(comp_id,nei);
							}
						}
						 
					}
					next = out_list_node->getHasNext();
					out_bufid = out_list_node->getNextListNode();
				}while(next == 1);
			}
			//incoming edges loop
			//data for incoming edges
			if(flag_in == 0){
				in_bufid = in.getListHead(id);
				do{
					in_list_node = inbuf.getListNode(in_bufid);
					in_num_neighbors = in_list_node->getNumOfNeighbors();

					for(i=0; i<in_num_neighbors; i++){
						nei = in_list_node->getNeighbor(i);

						nei_out_id = out.getListHead(nei);
						nei_out_lnode = outbuf.getListNode(nei_out_id);
						nei_out_neighbors = nei_out_lnode->getNumOfNeighbors();
						exists = look(nei,visited);
						if((!in.isUsed(nei) && nei_out_lnode->getHasNext() == 0) && (exists ==0) && (nei_out_neighbors == 1 )){
							
							update(nei,visited);
							counter++;
							my_wcc->assign_compId(nei,comp_id);
							my_wcc->add_to_components(comp_id,nei);
						
						}
						else{
							if((exists == 0) && ((out.isUsed(nei) == true) || (in.isUsed(nei) == true))){
						
								update(nei,visited);
								counter++;
								temp = nei;
								queue.pushBack(temp);
								my_wcc->assign_compId(nei,comp_id);
								my_wcc->add_to_components(comp_id,nei);
							}
						}
						
					}
					next = in_list_node->getHasNext();

					in_bufid = in_list_node->getNextListNode();
				}while(next == 1);
			}
			//WCC must be stored here
			queue.popFront();
		}


		my_wcc->check_components();
		my_wcc->updateCount();
		
		first_zero_value = UINT32_MAX;
		if(counter == num_of_nodes){
			break;
		}

		for(i=indicator; i<maxnum; i++){
			if((look(i,visited) == false) && ((out.isUsed(i) == true) || (in.isUsed(i) == true))){
				first_zero_value = i;	//find the first node of the graph that is not visited. Use him to find next wcc
				indicator = first_zero_value;	//indicator holds the index of the visited table 
				break;
			}
		}
		if(first_zero_value == UINT32_MAX){
			break;
		}
		temp = first_zero_value;
		comp_id++;
		update(first_zero_value,visited);
		counter++;
		queue.pushBack(temp);
		my_wcc->assign_compId(first_zero_value,comp_id);
		my_wcc->init_component(comp_id);
		my_wcc->add_to_components(comp_id,first_zero_value);
	

	}
	

	my_wcc->create_islands();

	free(visited);
	return my_wcc;
}

void Graph::ask_exist(uint32_t nodeId){
	if(in.isUsed(nodeId) == true){
		cout<<"in: yes"<<endl;
	}
	if(out.isUsed(nodeId) == true){
		cout<<"out: yes"<<endl;
	}
	return;
}