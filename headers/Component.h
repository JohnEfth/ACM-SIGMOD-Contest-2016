#include <iostream>
#include <cstring>

using namespace std;


class Component{
	uint32_t componentId;
	uint32_t includedNodesCount;
	uint32_t* includedNodeIds;
	uint32_t includedNodesSize;

public:

	Component();

	Component(uint32_t compId);

	void setComponentId(uint32_t compId){componentId = compId;}
	uint32_t getComponentId(){return componentId;}

	void setIncludedNodesCount(uint32_t count){includedNodesCount = count;}
	uint32_t getIncludedNodesCount(){return includedNodesCount;}

	void addincludedNodeId(uint32_t nodeId);
	void initComponent(uint32_t compId);
	uint32_t getIncludedNodeId(uint32_t id);

	void printIncludedNodeIds();

	~Component();
};
