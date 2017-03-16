//#ifndef BUFFER_H
//#define BUFFER_H

#include "../headers/list_node.h"

class buffer
{
    private:
        list_node * adj_list;
        uint32_t nodeCounter;    //counts how many list_nodes are assigned and being used from index
        uint32_t currentSize;
    public:
        buffer();
        ~buffer();
        list_node* getListNode(uint32_t ptr);
        uint32_t allocNewNode();
        uint32_t insertNeighbor(uint32_t ptr,uint32_t neighborId,int version);
        uint32_t getCurrentSize(){return currentSize;};
        uint32_t insertNeighborNoHash(uint32_t tail,uint32_t neighborId,int version);
};

//#endif // BUFFER_H
