#ifndef SCC_H
#define SCC_H

#include <iostream>
#include <cstring>

#include "../headers/Component.h"

using namespace std;


class ComponentCursor
{
	uint32_t index;
public:
	Component *component_ptr;

	void setIndex(uint32_t i){index = i;}
	uint32_t getIndex(){return index;}
};



class SCC {
		Component *components;
		uint32_t componentsSize;
		uint32_t componentsCount;
		uint32_t* idBelongsToComponent;	//inverted index

	public:

		SCC(uint32_t numOfnodes) : componentsSize(numOfnodes){

			// cout<<"In SCC constructor"<<endl;
			componentsCount = 0;
			// componentsSize = numOfnodes;

			components = new Component[componentsSize];
			
			idBelongsToComponent = new uint32_t [numOfnodes];
			memset(idBelongsToComponent, UINT32_MAX, numOfnodes); 
		}

		// void estimateStronglyConnectedComponents(Graph *graph){
		// 	graph->tarjan(this);
		// }

		Component* getComponent(uint32_t id){
			if(id < componentsCount)
				return &components[id];
			else
				return NULL;
		}

		void addNodeToComponent(uint32_t compId , uint32_t nodeId);
		void initComponent(uint32_t compId);

		void setIdBelongsToComponent(uint32_t id,uint32_t componentId){idBelongsToComponent[id] = componentId;}
		//findNodeStronglyConnectedComponentID
		uint32_t getIdBelongsToComponent(uint32_t id){return idBelongsToComponent[id];}

		uint32_t getComponentsCount(){return componentsCount;}
		void setComponentsCount(uint32_t compCount){componentsCount = compCount;}


		void printComponents();

		bool iterateStronglyConnectedComponentID(ComponentCursor &cursor);

		bool next_StronglyConnectedComponentID(ComponentCursor &cursor);
		~SCC(){
			// cout<<"In SCC destructor"<<endl;
			delete [] components;
			delete [] idBelongsToComponent;
		};

		
};




#endif // SCC_H




