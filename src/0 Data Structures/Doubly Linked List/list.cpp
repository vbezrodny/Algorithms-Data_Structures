#include "list.h"

namespace LIST {
	Item::Item() : prev(nullptr), next(nullptr), parent(nullptr) {}
	Item::Item(Item *prev_, Item *next_, List *parent_) : prev(prev_), next(next_), parent(parent_) {}
	Item::~Item() {
		if (parent != nullptr) {
			parent->Remove(parent->GetIndex(this));
		}
	}
	
	List::List() : head(nullptr), tail(nullptr) {}
	List::List(Item *head_, Item *tail_) : head(head_), tail(tail_) {}
	
	void List::Add(Item *item) {
    	if (!item) return;

    	if (!head && !tail) {
        	head = item;
        	tail = item;
        	
        	head->set_prev(nullptr);
        	tail->set_next(nullptr);
        	item->set_parent(this);
			 
    	} else {
        	tail->set_next(item);
        	item->set_prev(tail);
        	
        	tail = item;
        	
        	head->set_prev(nullptr);
        	tail->set_next(nullptr); 
			item->set_parent(this); 
    	}	
	}
	
	Item *List::GetItem(int numb) {
		if (numb < 0) return nullptr;
		
    	Item *temp = head;
    	int i = 0;

    	while (temp && i < numb) {
        	temp = temp->get_next();
        	i++;
    	}

    	return temp;
	}

	Item *List::Remove(int numb) {
    	Item *temp = GetItem(numb);

    	if (!temp) return nullptr;
	
	    if (head == tail) {
        	head = nullptr;
        	tail = nullptr;
        	
    	} else if (head == temp) {
        	head = temp->get_next();
        	head->set_prev(nullptr);
        	temp->set_next(nullptr);
        	
    	} else if (tail == temp) {
        	tail = temp->get_prev();
        	tail->set_next(nullptr);
        	temp->set_prev(nullptr);
        	
    	} else {
        	temp->get_prev()->set_next(temp->get_next());
        	temp->get_next()->set_prev(temp->get_prev());
        	temp->set_next(nullptr);
        	temp->set_prev(nullptr);
    	}

		temp->set_parent(nullptr);

    	return temp;
	}

	void List::Delete(int numb) {
    	if (numb < 0) return;
    	
    	delete Remove(numb);
	}	
	
	void List::Insert(Item *item, int numb) {
    	if (!item) return;
    	
    	Item *temp = GetItem(numb);
    	
    	if (!temp) {
        	Add(item);
        	
			return;
    	}
    	
    	if (temp == head) {
        	item->set_prev(nullptr);
        	item->set_next(head);
        	head->set_prev(item);
        	head = item;
        	temp->set_parent(this);
        	
			return;
    	}
    	
    	temp->get_prev()->set_next(item);
    	item->set_prev(temp->get_prev());
    	temp->set_prev(item);
    	item->set_next(temp);
    	temp->set_parent(this);
	}	
	
	int List::Count() const {
    	Item *temp = head;
    	int cnt = 0;

    	while (temp) {
        	cnt++;
        	temp = temp->get_next();
    	}

    	return cnt;
	}
	
	void List::Clear() {
    	while (head) {
        	Delete(0);
    	}
	}
	
	int List::GetIndex(Item *item) const {
    	if (!item) return -1;
		
		Item *temp = head;
    	int i = 0;

    	while (temp && temp != item) {
        	temp = temp->get_next();
        	i++;
    	}

    	return i;
	}

	
	List::~List() {
		Clear();
	}
}


