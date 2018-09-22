#ifndef NOSUCHOBJECT_H
#define NOSUCHOBJECT_H

#include <stdexcept>
#include <string>

using namespace std;

class NoSuchObject:public logic_error{
  public:
	NoSuchObject():logic_error("no such object exists"){} //default constructor
	NoSuchObject(const string &what):logic_error(what){} //explicit constructor
}; //NoSuchObject

#endif
