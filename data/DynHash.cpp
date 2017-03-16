#include <iostream>
#include <stdlib.h>
#include "../headers/DynHash.h"

using namespace std;

DynHash::DynHash()
{
	
}

void DynHash::initDtable(){
	dtable=(uint32_t*) malloc(sizeof(uint32_t));   // Initialize with one bucket
	size=1;
}


void DynHash::duplicatetable()
{
	uint32_t newsize;
	uint32_t* temp=NULL;
	newsize=size<<1;        // size * 2
	temp=(uint32_t*)realloc(dtable,(newsize)*sizeof(uint32_t));   // I realloc space for an array two times bigger
	if(temp !=NULL)    // realloc success
	{
		dtable=temp;       
	}
	else				// realloc no success
	{
		free(dtable);
		cerr<<"Error reallocating memory for Hash"<<endl;
		exit(2);
	}

	for(uint32_t i=size; i<newsize; i++ )  // Setting the pointers of the new buckets to the appropriate list_node
	{
		dtable[i]=dtable[i - size];
	}

	size=newsize;

}

void DynHash::destroyDtable()
{
	if(dtable != NULL)
	{
		free(dtable);
	}	
}

void DynHash::setsizedtable()
{
	dtable=NULL;
	size=0;
}

DynHash::~DynHash()
{
	cout<<"Dynamic Hash Table's destructor is called"<<endl;
}