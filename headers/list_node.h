//#ifndef LIST_NODE_H
//#define LIST_NODE_H

#include <cstdint>
#include <cinttypes>
#define N 20

class list_node
{
    private:
        uint32_t neighbor[N];
        uint32_t edgeProperty[N];
        uint32_t nextListNode;
        uint32_t numOfNeighbors;
        bool hasNext;
        uint32_t localDepth;
    public:
        list_node();
        virtual ~list_node();
       
        uint32_t getNumOfNeighbors(){return numOfNeighbors;}
        void setNumOfNeighbors(uint32_t num){numOfNeighbors = num;}
       
        void setNeighbor(int i ,uint32_t value){neighbor[i]=value;}
        uint32_t getNeighbor(uint32_t i){return neighbor[i];}

        int getNextListNode(){return nextListNode;}
        void setNextListNode(uint32_t next){nextListNode = next;}
       
        bool getHasNext(){return hasNext;}
        void setHasNext(bool flag){hasNext=flag;}
        
        
        uint32_t getLocalDepth(){return localDepth;};
        void setLocalDepth(uint32_t depth){localDepth = depth;};

        void setEdgeProperty(int i , uint32_t value){edgeProperty[i] = value;}
        uint32_t getEdgeProperty(int i){return edgeProperty[i];}
};

//#endif // LIST_NODE_H
