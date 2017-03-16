#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../headers/index.h"

using namespace std;


Index::Index(){	//same with createNodeIndex() function	
	
	table = (NodeIndex*)malloc(sizeof(NodeIndex)*SIZE);
	current_size = SIZE;
	num_of_nodes = 0;
	uint32_t i;

	for(i=0; i<current_size; i++){
		table[i].inUse = false;
		table[i].allneightboors = 0;
		table[i].LaHash.setsizedtable();
	}
	
}
void Index::createHashTable(uint32_t nodeId){
	table[nodeId].LaHash.initDtable();
	return;
}


Index::~Index(){
	
	for(uint32_t i=0; i<current_size; i++)
	{
		if(isUsed(i))
		{
			table[i].LaHash.destroyDtable();
		}
	}

	free(table);
}

int Index::insertNode(uint32_t nodeId){
	uint32_t i;

	if(nodeId<0){
		cout<<"Not acceptable id:"<<nodeId<<". Insertion failed"<<endl;
		return -1;
	}
	if(nodeId<current_size){
		if(table[nodeId].inUse == false){	//means nodeId doesn't exist so create it
			
			num_of_nodes++;			
			return 0; //success
		}
		else{
			
			return -2;	//nodeId already exists
		}
	}
	else{
		//realloc to store new node.
		uint32_t multiplier = 1;
		while(nodeId>(current_size<<multiplier)-1){
			multiplier +=1;
		}

		uint32_t newsize;
		newsize=current_size<<multiplier;

		NodeIndex* temp_index;
		temp_index = (NodeIndex*)realloc(table,(newsize)*sizeof(NodeIndex));
		
		if(temp_index!=NULL){
			table = temp_index;
		}
		else{
			cout<<"Error occured during realloc"<<endl;
			exit(1);
		}


		for(i=current_size; i<newsize; i++){
		 	setInUseFalse(i);	//all new (empty) have inUse = false;
		 	table[i].allneightboors=0;
		 	table[i].LaHash.setsizedtable();
		}
		
		current_size = newsize;
		num_of_nodes++;
		return 0;
	}
	
} 



int Index::getListHead(uint32_t nodeId){
	int list_head;
	list_head = table[nodeId].head;
	return list_head;
}

int Index::getListTail(uint32_t nodeId){
	int list_tail = table[nodeId].tail;
	return list_tail;
}

void Index::init_index(){	//only for the beginning at main
	
	cout<<"Index initialized"<<endl;
	return;
}


NodeIndex::NodeIndex(){		
	
}

int Index::setListHead(uint32_t nodeId,int value){

	table[nodeId].head = value;
	return 0;
}

int Index::setListTail(uint32_t nodeId,int value){

	table[nodeId].tail = value;
	return 0;
}

void Index::setInUseTrue(uint32_t nodeId){
	table[nodeId].inUse = true;
	return;
}

void Index::setInUseFalse(uint32_t nodeId){
	table[nodeId].inUse = false;
	return;
}

bool Index::isUsed(uint32_t nodeId){
	
	if(table[nodeId].inUse == true){
		
		return true;
	}
	else{
		return false;
	}
}

void Index::setHashTable(uint32_t nodeId1,uint32_t ptr,uint32_t cell){
	table[nodeId1].LaHash.setpointer(cell,ptr);
	return;
}

uint32_t Index::getHashTable(uint32_t nodeId,uint32_t cell){	//returns the list node hash[cell] points to
	return table[nodeId].LaHash.getpointer(cell);
}


uint32_t Index::hashFunction(uint32_t nodeId1,uint32_t nodeId2){
	uint32_t hash_size = table[nodeId1].LaHash.getDtableSize();
	return nodeId2%hash_size;
}
