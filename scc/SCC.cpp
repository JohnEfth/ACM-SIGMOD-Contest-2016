#include <cinttypes>

#include "../headers/SCC.h"



void SCC::initComponent(uint32_t compId){
	if(compId < componentsSize) 
		components[compId].setComponentId(compId);
	else{
		cout<<"Invalid Component"<<endl;
		exit(1);
	}	
}

void SCC::addNodeToComponent(uint32_t compId , uint32_t nodeId){
	if(compId < componentsSize) 
		components[compId].addincludedNodeId(nodeId);
	else{
		cout<<"Invalid Component"<<endl;
		exit(1);
	}
}


void SCC::printComponents(){
	ComponentCursor cursor;
	bool r = iterateStronglyConnectedComponentID(cursor);
	if(!r){
		cout<<"Error in iterate!"<<endl;
		return;
	}

	do{
		cout<<" COMPONENT "<<cursor.component_ptr->getComponentId()<<endl;
		cursor.component_ptr->printIncludedNodeIds();
	}while(next_StronglyConnectedComponentID(cursor));	
}

bool SCC::iterateStronglyConnectedComponentID(ComponentCursor &cursor){
	if(componentsCount >= 1){
		cursor.component_ptr = &components[0];
		cursor.setIndex(0);
		return true;
	}
	else{
		cout<<"SCC empty!"<<endl;
		return false;
	}
}

bool SCC::next_StronglyConnectedComponentID(ComponentCursor &cursor){
	uint32_t index = cursor.getIndex();
	if(index <= componentsCount - 2 ){  //still components to iterate
		index++;
		cursor.component_ptr = &components[index];
		cursor.setIndex(index);
		return true;
	} 
	else
		return false;
}



