#include <iostream>
#include <climits>
#include <unistd.h>
#include "../headers/HyperGraph.h"
using namespace std;

void HyperGraph::insert(uint32_t nodeId1,uint32_t nodeId2)
{
	if(nodeId1 < gimmecursizeout()){	
		if(edgeExists(nodeId1,nodeId2)){
			
			return;
		}
	}

	edges ++;

	uint32_t hash_value;	//position in hash table
	int ret;
	uint32_t ptr1,ptr2;
	ret = out.insertNode(nodeId1);
	
	if(ret == 0){
		ptr1 = outbuf.allocNewNode();
		
		out.createHashTable(nodeId1);
		out.setHashTable(nodeId1,ptr1,0);
		out.setListHead(nodeId1,ptr1);
		
		out.setInUseTrue(nodeId1);	//table[nodeId1] is now used

		ptr2 = outbuf.insertNeighbor(ptr1,nodeId2,0);
	}
	else if(ret == -1){
		cout<<"Unacceptable nodeId"<<endl;
		return;
	}
	else{
		hash_value = out.hashFunction(nodeId1,nodeId2);
		ptr1 = out.getHashTable(nodeId1,hash_value);
		ptr2 = outbuf.insertNeighbor(ptr1,nodeId2,0);
		
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
				hashVal = out.hashFunction(nodeId1,nei);
				ptr = out.getHashTable(nodeId1,hashVal);
				retPtr = outbuf.insertNeighbor(ptr,nei,0);		
			}
			hashVal = out.hashFunction(nodeId1,nodeId2);
			ptr = out.getHashTable(nodeId1,hashVal);
			retPtr = outbuf.insertNeighbor(ptr,nodeId2,0);
			
			if(ptr != retPtr)
			{
				
				cout<<"Warning !!! in reallocation of neighbors"<<endl;
				return;
			}
		}
		
	}
}


bool HyperGraph::edgeExists(uint32_t nodeId1, uint32_t nodeId2){
	
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

void HyperGraph::printGraph(){
	int ret;
	uint32_t i;
	list_node* n;
	uint32_t j;
	uint32_t num_neighbors;
	cout<<"In Hyper Print"<<endl;
	for(i=0; i<out.getCurrentSize(); i++){	
		ret = out.getListHead(i);
		if(out.isUsed(i)){
			cout<<"NODE: "<<i<<endl;
			int ptr = ret;
			while(1){
				n = outbuf.getListNode(ptr);
				num_neighbors = n->getNumOfNeighbors();
				// cout<<"nei: " <<num_neighbors<<endl;
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
	}
}