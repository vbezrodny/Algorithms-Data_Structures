#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

namespace LIST {
	
	class Item;
	class List; 
	
	class Item {
    	Item *next;
    	Item *prev;
    	List *parent; 
    	
    	public:
    		Item();
    		Item(Item *prev, Item *next, List *parent); 
    		virtual ~Item();
    		
    		void set_prev(Item *prev_) { prev = prev_; }
    		Item *get_prev() const {return prev; }
    		
    		void set_next(Item *next_) { next = next_; }
    		Item *get_next() const { return next; }
    		
    		void set_parent(List *parent_) { parent = parent_; }
    		List *get_parent() const { return parent; }
	};

	class List {
    	Item *head;
    	Item *tail;
    	
    	public: 
    		List();
    		List(Item *head, Item *tail); 
    		~List();
			
    		void set_head(Item *head_) { head = head_; }
    		Item *get_head() const { return head; }
    		
    		void set_tail(Item *tail_) { tail = tail_; }
    		Item *get_tail() const { return tail; }
    		
    		void Add(Item *item);
    		Item *GetItem(int numb);
			Item *Remove(int numb);
			void Delete(int numb);
			void Insert(Item *item, int numb);
			int Count() const;
			void Clear();
			int GetIndex(Item *item) const;
			
	};
	
}

#endif
