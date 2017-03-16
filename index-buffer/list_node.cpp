#include <iostream>
#include <cstdlib>

#include "../headers/list_node.h"

using namespace std;

list_node::list_node()
{
    //ctor
    hasNext=false;
    numOfNeighbors = 0;
    nextListNode = 0;
    localDepth = 1;
}

list_node::~list_node()
{
    //dtor
}

