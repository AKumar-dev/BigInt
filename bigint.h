#include "list.h"
#include "vector.h"
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

const char CHAR_OFFSET = '0';

template <typename T, template <typename> typename C>
class BigInt;

template <typename T, template <typename> typename C>
ostream& operator<<(ostream& , const BigInt<T,C> &);

template <typename T, template <typename> typename C>
ifstream& operator>>(ifstream& , BigInt<T,C> &);

template <typename T, template <typename> typename C>
class BigInt{
  friend ostream &operator<< <>(ostream& , const BigInt<T,C> &);
  friend ifstream &operator>> <>(ifstream& , BigInt<T,C> &);
  private:
	C<T> container;
  	size_t count;
  	bool sign; //stores whether the number is positive or negative
	bool direction; //how the container is stored (forward or backwards)
	void insert(const T&);
	void addHelper(typename C<T>::Iterator, typename C<T>::Iterator, typename C<T>::Iterator,
				   size_t, bool, bool, BigInt<T,C>&, int);

  public:
	BigInt();
	BigInt(const BigInt<T,C>&);
	void negate();
	void reverse();
	size_t getCount() const noexcept;
	const BigInt<T,C> operator+(const BigInt<T,C>&);
	const BigInt<T,C> operator-(const BigInt<T,C>&);
	const BigInt<T,C> operator+=(const BigInt<T,C>&);
	const BigInt<T,C> operator-=(const BigInt<T,C>&);
	const BigInt<T,C>& operator=(const BigInt<T,C>&);
	bool absEquals(const BigInt<T,C>&) const noexcept;
	bool operator==(const BigInt<T,C>&) const noexcept;
	bool operator!=(const BigInt<T,C>&) const noexcept;
	bool absGreaterThan(const BigInt<T,C> &other) const noexcept;
	bool operator>(const BigInt<T,C> &) const noexcept;
	bool operator<(const BigInt<T,C> &) const noexcept;
};//BigInt class

/////////////////////////// FRIEND FUNCTIONS //////////////////////////////////

template <typename T, template <typename> typename C>
ostream& operator<<(ostream &out, const BigInt<T,C> &bigint){
	if(bigint.count == 0) //for an empty BigInt, print out '0'
		out << "0";
	else{
		(bigint.sign == 0? out << "-" : out << " ");
		if(bigint.direction){ //case where BigInt is stored in forward order
			typename C<T>::Iterator last = bigint.container.end();
			typename C<T>::Iterator first = bigint.container.begin();
			if(typeid(T) == typeid(char)){
				while(first != last){
					char intRepresentation = *first + CHAR_OFFSET;
					out << intRepresentation;
					++first;
				}
			}
			else{
				while(first != last){
					out << *first;
					++first;
				}
			}
		}
		else{ //case where BigInt is stored in reverse order
			typename C<T>::Iterator last = bigint.container.prev(bigint.container.begin());
			typename C<T>::Iterator first = bigint.container.prev(bigint.container.end());
			if(typeid(T) == typeid(char)){
				while(first != last){
					char intRepresentation = *first + CHAR_OFFSET;
					out << intRepresentation;
					--first;
				}
			}
			else{
				while(first != last){
					out << *first;
					--first;
				}
			}
		}
	}
	return out;	
}

//BigInts are created in order from greatest-value to smallest-value digit
template <typename T, template <typename> typename C>
ifstream& operator>>(ifstream &in, BigInt<T,C> &bigint){ 
	string reader;
	char traverse;
    traverse = in.peek();
    if(traverse == '+' || traverse == '-'){
    	in >> traverse;
		if(traverse == '-')
			bigint.sign = 0;
	}
	while(in.get(traverse) && traverse != '\n'){
		if(traverse == '0'){
			if(bigint.count > 0)
				bigint.insert(traverse - CHAR_OFFSET);
		}
		else
			bigint.insert(traverse - CHAR_OFFSET);
	}
	return in;
}

/////////////////////////// PRIVATE METHODS ///////////////////////////////////

template <typename T, template <typename> typename C>
void BigInt<T,C>::insert(const T& value){
	container.push_back(value);
	++count;
}

//'G' relates to object with greater count, 'S' relates to object with smaller count
template <typename T, template <typename> typename C>
void BigInt<T,C>::addHelper(typename C<T>::Iterator beginG, 
							typename C<T>::Iterator endG,
							typename C<T>::Iterator beginS,
							size_t countS,
							bool dirG, bool dirS, BigInt<T,C> &object, int code){
	auto addDigits = [](T& temporary, T& carriedVal, BigInt<T,C> &obj) { //lambda expression for adding
		temporary += carriedVal;
		if (temporary >= 10){
			temporary -= 10;
			carriedVal = 1;
		}
		else
		carriedVal = 0;
		obj.container.push_back(temporary);
		obj.count++;
	};
	T carry = 0;
	if(code == 1){
		if(dirG == dirS){ // if both BigInts are stored in the same direction
			if(dirG){ // if both are stored in forwards order
				for(size_t total = 0; beginG != endG; total++){
					T temp = *beginG + (total < countS? *beginS: 0);
					addDigits(temp, carry, object);
					--beginG;
					--beginS;
				}
			}	
			else{ //if both are stored in reverse order
				for(size_t total = 0; beginG != endG; total++){
					T temp = *beginG + (total < countS? *beginS: 0);
					addDigits(temp, carry, object);
					++beginG;
					++beginS;
				}
			}
		}
		else if(dirG){ //if the bigger obj is forwards, and smaller is reverse order
			for(size_t total = 0; beginG != endG; total++){
				T temp = *beginG + (total < countS? *beginS: 0);
				addDigits(temp, carry, object);
				--beginG;
				++beginS;
			}
		}
		else{ //if the bigger object is reverse order, and smaller is forwards
			for(size_t total = 0; beginG != endG; total++){
				T temp = *beginG + (total < countS? *beginS: 0);
				addDigits(temp, carry, object);
				++beginG;
				--beginS;
			}
		}
	}
	else if(code == 2){
		if(dirG == dirS){ // if both BigInts are stored in the same direction
			if(dirG){ // if both are stored in forwards order
				for(size_t total = 0; beginG != endG; total++){
					T temp = *beginG + (total < countS? (9 - *beginS): 9);
					addDigits(temp, carry, object);
						--beginG;
					--beginS;
				}
			}	
			else{ //if both are stored in reverse order
				for(size_t total = 0; beginG != endG; total++){
					T temp = *beginG + (total < countS? (9 - *beginS): 9);
					addDigits(temp, carry, object);
						++beginG;
					++beginS;
				}
			}
		}
		else if(dirG){ //if the bigger obj is forwards, and smaller is reverse order
			for(size_t total = 0; beginG != endG; total++){
				T temp = *beginG + (total < countS? (9 - *beginS): 9);
				addDigits(temp, carry, object);
				--beginG;
				++beginS;
			}
		}
		else{ //if the bigger object is reverse order, and smaller is forwards
			for(size_t total = 0; beginG != endG; total++){
				T temp = *beginG + (total < countS? (9 - *beginS): 9);
				addDigits(temp, carry, object);
				++beginG;
				--beginS;
			}
		}
	}
	if(carry){
		if(code == 1){
			object.container.push_back(carry);
			object.count++;
		}
		else if(code == 2){
			BigInt<T,C> carryAdder;
			carryAdder.insert(carry);
			object += carryAdder;  //in 9's comp you have to add the carry
		}
	}
	
	//removes any 0's at the beginning of the number
	while(*object.container.prev(object.container.end()) == 0){
		object.container.pop_back();
		--object.count;
	}
}

/////////////////////////// PUBLIC METHODS ////////////////////////////////////

//default constructor
template <typename T, template <typename> typename C>
BigInt<T,C>::BigInt():count{0},sign{1},direction{1}{}

//negates a BigInt
template <typename T, template <typename> typename C>
void BigInt<T,C>::negate(){
	sign = (!sign);
}

//reverse a BigInt (switch the order of digits)
template <typename T, template <typename> typename C>
void BigInt<T,C>::reverse(){
	direction = (!direction);	
}

template <typename T, template <typename> typename C>
size_t BigInt<T,C>::getCount() const noexcept{
	return count;	
}

//copy constructor
template <typename T, template <typename> typename C>
BigInt<T,C>::BigInt(const BigInt<T,C> &other){
	*this = other;
}

template <typename T, template <typename> typename C>
const BigInt<T,C> BigInt<T,C>::operator+(const BigInt<T,C> &other){
	if(this->count == 0) //case when "this" is a BigInt with value '0'
		return other;
	if(other.count == 0) //case when "other" is a BigInt with value '0'
		return *this;
	BigInt<T,C> returner;
	returner.direction = 0; //after addition, all BigInts are stored in reverse
	if(this->sign == other.sign){
		bool bigger = (this->count >= other.count? 1: 0); //find the larger BigInt
		if(this->direction == other.direction){
			if(this->direction){ //both BigInts are stored in forwards order
				if(bigger){ //if 'this' BigInt has a greater/equal count
					addHelper(this->container.prev(this->container.end()),
						 	  this->container.prev(this->container.begin()),
						  	  other.container.prev(other.container.end()),
						 	  other.count,
							  this->direction, other.direction, returner, 1);
				}else //executes when 'other' has a greater count
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}	
			else{ //case when both BigInts are stored in reverse order
				if(bigger)
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
		}
		else{ //when the BigInts are stored in opposite directions
			if(this->direction){ //'this' is stored forwards, other is backwards
				if(bigger) //if 'this' BigInt has a greater/equal count
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else //executes when 'other' has a greater count
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
			else{ // 'this' is stored backwards, and other is stored forwards
				if(bigger) //if 'this' BigInt has a greater/equal count
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else //execures when 'other' has a greater count
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
		}
		returner.sign = this->sign;
		return returner;
	}//if nothing returned, do subtraction-like operations using 9's complement
	else{
		if(this->absEquals(other))
			return BigInt();
		if(this->sign && !other.sign){
			if(this->absGreaterThan(other)){
				if(this->direction == other.direction){
					if(this->direction){
						addHelper(this->container.prev(this->container.end()),
								  this->container.prev(this->container.begin()),
								  other.container.prev(other.container.end()),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
					else{
						addHelper(this->container.begin(),
								  this->container.end(),
								  other.container.begin(),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
				}
				else{
					if(this->direction){
						addHelper(this->container.prev(this->container.end()),
								  this->container.prev(this->container.begin()),
								  other.container.begin(),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
					else{
						addHelper(this->container.begin(),
								  this->container.end(),
								  other.container.prev(other.container.end()),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
				}
				returner.sign = 1;
			}
			else{
				if(this->direction == other.direction){
					if(this->direction){
						addHelper(other.container.prev(other.container.end()),
								  other.container.prev(other.container.begin()),
								  this->container.prev(this->container.end()),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
					else{
						addHelper(other.container.begin(),
								  other.container.end(),
								  this->container.begin(),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
				}
				else{
					if(this->direction){
						addHelper(other.container.prev(other.container.end()),
								  other.container.prev(other.container.begin()),
								  this->container.begin(),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
					else{
						addHelper(other.container.begin(),
								  other.container.end(),
								  this->container.prev(this->container.end()),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
				}
				returner.sign = 0;
			}
		}
		else{ //when this is negative and other is positive
			if(this->absGreaterThan(other)){
				if(this->direction == other.direction){
					if(this->direction){
						addHelper(this->container.prev(this->container.end()),
								  this->container.prev(this->container.begin()),
								  other.container.prev(other.container.end()),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
					else{
						addHelper(this->container.begin(),
								  this->container.end(),
								  other.container.begin(),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
				}
				else{
					if(this->direction){
						addHelper(this->container.prev(this->container.end()),
								  this->container.prev(this->container.begin()),
								  other.container.begin(),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
					else{
						addHelper(this->container.begin(),
								  this->container.end(),
								  other.container.prev(other.container.end()),
								  other.count,
								  this->direction, other.direction, returner, 2);
					}
				}
				returner.sign = 0;
			}
			else{
				if(this->direction == other.direction){
					if(this->direction){
						addHelper(other.container.prev(other.container.end()),
								  other.container.prev(other.container.begin()),
								  this->container.prev(this->container.end()),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
					else{
						addHelper(other.container.begin(),
								  other.container.end(),
								  this->container.begin(),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
				}
				else{
					if(this->direction){
						addHelper(other.container.prev(other.container.end()),
								  other.container.prev(other.container.begin()),
								  this->container.begin(),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
					else{
						addHelper(other.container.begin(),
								  other.container.end(),
								  this->container.prev(this->container.end()),
								  this->count,
								  other.direction, this->direction, returner, 2);
					}
				}
				returner.sign = 1;
			}
		}
		return returner;
	}
}

template <typename T, template <typename> typename C>
const BigInt<T,C> BigInt<T,C>::operator-(const BigInt<T,C> &other){
	BigInt<T,C> returner;
	if(*this == other)
		return returner;
	bool bigger = (this->count >= other.count? 1: 0); //find the larger BigInt
	returner.direction = 0; //after subtraction, all BigInts are stored in reverse
	if(this->sign && other.sign){ // CASE 1 -- SIGNS ARE BOTH POSITIVE
		if(*this == other) return returner;
		if(this->direction == other.direction){ // if both are going in the same direction
			if(this->direction){ //both are stored in forwards order
				if(*this > other){
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 2);
				returner.sign = 1;
				}
				else{
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 2);
				returner.sign = 0;
				}
			}
			else{
				if(*this > other){
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 2);
				returner.sign = 1;
				}
				else{
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 2);
				returner.sign = 0;
				}
			}
		}
		else{ //when they are going in opposite directions, but still both positive
			if(this->direction){ // this is positive direction
				if(*this > other){
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.end()),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 2);
				returner.sign = 1;
				}
				else{
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 2);
				returner.sign = 0;
				}
			}
			else{ // this is in negative direction, other is going in the positive direction
				if(*this > other){
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 2);
					returner.sign = 1;
				}
				else{
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 2);
					returner.sign = 0;
				}
			}
		}	
	}
	else if(this->sign && !other.sign){ //this is positive, the other is negative
		if(this->direction == other.direction){ //both are stored forwards
			if(this->direction){
				if(bigger)
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
			else{ //both are stored in the backwards direction
				if(bigger)
					addHelper(this->container.end(),
							  this->container.begin(),
							  other.container.end(),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.end(),
							  other.container.begin(),
							  this->container.end(),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
		}
		else{ //if the directions are not equal
			if(this->direction){ // if this is forwards and the other is backwards
				if(bigger)
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
			else{ //if this is backwards and the other is forwards
				if(bigger)
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
		}
		// addHelper regular (..., 1) (CASE 2)
		returner.sign = 1;
	}
	else if(!(this->sign) && other.sign){ //if this is negative and the other is positive
		if(this->direction == other.direction){ //both are going in the same direction
			if(this->direction){ //if both are going forwards
				if(bigger)
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
			else{ //if both are stored backwards
				if(bigger)
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 1);
				else
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 1);
			}
		}
		else if(this->direction){ //this is stored forwards, other is stored backwards
			if(bigger)
				addHelper(this->container.prev(this->container.end()),
						  this->container.prev(this->container.begin()),
						  other.container.begin(),
						  other.count,
						  this->direction, other.direction, returner, 1);
			else
				addHelper(other.container.begin(),
						  other.container.end(),
						  this->container.prev(this->container.end()),
						  this->count,
						  other.direction, this->direction, returner, 1);

		}
		else{ //this is stored backwards, other is stored forwards
			if(bigger)
				addHelper(this->container.begin(),
						  this->container.end(),
						  other.container.prev(other.container.end()),
						  other.count,
						  this->direction, other.direction, returner, 1);
			else
				addHelper(other.container.prev(other.container.end()),
						  other.container.prev(other.container.begin()),
						  this->container.begin(),
						  this->count,
						  other.direction, this->direction, returner, 1);
		}
		// addHelper regular (..., 1) (CASE 3)
		returner.sign = 0;
	}
	else{ //if both of the numbers are negative
		if(*this == other)
			return returner;
		if(this->absGreaterThan(other)){ //
			if(this->direction == other.direction){ // if both are going in the same direction
				if(this->direction) // if both are stored forwards
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 2);
				else // if both are stored backwards
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 2);
			}
			else{ //if both are going in opposing directions
				if(this->direction) //if this is forwards, other is backwards
					addHelper(this->container.prev(this->container.end()),
							  this->container.prev(this->container.begin()),
							  other.container.begin(),
							  other.count,
							  this->direction, other.direction, returner, 2);
				else //if this is backwards, other is forwards
					addHelper(this->container.begin(),
							  this->container.end(),
							  other.container.prev(other.container.end()),
							  other.count,
							  this->direction, other.direction, returner, 2);
			}
			returner.sign = 0;
		}
		else{ // if other >= this
			if(this->direction == other.direction){ // if both are going in the same direction
				if(this->direction) //if both are stored forwards
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.prev(this->container.end()),
							  this->count,
							  other.direction, this->direction, returner, 2);
				else //if both are stored backwards
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 2);
			}
			else{ //if both are going in opposing directions
				if(this->direction) //if this is forwards, other is backwards
					addHelper(other.container.begin(),
							  other.container.end(),
							  this->container.prev(this->container.begin()),
							  this->count,
							  other.direction, this->direction, returner, 2);
				else //if this is backwards, other is forwards
					addHelper(other.container.prev(other.container.end()),
							  other.container.prev(other.container.begin()),
							  this->container.begin(),
							  this->count,
							  other.direction, this->direction, returner, 2);
			}
			returner.sign = 1;
		}
	}
	return returner;
}

template <typename T, template <typename> typename C>
const BigInt<T,C> BigInt<T,C>::operator+=(const BigInt<T,C> &other){
	*this = *this + other;		
	return *this;
}

template <typename T, template <typename> typename C>
const BigInt<T,C> BigInt<T,C>::operator-=(const BigInt<T,C> &other){
	*this = *this - other;
	return *this;
}

template <typename T, template <typename> typename C>
const BigInt<T,C>& BigInt<T,C>::operator=(const BigInt<T,C> &other){
	this->container = other.container;
	this->count = other.count;
	this->sign = other.sign;
	this->direction = other.direction;
	return *this;
}

template <typename T, template <typename> typename C>
bool BigInt<T,C>::absEquals(const BigInt<T,C> &other) const noexcept{
	if(this->count == other.count){
		typename C<T>::Iterator travThis;
		typename C<T>::Iterator travOther;
		typename C<T>::Iterator stopHere;

		if(this->direction == other.direction){
			travThis = this->container.prev(this->container.end());
			travOther = other.container.prev(other.container.end());
			stopHere = this->container.prev(this->container.begin());
			while(travThis != stopHere){
				if(*travThis != *travOther)
					return false;
				--travThis;
				--travOther;
			}
			return true;
		}
		else{
			travThis = this->container.prev(this->container.end());
			travOther = other.container.begin();
			stopHere = this->container.prev(this->container.begin());
			while(travThis != stopHere){
				if(*travThis != *travOther)
					return false;
				--travThis;
				++travOther;
			}
			return true;
		}
	}
	return false;
}

template <typename T, template <typename> typename C>
bool BigInt<T,C>::operator==(const BigInt<T,C> &other) const noexcept{
	if(this->sign != other.sign)
		return false;
	return absEquals(other);
}

template <typename T, template <typename> typename C>
bool BigInt<T,C>::operator!=(const BigInt<T,C> &other) const noexcept{
	return !(*this == other);
}

template <typename T, template <typename> typename C>
bool BigInt<T,C>::absGreaterThan(const BigInt<T,C> &other) const noexcept{
	if(this->count > other.count)
		return true;
	else if(other.count > this->count)
		return false;
	else{
		typename C<T>::Iterator travThis;
		typename C<T>::Iterator travOther;
		typename C<T>::Iterator stopHere;
		
		if(this->direction == other.direction){
			if(this->direction){
				travThis = this->container.begin();
				travOther = other.container.begin();
				stopHere = this->container.end();
				while(travThis != stopHere){
					if(*travThis > *travOther)
						return true;
					else if(*travOther > *travThis)
						return false;
					++travThis;
					++travOther;
				}
			}
			else{	
				travThis = this->container.prev(this->container.end());
				travOther = other.container.prev(other.container.end());
				stopHere = this->container.prev(this->container.begin());
				while(travThis != stopHere){
					if(*travThis > *travOther)
						return true;
					else if(*travOther > *travThis)
						return false;
					--travThis;
					--travOther;
				}
			}
		}
		else{
			if(this->direction){
				travThis = this->container.begin();	
				travOther = other.container.prev(other.container.end());
				stopHere = this->container.end();
				while(travThis != stopHere){
					if(*travThis > *travOther)
						return true;
					else if(*travOther > *travThis)
						return false;
					++travThis;
					--travOther;
				}
			}
			else{
				travThis = this->container.prev(this->container.end());	
				travOther = other.container.begin();
				stopHere = this->container.prev(this->container.begin());
				while(travThis != stopHere){
					if(*travThis > *travOther)
						return true;
					else if(*travOther > *travThis)
						return false;
					--travThis;
					++travOther;
				}
			}
		}
		return false;
	}
}

template <typename T, template <typename> typename C>
bool BigInt<T,C>::operator>(const BigInt<T,C> &other) const noexcept{
	if(*this != other && !(*this < other))
		return true;
	return false;
}

template <typename T, template <typename> typename C>
bool BigInt<T,C>::operator<(const BigInt<T,C> &other) const noexcept{
	if(this->sign && !(other.sign))
		return false;
	if(!(this->sign) && other.sign)
		return true;
	if(this->count < other.count)
		return (this->sign? 1: 0);
	else if(other.count < this->count)
		return (this->sign? 0: 1);
	else{
		typename C<T>::Iterator travThis;
		typename C<T>::Iterator travOther;
		typename C<T>::Iterator stopHere;
		
		if(this->direction == other.direction){
			if(this->direction){
				travThis = this->container.begin();
				travOther = other.container.begin();
				stopHere = this->container.end();
				while(travThis != stopHere){
					if(*travThis < *travOther)
						return (this->sign? 1: 0);
					else if(*travOther < *travThis)
						return (this->sign? 0: 1);
					++travThis;
					++travOther;
				}
			}
			else{	
				travThis = this->container.prev(this->container.end());
				travOther = other.container.prev(other.container.end());
				stopHere = this->container.prev(this->container.begin());
				while(travThis != stopHere){
					if(*travThis < *travOther)
						return (this->sign? 1: 0);
					else if(*travOther < *travThis)
						return (this->sign? 0: 1);
					--travThis;
					--travOther;
				}
			}
		}
		else{
			if(this->direction){
				travThis = this->container.begin();	
				travOther = other.container.prev(other.container.end());
				stopHere = this->container.end();
				while(travThis != stopHere){
					if(*travThis < *travOther)
						return (this->sign? 1: 0);
					else if(*travOther < *travThis)
						return (this->sign? 0: 1);
					++travThis;
					--travOther;
				}
			}
			else{
				travThis = this->container.prev(this->container.end());	
				travOther = other.container.begin();
				stopHere = this->container.prev(this->container.begin());
				while(travThis != stopHere){
					if(*travThis < *travOther)
						return (this->sign? 1: 0);
					else if(*travOther < *travThis)
						return (this->sign? 0: 1);
					--travThis;
					++travOther;
				}
			}
		}
		return false;
	}	
}