// gmanager-demo.cpp
// gmanager demo
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                 //
/////////////////////////////////////////////////////////////

#include "menu.h"
#include "membercollection.h"
#include "parseclusters.h"
#include <iostream>

using namespace com_vpcepa::actionGroup;
using namespace com_vpcepa::utils;
using namespace std;

// The Menu class (menu.h, menu.cpp) will be used in this demo
// as a specific Member<T> object. The Menu class defines the
// requested methods string Menu::getName() and void setState(bool).
// Also it has operator << defined.


/** generate some menus automatically for this demo */
typedef vector<Member<Menu> > MVECTOR;

template<typename T>
class Gen{
	int i;
	public:
	Gen() : i(0) {}
	Member<T> operator()();
};

template<typename T>
Member<T> Gen<T>::operator()(){
	return Member<T>(new T("m" + Number<int>::toString(i++)));	
}

void unexpected_h() {
	cerr << "#GManager DEMO - unexpected exception";
	exit(1);
}

int main(){
	set_unexpected(unexpected_h);
	cout << "\n* START OF GROUPS MANAGER DEMO *" << endl;

	// using an action log file:
	VLog log("gmanager-demo.log");

	// this is the first thing to do before
	// using any of the other gmanager objects !!!
	Member<Menu>::setNameMethod(&Menu::getName);
	Member<Menu>::setStateMethod(&Menu::setState);

	MVECTOR v;
	v.reserve(10);
	generate_n(back_inserter(v), 10, Gen<Menu>());
	cout << "Our 10 MENUS:\n";
	copy(v.begin(), v.end(), ostream_iterator<Member<Menu> >(cout, "\n"));

	// a member collection is a class to organize the
	// member resources. It keeps the members and a
	// GroupManager associated with them.
	// The GroupManger<T> can be used directly with Member<T>
	// without this class. But using it is handy.
	MemberCollection<Menu> mc;
	mc.setLog(log);
	mc.canLog(true);

	MVECTOR::iterator i00 = v.begin();
	while(i00 != v.end()){
		mc.add((*i00));
		i00++;
	}
	// alternative ways of doing it:
	Member<Menu> m10(new Menu("m10"));
	mc.add(m10);
	//mc.add(new Menu("m11"));

	//this is how to assess the original Menu objects based on Member<T>
	Menu* tempMenu2 = static_cast<Menu *>(v[2]); 
 	cout << "\nAccessing Menu2: " << *tempMenu2 << endl;


	// declare some relations between members.
	// the manual way:
	GroupManager<Menu>& gm = static_cast<GroupManager<Menu>&>(mc);
	gm.declareFriend(v[1],v[2]);
	gm.declareEnemy(v[1],v[3]);
	gm.declareMutualFriend(v[2],v[3]);

	cout << "\n1 - Here is like our MemberCollection looks now:\n" << mc;

	// Now lets remove a Member that we need no more.
	// It will be removed also from all groups of GroupManager gm.

	MVECTOR* pv = mc.getMembersVector(); // handy way to get all members
	Member<Menu> mm0 = pv->at(0); 
	mc.remove(mm0);
	mc.deleteMembersVector(pv); // do NOT forget to delete the vector returned.

	cout << "\n2 - After delete of element at 0:\n" << mc;
	cout << "\n3 - Testing copy:\n";

	// a new empty collection
	MemberCollection<Menu> mc2;
	mc2 = mc;
	cout << "\nA copy of member collection:\n" << mc2;

	cout << "\n4 - Removing an item from copy does not effect the original:\n";
	cout << "Removing m2:\n";
	// another handy way of doing it:
	Member<Menu> tm = mc2.findMember(string("m2"));
	mc2.remove(tm);

	cout << "\nNow mc and mc2 look like this:\n"
	"(note that every different collection has a different numeric id)\n";
	cout << mc << mc2;

	// We need to include 'parseclusters.h' for this:
	cout << "\n5 - Reseting GroupManager at mc with relations\n"
		"from an external action file 'menus.txt'.\n"
		"Note that only members that exists are effected by this.";

	gm.reset();        // clear all current relations.
	mc.getLog() << "\nWriting gm to log file:\n" << gm << endl; // this how we can write log messages!
	parseAction<Menu>(gm, static_cast<MemberCollection<Menu>::MAP&>(mc), "menus.txt");
	cout << mc << mc2;

	// demonstrating group activation
	cout << "\n\nActivating group that has m3" << endl;
	gm.exclusiveActivate(v[3]);
	cout << gm;
	cout << "All disabled: "<< endl;
	gm.deactivate(); // all
	cout << gm;

	cout << "\n\nEND" << endl;

	// cleaning up
	MVECTOR::iterator i01 = v.begin();
	while(i01 != v.end()){
		delete static_cast<Menu*>(*i01);
		i01++;
	}
}
// EOF