// vutils.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#ifndef VUTILS_H
#define VUTILS_H

#include <ctime>
#include <typeinfo>
#include <fstream>
#include <cstdlib>
#include <string>
#include <strstream>

namespace com_vpcepa{
namespace utils{
using namespace std;

template <typename T>
class Number{
public:
static string toString(T);
static T parseNumber(const char*);
};

template <typename T>
string Number<T>::toString(T t){
	ostrstream o;
	o << t;
	return (string)o.str();
}

template <typename T>
T Number<T>::parseNumber(const char * s){
	return atof(s);
}

/** A log class. */
class VLog : public ofstream {
	string fname;
public:
	VLog(char *name);
	char* writeTimeStamp();
	string& name();
};

string trim(string);
string trim(const char*);

}}
#endif
//EOF