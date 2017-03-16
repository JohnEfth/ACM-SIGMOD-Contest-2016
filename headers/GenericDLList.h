#ifndef LinkedList_h
#define LinkedList_h

#include <assert.h>
#include <iostream>

using namespace std;

template <class T>

class GenericDLList
{

	class Node{
	public: 
		T data;
		Node *next;
		Node *prev;

		Node(T t,Node* p, Node* n):data(t),prev(p),next(n){}
	};


	Node* head;
	Node* tail;
	int listSize;

	

public:
	GenericDLList(): head(NULL), tail(NULL), listSize(0){};

	int getListSize(){return listSize;};

	void pushFront(T data)
	{
		Node *prevHead = head;
		head = new Node(data,NULL,head);
		if(prevHead!=NULL)
			prevHead->prev = head;

		if(listSize <= 0) //empty list
			tail = head;
			
		listSize++;

	}
	
	void pushBack(T data)
	{
		Node *prevTail = tail;
		tail = new Node(data,tail,NULL);
		if(prevTail!=NULL)
			prevTail->next = tail;

		if(listSize <= 0) //list empty
			head = tail;
		
		listSize++;
	}

	
	T popFront(){

		assert(head!=NULL);

		Node *node = head;
		T data = head->data;
		head = head->next;
		
		listSize--;
		
		if(listSize <= 0){ //list got empty
			head = NULL;
			tail = NULL;
		}
		delete node;
		return data;
	}

	T popBack(){

		assert(tail!=NULL);

		Node *node = tail;
		T data = tail->data;
		tail = tail->prev;
		
		listSize--;
		
		if(listSize <= 0){ //list got empty
			head = NULL;
			tail = NULL;
		}	

		delete node;
		return data;
	}

	// use flag as 0 to delete 1st occurence of node OR 1 to delete all occurences
	void deleteNode(T data , bool flag){

		Node *tempNode = head;
		while(tempNode!=NULL){
			if(data == tempNode->data){
				Node *tempNode_cp = tempNode;
				if(tempNode->next != NULL)
					tempNode->next->prev = tempNode->prev;
				else   //node is tail
					tail = head;	
				if(tempNode->prev != NULL)
					tempNode->prev->next = tempNode->next;
				else //node is head
					head = tail;

				tempNode = tempNode->next;

				listSize--;
				if(listSize<=0)
					head = tail = NULL;
				
				delete tempNode_cp;
				
				if(!flag)    
					break;
			}
			else
				tempNode = tempNode->next;
		}
	}
	
	T getHead(){
		
		assert(head!=NULL);
		return head->data;
	}

	
	T getTail(){
		
		assert(tail!=NULL);
		return tail->data;
	}

	
	~GenericDLList()
	{
		while(head != NULL)
		{
			Node *temp = head;
			head = head->next;
			delete temp;
		}
	}

};

#endif

