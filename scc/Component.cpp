#include "../headers/Component.h"

Component::Component(){
	includedNodesCount = 0;
	includedNodeIds = NULL;
	includedNodesSize = 0;
}

Component::Component(uint32_t compId) : componentId(compId){
	includedNodesCount = 0;
	includedNodeIds = NULL;
	includedNodesSize = 0;	
}


void  Component::addincludedNodeId(uint32_t nodeId){
	
	if(includedNodesCount == 0){
		includedNodeIds = new uint32_t [1];	
		includedNodeIds[0] = nodeId;
		includedNodesSize = 1;
		includedNodesCount++;
	
	}
	else{
		if(includedNodesCount >= includedNodesSize){
			uint32_t* temp;
			includedNodesSize <<=1;
			temp = new uint32_t [includedNodesSize];
			memcpy(temp,includedNodeIds,includedNodesCount*sizeof(uint32_t));
			delete [] includedNodeIds;
			temp[includedNodesCount] = nodeId;
			includedNodeIds = temp;
			includedNodesCount++;
		}	
		else{
			includedNodeIds[includedNodesCount] = nodeId;
			includedNodesCount++;
		}
	}	
	
}

void Component::printIncludedNodeIds(){
		for(uint32_t i=0; i<includedNodesCount; i++)
			cout<<includedNodeIds[i]<<" ";
		cout<<endl;
	}

uint32_t Component::getIncludedNodeId(uint32_t id){
	if(id < includedNodesCount) 
		return includedNodeIds[id]; 
	else 
		return UINT32_MAX;  //error
}

Component::~Component(){
	
	if(includedNodeIds != NULL)
		delete [] includedNodeIds;
}
