#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>
#include <new>
#include "nosuchobject.h"

using namespace std;

template <typename T>
class Vector;

/* op<< has been modified in this version of Vector to display
 * the data members continuously without spaces and commas, 
 * as that is the appropriate representation for integers.
 */
template <typename T>
ostream& operator<<(ostream &, const Vector<T>& );

template <typename T>
class Vector {
  friend ostream& operator<< <>(ostream&, const Vector<T>&);

  public:
	typedef T* Iterator;
	Vector();
	Vector(const size_t count);
	Vector(const size_t count, const T& value);
	Vector(const Vector<T>& other);
	Vector(initializer_list<T> init); 
	~Vector();
	void clear();
	T& at(const size_t i) const;
	Iterator atPosition(size_t index) const;
	Iterator begin() const noexcept;
	Iterator end() const noexcept;
	Iterator prev(Iterator it) const;
	Iterator insert(Iterator pos, const T& value);
	Iterator insert(Iterator pos, size_t count, const T& value);
	Iterator insert(Iterator pos, initializer_list<T> ilist);
	Iterator erase(Iterator pos);
	Iterator erase(Iterator first, Iterator last);
	T& operator[](const size_t i) const;
	const T& front() const;
	const T& back() const;
	const Vector<T>& operator=(const Vector<T>& other);
	const Vector<T>& operator=(initializer_list<T> ilist);
	void push_back(const T& value);
	void pop_back();
	size_t size() const noexcept;
	bool empty() const noexcept;
	void printVector() const noexcept;

  private:
	T* contents;
	size_t sz;
	size_t cap;
	void increaseCapacity();
}; // Vector<T> class

///////////////////// FRIEND FUNCTIONS /////////////////////////////

template <typename T>
ostream& operator<<(ostream &out, const Vector<T> &vector){
	for(auto current:vector)
		out << current;
	return out;
}

///////////////////// PRIVATE METHODS //////////////////////////////
template <typename T>
void Vector<T>::increaseCapacity(){
	cap *= 2;
	T *newContents = new T[cap];
	for(unsigned i=0; i<sz; i++)
		newContents[i] = contents[i];
	delete[] contents;
	contents = newContents;
}

// PUBLIC METHODS /////////////////////////
template <typename T>
Vector<T>::Vector():sz{0},cap{10}{
	contents = new T[cap];
}

template <typename T>
Vector<T>::Vector(const size_t count):sz{count},cap{count}{
	contents = new T[cap];	
}

template <typename T>
Vector<T>::Vector(const size_t count, const T& value):Vector(count){
	for(size_t i = 0; i < count; i++)
		contents[i] = value;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other):contents{0},sz{0},cap{0}{
	*this = other;
}

template <typename T>
Vector<T>::Vector(initializer_list<T> init):cap{init.size()}{
	contents = new T[cap];
	sz = 0;
	for(T object: init){
		push_back(object);
	}
}

template <typename T>
Vector<T>::~Vector(){
	delete[] contents;
}

template <typename T>
void Vector<T>::clear(){
	sz = 0;
}

template <typename T>
T& Vector<T>::at(const size_t i) const{
	return *this[i];
}

template <typename T>
bool Vector<T>::empty() const noexcept{
	return sz==0;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::atPosition(size_t index) const{
	Iterator returner = &operator[](index);
	return returner;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() const noexcept{
	return &contents[0];	
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::prev(Iterator it) const{
	return --it;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() const  noexcept{
	return &contents[sz];	
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(Iterator pos, const T& value){
	if(sz==cap){
		size_t location = pos - begin();
		increaseCapacity();
		++sz;
		for(size_t i = sz-1; i > location; i--)//copies over old values
			contents[i] = contents[i-1];
		contents[location] = value;//adds new value
		Iterator returner = atPosition(location);
		return returner;//returns location where new value was inserted
	}
	else{	//case when we don't increaseCap, so 'pos' doesnt get invalidated
		Iterator traverse = end();
		Iterator trail = traverse;
		while(traverse!=pos){//shifting everything over 1 slot
			*traverse = *(--trail);
			--traverse;
		}
		*pos = value;
		++sz;
		return pos;//returns location where new value was inserted
	}
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(Iterator pos, size_t count, const T& value){
	if(sz == cap){
		size_t location = pos - begin();
		sz += count;
		while(cap < sz)
			increaseCapacity();
		size_t addUpTo = location + count;
		for(size_t i = sz-1; i >= addUpTo; i--) // copies over old elements
			contents[i] = contents[i-count];
		for(size_t i = location; i < addUpTo; i++) // adds the new 'value' elements
			contents[i] = value;
		Iterator returner = atPosition(location);
		return returner;//returns first location where new value was inserted
	}
	else{//we don't need to increaseCap, so we dont need a new iterator. pos works
		sz += count;
		Iterator returner = pos;
		Iterator helper = pos, posPlusCount = pos + count;
		Iterator traverse = pos + count;
		while(traverse != end()) // copies over old elements
			*(traverse++)=*(helper++);
		for(;pos!=posPlusCount;++pos)//adds in new value
			*pos = value;
		return returner;//returns first location where new value was inserted
	}
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(Iterator pos, initializer_list<T> ilist){
	if(sz == cap){
		size_t location = pos - begin();
		sz += ilist.size();// size of resulting vector after inserting
		while(cap < sz)
			increaseCapacity();
		size_t addUpTo = location + ilist.size();
		for(size_t i = sz-1; i >= addUpTo; i--) // copies over old elements
			contents[i] = contents[i-ilist.size()];
		Iterator import = atPosition(location);
		for(T object:ilist){// traverse through initializer list and adds elements
			*import = object;
			++import;
		}
		Iterator returner = atPosition(location);
		return returner;
	}
	else{//case where we dont need to increase cap, so we can still use 'pos'
		sz += ilist.size();
		Iterator returner = pos;
		Iterator helper = pos;
		Iterator traverse = pos + ilist.size();
		while(traverse != end()) // copies over old elements
			*(traverse++)=*(helper++);
		for(T object:ilist){// traverse through initializer list and add elements
			*pos = object;
			++pos;
		}
		return returner;
	}
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::erase(Iterator pos){
	if(pos!=end()){
		Iterator traverse = pos;
		Iterator trail = pos;
		Iterator lastElement = end()-1;
		while(traverse!=lastElement){
			*traverse = *(++trail);
			traverse++;
		}
		sz--;
		return pos;
	}	
	throw out_of_range("Vector::erase(Iter): pos is out of range");
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::erase(Iterator first, Iterator last){
	Iterator pos = first;
	if(last == end()){//in cases where last = end, we can just decrease size
		if(first == begin())
			clear();
		else{
			size_t difference = last - first;
			sz -= difference;
		}
	}
	else{
		size_t difference = last-first;//pointer arithmetic to figure out sz change
		while(last!=end()){//copies elemens to fill the gap created by erasing
			*first=*last;
			++first;
			++last;
		}
		sz -= difference;
	}
	return pos;
}

template <typename T>
T& Vector<T>::operator[](const size_t i) const{
	if(i<0 || i>=sz)
		throw out_of_range("Vector::operator[]: error! tried access invalid index");
	return contents[i];
}

template <typename T>
const T& Vector<T>::front() const{
	if(sz<=0)
		throw NoSuchObject("Vector::front: error! tried accessing an empty vector");
	return contents[0];
}

template <typename T>
const T& Vector<T>::back() const{
	if(sz<=0)
		throw NoSuchObject("Vector::back: error! tried accessing an empty vector");
	return contents[sz-1];
}

template <typename T>
const Vector<T>& Vector<T>::operator=(const Vector<T>& other){
	if(&other != this){
		if(!empty()) clear();
		if(cap!=other.cap){
			delete[] contents;
			cap = other.cap;
			contents = new T[cap];
		}
		sz = other.sz;
		for(unsigned i=0; i<sz; i++)
			contents[i] = other[i];
	}
	return *this;
}

template <typename T>
const Vector<T>& Vector<T>::operator=(initializer_list<T> ilist){
	delete[] contents;
	contents = new T[ilist.size()];
	sz = 0;
	for(T object: ilist)
		push_back(object);
	return *this;
}

template <typename T>
void Vector<T>::push_back(const T& value){
	if(sz == cap)
		increaseCapacity();
	contents[sz++] = value; 
}

template <typename T>
void Vector<T>::pop_back(){
	if(sz>0)
		sz--;
	else
		throw NoSuchObject("Vector::pop_back: error! tried popping an empty vector");
}

template <typename T>
size_t Vector<T>::size() const noexcept{
	return sz;
}

template <typename T>
void Vector<T>::printVector() const noexcept{
	for(auto current:*this)
		cout << current << " ";
}

#endif
