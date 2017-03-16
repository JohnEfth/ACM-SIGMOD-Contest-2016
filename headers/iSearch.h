#include <cinttypes>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#define ROW_SIZE 512

class SearchNode
{
	private:
		uint32_t id;
		int level;
		
	public:
		SearchNode(int id,int lvl);
		int getlevel(){return this->level;}
		uint32_t getid(){return this->id;}

		void set_level(int setlvl){level=setlvl;}
		void set_id(uint32_t setid){id=setid;}

		void print();
		~SearchNode();
};

class iSearch
{
	private:
		uint32_t *vfrom, *vto;
		SearchNode* rfrom,*rto;
		uint32_t head_rfrom,tail_rfrom,head_rto,tail_rto;
		uint32_t version;
		uint32_t current_size_from,current_size_to,current_size_visited;
	public:
		iSearch();
		uint32_t get_head_rfrom(){return head_rfrom;}
		uint32_t get_tail_rfrom(){return tail_rfrom;}
		uint32_t get_head_rto(){return head_rto;}
		uint32_t get_tail_rto(){return tail_rto;}
		void set_head_rfrom(uint32_t setter){head_rfrom=setter;}
		void set_tail_rfrom(uint32_t setter){tail_rfrom=setter;}
		void set_head_rto(uint32_t setter){head_rto=setter;}
		void set_tail_rto(uint32_t setter){tail_rto=setter;}

		void initialize(uint32_t graph_size);

		uint32_t get_head_id(bool from);
		int get_head_lvl(bool from);

		void update(uint32_t position,bool from);
		bool look(uint32_t position, bool from);

		void add(uint32_t id,int level,bool from);     // Boolean to know which table is about (from / to)
		void remove(bool from);
		bool isEmpty(bool from);


		~iSearch();

};