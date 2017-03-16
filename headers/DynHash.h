#include <iostream>

using namespace std;

class DynHash 
{
	private:
		uint32_t size;
		uint32_t* dtable;
	
	public:
		DynHash();

		void setpointer(uint32_t cell , uint32_t points){dtable[cell]=points;};
		uint32_t getpointer(uint32_t cell){return dtable[cell];}
		void duplicatetable();
		uint32_t getDtableSize(){return size;}
		void initDtable();
		void destroyDtable();
		void setsizedtable();

		~DynHash();
};