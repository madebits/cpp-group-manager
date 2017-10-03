// membercollection.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#ifndef MEMBER_COLLECTION_H
#define MEMBER_COLLECTION_H
#include "groupmanager.h"
#include <map>
#include <vector>
#include <exception>
#include <string>
#include <iostream>

namespace com_vpcepa{
namespace actionGroup{
using namespace std;

/**
 * The class MemberCollection serves a handy way to group together Members with a GroupManager (GM).
 * Even thought the GM class can be used without this class, this class has some methods that
 * facilitate use of GM.
 * As implemented here, a collection of members has only one GM connected with them.
 * If you need more than one GM you should build a new MemberCollection from an existing one, with
 * a copy contructor/operator and reset its GM. Alternatively, you can implement a class such as this
 * one that works with multiple copies of GM-s.
 */
template <typename T>
class MemberCollection{
protected:
	static unsigned int counter;
public:
	typedef Member<T> MEMBER;
	typedef map<string, MEMBER > MAP;
	typedef pair<MAP::iterator, bool> RESULT;
	typedef vector<MEMBER > VECTOR;
	const unsigned int id;
private:
	MAP m;
	GroupManager<T> gm;
	void copyThis(const MemberCollection<T>&);
public:
	MemberCollection() : id(++(MemberCollection<T>::counter)) {}
	MemberCollection(const MemberCollection<T>&);
	~MemberCollection(){};
	MemberCollection<T>& operator= (const MemberCollection<T>&);
	RESULT add(T *) throw(domain_error);
	RESULT add(Member<T>&) throw(domain_error);
	void remove(const MEMBER&) throw(domain_error);
	MEMBER& findMember(string&) throw(domain_error);

	VECTOR* getMembersVector();
	void deleteMembersVector(const VECTOR*);

	operator MAP& ();
	operator GroupManager<T>& ();

	void print(ostream&);
    	friend ostream& operator<< <T>(ostream&, GroupManager<T>&);

	void setLog(ostream&);
	ostream& getLog();
	void canLog(bool);
	bool canLog();
};

template <typename T>
unsigned int MemberCollection<T>::counter = 0;

template <typename T>
ostream& operator<<(ostream& os, MemberCollection<T>& mc){
	mc.print(os);
	return os;
}

#include "membercollection.cpp"

}}
#endif
//EOF