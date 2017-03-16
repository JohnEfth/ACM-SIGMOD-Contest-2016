#include <iostream>
#include <string>

#include "../headers/GenericDLList.h"

using namespace std;


class TestClass{
public:
	int x;
	double y;
	TestClass(int a,double b):x(a),y(b){};
	void set(int a,double b){x=a;y=b;};
};

int main(){
	GenericDLList <int> list ;
	assert(list.getListSize() == 0);
	list.pushFront(6);
	list.pushFront(7);
	list.pushFront(16);
	assert(list.getListSize() == 3);
	assert(list.getHead() == 16);
	assert(list.getTail() == 6);

	list.popFront();
	list.popFront();
	assert(list.getListSize() == 1);
	assert(list.getHead() == 6);
	assert(list.getTail() == 6);

	list.pushFront(8);
	list.pushFront(9);

	list.deleteNode(8,0);
	assert(list.getListSize() == 2);
	assert(list.getHead() == 9);
	assert(list.getTail() == 6);
	list.deleteNode(6,1);
	assert(list.getListSize() == 1);
	assert(list.getHead() == 9);
	assert(list.getTail() == 9);

	list.deleteNode(9,0);
	assert(list.getListSize() == 0);

	//check that the structure behaves correctly as a stack
	GenericDLList <TestClass> stack;
	TestClass temp(1,0.0);
	assert(stack.getListSize() == 0);

	stack.pushFront(temp);
	temp.set(2,1.1);
	stack.pushFront(temp);
	temp.set(3,4.4);
	stack.pushFront(temp);
	assert(stack.getListSize() == 3);
	temp = stack.getHead();
	assert(temp.x == 3);
	temp = stack.getTail();
	assert(temp.y == 0.0);

	temp = stack.popFront();
	assert(temp.x == 3);
	assert(temp.y == 4.4);

	temp = stack.popFront();
	assert(temp.x == 2);
	assert(temp.y == 1.1);

	assert(stack.getListSize() == 1);
	temp = stack.getHead();
	assert(temp.x == 1);
	temp = stack.getTail();
	assert( temp.x == 1);	
	assert (temp.y == 0.0);

	//check that the structure behaves correctly as a queue
	GenericDLList <string> queue;
	assert(queue.getListSize() == 0);
	queue.pushBack("abc");
	queue.pushBack("def");
	queue.pushBack("ghi");
	queue.pushBack("jkl");
	assert(queue.getListSize() == 4);
	assert(queue.getHead() == "abc");
	assert(queue.getTail() == "jkl");

	string str;
	str = queue.popFront();
	assert(str == "abc");
	str = queue.popFront();
	assert(str == "def");
	assert(queue.getListSize() == 2);
	assert(queue.getHead() == "ghi");
	assert(queue.getTail() == "jkl");

	str = queue.popFront();
	assert(queue.getHead() == "jkl");
	assert(queue.getTail() == "jkl");
	str = queue.popFront();
	assert(queue.getListSize() == 0);


	return 0;

}