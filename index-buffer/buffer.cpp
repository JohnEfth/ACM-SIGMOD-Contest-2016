#include <iostream>
#include <cstdlib>

#include "../headers/buffer.h"

#define BufferSize 512
using namespace std;

buffer::buffer()
{
    //ctor
    adj_list = (list_node*) malloc(BufferSize * sizeof(list_node));
    nodeCounter = 0;
    currentSize = BufferSize;
    // ~~~~~~~~~~~~~~TEST~~~~~~~~~~~~~~~~~~~~
    for(uint32_t i=0;i<currentSize;i++){
        adj_list[i].setHasNext(false);        
        adj_list[i].setNumOfNeighbors(0);
        adj_list[i].setNextListNode(0);
        adj_list[i].setLocalDepth(1);
    }
     // ~~~~~~~~~~~~~~END OF TEST~~~~~~~~~~~~~~~~~~~~
}

buffer::~buffer()
{
    //dto
    free(adj_list);
}

list_node* buffer::getListNode(uint32_t ptr){
    return &adj_list[ptr];
}

uint32_t buffer::allocNewNode(){
    uint32_t nodeIndex,previous_size;
    if (nodeCounter>=currentSize){  //all list nodes are in use , reallocate memory
        previous_size = currentSize;    //ADDED AFTER SEG
        
        list_node *tempBuffer = NULL;
        tempBuffer = (list_node*)realloc(adj_list,(currentSize<<1)*sizeof(list_node));
        if(tempBuffer!=NULL)
            adj_list = tempBuffer;
        else{
            free(adj_list);
            cerr<<"Error reallocating memory"<<endl;
            exit(1);
        }
        currentSize <<= 1;
        nodeIndex = nodeCounter++;
       
        for(uint32_t i=previous_size;i<currentSize;i++){
            adj_list[i].setHasNext(false);
            adj_list[i].setNumOfNeighbors(0);
            adj_list[i].setNextListNode(0);
            adj_list[i].setLocalDepth(1);
        }
       
    }
    else //find the first node that's not allocated
        nodeIndex = nodeCounter++;
    
    return nodeIndex;
}

uint32_t buffer::insertNeighbor(uint32_t ptr1,uint32_t neighborId,int version){  //ptr = tail
    list_node *tempNode1;
    uint32_t neighbors;
    tempNode1 = &adj_list[ptr1];
  
    neighbors = tempNode1->getNumOfNeighbors();
           
    if(neighbors<N){ //there is still room for one more
        tempNode1->setNeighbor(neighbors,neighborId);
        tempNode1->setEdgeProperty(neighbors,version);
        tempNode1->setNumOfNeighbors(++neighbors);    
        return ptr1;
    }

    
    uint32_t ptr2 = allocNewNode();
    list_node *tempNode2 = &adj_list[ptr2];
        
    tempNode1 = &adj_list[ptr1];
    tempNode1->setNumOfNeighbors(0);    //ready to reinitilize both listnodes
    tempNode2->setNumOfNeighbors(0);

    uint32_t local_depth = tempNode1->getLocalDepth();
    local_depth <<= 1;
    tempNode1->setLocalDepth(local_depth);
    tempNode2->setLocalDepth(local_depth);
    
    if(tempNode1->getHasNext()){         //set next ptr
        tempNode2->setHasNext(true);
        tempNode2->setNextListNode(tempNode1->getNextListNode());
    }
    else{
        tempNode2->setHasNext(false);
    }
    tempNode1->setHasNext(true);
    tempNode1->setNextListNode(ptr2);
    return ptr2;
}


//insert neighbor for "in" buffer
uint32_t buffer::insertNeighborNoHash(uint32_t tail,uint32_t neighborId,int version){  //ptr = tail
    list_node *tempNode;
    uint32_t neighbors;
    tempNode = &adj_list[tail];
   
    neighbors = tempNode->getNumOfNeighbors();
    
    if(neighbors<N){ //there is still room for one more
        tempNode->setNeighbor(neighbors,neighborId);
        tempNode->setEdgeProperty(neighbors,version);
        tempNode->setNumOfNeighbors(++neighbors);
        return tail;
    }

    uint32_t ptr2 = allocNewNode();
    list_node *tempNode2 = &adj_list[ptr2];
    tempNode2->setNeighbor(0,neighborId);
    tempNode2->setEdgeProperty(0,version);
    tempNode2->setNumOfNeighbors(1);
    tempNode = &adj_list[tail];
    tempNode->setHasNext(true);
    tempNode->setNextListNode(ptr2);
    return ptr2;
}








//version 2 
// TEST?//////////////////////////////////
// uint32_t buffer::insertNeighbor(uint32_t ptr,uint32_t neighborId){  //ptr = tail
//     list_node *tempNode;
//     uint32_t neighbors;
//     tempNode = &adj_list[ptr];
//     uint32_t tail;
//     neighbors = tempNode->getNumOfNeighbors();
//     //search if edge already exist
//     while(1){
//         for(int i=0;i<neighbors;i++)
//             if(neighborId == tempNode->getNeighbor(i)) return ptr;
//         if(tempNode->getHasNext()){
//             ptr = tempNode->getNextListNode();
//             tempNode = &adj_list[ptr];
//             neighbors = tempNode->getNumOfNeighbors();
//         }
//         else{
//             tail = ptr;
//             break;
//         }
//     }        
//     if(neighbors<N){ //there is still room for one more
//         tempNode->setNeighbor(neighbors,neighborId);
//         tempNode->setNumOfNeighbors(++neighbors);
//         return tail;
//     }
// //    cout<<"need to alloc new list_node"<<endl;
//     //need for new list_node
//     uint32_t ptr2 = allocNewNode();
//     list_node *tempNode2 = &adj_list[ptr2];
//     tempNode2->setNeighbor(0,neighborId);
//     tempNode2->setNumOfNeighbors(1);
//     tempNode = &adj_list[tail];
//     tempNode->setHasNext(true);
//     tempNode->setNextListNode(ptr2);
//     return ptr2;
// }
