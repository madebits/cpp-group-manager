// groupmanager.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H
#include "vutils.h"
#include "group.h"
#include <list>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <iostream>

namespace com_vpcepa{
namespace actionGroup{
using namespace com_vpcepa::utils;
using namespace std;
/**
 * See manager.cpp for an example fo use.
 */
template <typename T>
class GroupManager{
public:
	typedef list <Group<T>* > GMList;
	typedef GMList::iterator GMIterator;
private:
	GMList groupList;
	void copyThis(const GroupManager<T>&);
	void purge();
	Group<T>* createNewGroup(const Member<T>&);
	GMIterator find(const Member<T>&);

	ostream* glog;
	bool flog;
	void logAction(const char*, const Member<T>&, const Member<T>&);
public:
	GroupManager() : glog(0), flog(false) {}
	GroupManager(const GroupManager&);
	~GroupManager();
	GroupManager<T>& operator= (const GroupManager<T>&);
	operator GMList& ();
	GMIterator begin() { return groupList.begin(); }	
	GMIterator end() { return groupList.end(); }
	void print(ostream&);
	void reset();
	void exclusiveActivate(const Member<T>&);
	void activate(const Member<T>&);
	void deactivate();
	void declareEnemy(const Member<T>&, const Member<T>&); 
	void declareFriend(const Member<T>&, const Member<T>&);
	void declareMutualFriend(const Member<T>&, const Member<T>&);
	void declareAntiEnemy(const Member<T>&, const Member<T>&);
    	/** Need not really to be a classic friend but I want a bound function. */
    	friend ostream& operator<< <T>(ostream&, GroupManager<T>&);      

	void setLogStream(ostream&);
	ostream& getLogStream();
	void log(bool);	
	bool log();
};

template <typename T>
ostream& operator<<(ostream& os, GroupManager<T>& gm){
	gm.print(os);
	return os;
}

#include "groupmanager.cpp"

}}
#endif
// EOF

