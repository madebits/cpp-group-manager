// group.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#ifndef GROUP_H
#define GROUP_H
#include "member.h"
#include <vector>
#include <set>
#include <exception>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>

namespace com_vpcepa{
namespace actionGroup{
using namespace std;
/**
 * We save objects not pointers here. This may not be always preferable.
 * The implementation can be changed to make use of pointers, that is to store
 * types of Member<T>*, instead of Member<T> as it does now.
 * Since a member can not be in a group more than once, than a group is just a set.
 */
template <typename T>
class Group : public set<Member<T> > {
public:
	Group(){}
	void add(Member<T>&) throw();
	void remove(const Member<T>&) throw(domain_error);
	bool has(const Member<T>&) const throw();
	void activate(bool);
	void print(ostream& os) const;
	/** Need not really to be a classic friend but I want a bound function. */
	friend ostream& operator<< <T>(ostream&, const Group<T>&);
};

template <typename T>
ostream& operator<<(ostream& os, const Group<T>& g) {
    g.print(os);
	return os;
}

template <typename T>
void Group<T>::add(Member<T>& m) throw() {
	insert(m);
}

template <typename T>
void Group<T>::remove(const Member<T>& m) throw(domain_error){
	iterator it = this->find(m);
	if(it == end()){
		throw domain_error("#Error: Can not remove a member that does not exist!");
	} else {
		erase(it);
	}
}

template <typename T>
bool Group<T>::has(const Member<T>& m) const throw() {
	bool result = true;
	const_iterator it = this->find(m);
	if(it == end()) result = false;
	return result;
}

template <typename T>
void Group<T>::activate(bool newState) {
	vector<Member<T> > temp;
	temp.reserve(this->size());
	transform(this->begin(), this->end(), temp.begin(), bind2nd(mem_fun_ref(&Member<T>::activate), newState));
	this->insert(temp.begin(), temp.end());
}

template <typename T>
void Group<T>::print(ostream& os) const {
	os << "Group{\n";
	copy(begin(), end(), ostream_iterator<Member<T> >(os, "\n"));
	os << "}\n";
}

template <class T>
Group<T>* mergeGroups(Group<T>* g1, Group<T>* g2){
	Group<T>* g = new Group<T>();
	vector<Member<T> > tempResult;
	back_insert_iterator<vector<Member<T> > > ii(tempResult);
	set_union(g1->begin(), g1->end(), g2->begin(), g2->end(), ii);
	g->insert(tempResult.begin(), tempResult.end());
	return g;
}

}}
#endif
// EOF

