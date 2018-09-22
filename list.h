#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>
#include <new>
#include "nosuchobject.h"

using namespace std;

template <typename T>
class List;

/* op<< has been modified in this version of List to display
 * the data members continuously without spaces and commas, 
 * as that is the appropriate representation for integers.
 */
template <typename T>
ostream& operator<<(ostream &, const List<T>& );

template <typename T>
class List { 
  friend ostream& operator<< <>(ostream&, const List<T>&);
  
  private:
	struct Node{
		T object;
		Node *next;
		Node *prev;
		Node(const T& data = T{}):object{data}, next{this}, prev{this}{}
	}; //Node struct

  public:
	class Iterator{
		friend class List;
		Node* current;
      public:
		Iterator():current{nullptr}{}
		Iterator(Node *node):current(node){}
		Iterator& operator--(){ //prefix--
			current = current->prev;
			return *this;
		}
		Iterator operator--(int){ //postfix--
			Iterator old = *this;
			++(*this);
			return old;
		}
		Iterator& operator++(){ //prefix++
			current = current->next;
			return *this;
		}
		Iterator operator++(int){ //postfix++
			Iterator old = *this;
			++(*this);
			return old;
		}
		T& operator*(){
			return current->object;
		}
		bool operator==(const Iterator& other) const{
			return current==other.current;
		}
		bool operator!=(const Iterator& other) const{
			return !(*this==other);
		}
	}; //Iterator sub-class

  private:
    Node* head;
    size_t sz;
    void initialize();
    Node* getIthNode( size_t pos ) const;
	void insertBefore(Node* &node, const T& value);
    void removeNode(Node* pos); 
  
  public:
    List();
    List(size_t count, const T& value);
    explicit List(size_t count);
    List(initializer_list<T> init);
    List(const List<T>& other);
    ~List();
    void clear();
	Iterator insert(Iterator it, const T& value);
    void push_back(const T& value = T{});
    void push_front(const T& value = T{});
    void pop_front();
    void pop_back();
    T &front() const;
    T &back() const;
    Iterator atPosition(size_t index) const;
    Iterator begin() const noexcept;
    Iterator end() const noexcept;
	Iterator prev(Iterator it) const;
    Iterator erase(Iterator pos);
    Iterator erase(Iterator first, Iterator last);
    bool empty() const noexcept;
    size_t size() const noexcept;
    const List<T> &operator=(const List<T> & );
    const List<T> &operator=(initializer_list<T> ilist); 
  	void printList() const noexcept;
}; // List class

///////////////////////// FRIEND FUNCTIONS ///////////////////////////////

template <typename T>
ostream& operator<<(ostream &out, const List<T> &list){
	for(auto current:list)
		out << current;
	return out;
}

///////////////////////// PRIVATE METHODS ////////////////////////////////

template <typename T>
void List<T>::initialize(){
    head = new Node;
    head->next = head;
    head->prev = head;
    sz = 0;
}
template <typename T>
typename List<T>::Node* List<T>::getIthNode(size_t pos) const{
    if(pos < 0 || pos >= sz)
        throw out_of_range("List::getIthNode: input is out of range");
    Node *temp = head->next;
    for(unsigned i=0; i<pos; i++){
        temp = temp->next;
    }
    return temp;
}

template <typename T>
void List<T>::insertBefore(Node* &node, const T& value){
	Node* temp = new Node(value);
	temp->next = node;
	temp->prev = node->prev;
	node->prev->next = temp;
	node->prev = temp;
}

///////////////////////// PUBLIC METHODS ////////////////////////

template <typename T>
List<T>::List(){
    initialize();
}

template <typename T>
List<T>::List(size_t count, const T& value){
    initialize();
    for(size_t i = 0; i < count; i++)
	push_back(value);
}

template <typename T>
List<T>::List(size_t count){
    initialize();
    for(size_t i=0; i < count; i++)
	push_back();
}

template <typename T>
List<T>::List(initializer_list<T> init){
    initialize();
    for(T object:init)
	push_back(object); 
}

template <typename T>
List<T>::List( const List<T> &other){
    initialize();
    *this = other;
}

template <typename T>
List<T>::~List(){
    clear();
    delete head;
}

template <typename T>
void List<T>::clear(){
    Node *temp = head->next;
    while(temp!=head){
        temp = temp->next;
        delete temp->prev;
    }
    head->next = head;
    head->prev = head;
    sz = 0;
}

template <typename T>
void List<T>::push_back(const T& value){
	insert(end(), value);
}

template <typename T>
void List<T>::push_front(const T& value){
	insert(begin(), value);
}

template <typename T>
typename List<T>::Iterator List<T>::insert(Iterator it, const T& value){
	Node* temp = it.current;
	insertBefore(temp, value);
	sz++;
	return --it;
}

template <typename T>
void List<T>::pop_front(){
    if(sz>0){
        Node *front = head->next;
        head->next = front->next;
        front->next->prev = head;
        delete front;
        sz--;
    }
    else
        throw NoSuchObject("List::pop_front: error! tried popping an empty list");
}

template <typename T>
void List<T>::pop_back(){
    if(sz>0){
        Node *back = head->prev;
        head->prev = back->prev;
        back->prev->next = head;
        delete back;
        sz--;    
    }
    else
        throw NoSuchObject("List::pop_back: error! tried popping an empty list"); 
}

template <typename T>
T& List<T>::front() const{
    if(sz<=0)
    	throw NoSuchObject("List::front: error! tried accessing an empty list");
    return head->next->object;
}

template <typename T>
T& List<T>::back() const{
    if(sz<=0)
    	throw NoSuchObject("List::back: error! tried accessing an empty list");
    return head->prev->object;

}

template <typename T>
typename List<T>::Iterator List<T>::atPosition(size_t index) const{
	Iterator atPos;
	atPos.current = getIthNode(index);
	return atPos;
}

template <typename T>
typename List<T>::Iterator List<T>::begin() const noexcept{
	Iterator first(head->next);
	return first;
}

template <typename T>
typename List<T>::Iterator List<T>::end() const noexcept{
	Iterator last(head);
	return last;
}

template <typename T>
typename List<T>::Iterator List<T>::prev(Iterator it) const{
	return --it;
}

template <typename T>
typename List<T>::Iterator List<T>::erase(Iterator pos){
	if(pos!=end()){
		Node* temp =pos.current;
		temp->next->prev = temp->prev;
		temp->prev->next = temp->next;
		Iterator nextValidPos = temp->next;
		delete temp;
		sz--;
		return nextValidPos;
	}
	throw out_of_range("List::erase: iterator is pointing past end of list");
}

template <typename T>
typename List<T>::Iterator List<T>::erase(Iterator first, Iterator last){
	if(first!=end()){
		Node* beg = (first.current);
		Node* end1 = (last.current);
		beg->prev->next = end1;//links the list past deleted elements
		end1->prev = beg->prev;//links the list past deleted elements
		Node* trail = beg;
		while(beg != end1){
			beg = beg->next;
			delete trail;
			trail = beg;
			sz--;
		}
		return last;
	}
	throw out_of_range("List::erase: iterator is pointing past end of list");
}

template <typename T>
bool List<T>::empty() const noexcept{
    return sz==0;
}

template <typename T>
size_t List<T>::size() const noexcept{
    return sz;
}

template <typename T>
const List<T>& List<T>::operator=(const List<T> &other){
	if(&other != this){
        if(!empty()) clear();
		for(auto current:other)
			push_back(current);
    }
    return *this;
}

template <typename T>
const List<T>& List<T>::operator=(initializer_list<T> ilist){
    if(!empty())
        clear();
    for(auto object: ilist)
	    push_back(object);
	return *this;
}

template <typename T>
void List<T>::printList() const noexcept{
	for(auto current:*this)
		cout << current << " ";
}

#endif


