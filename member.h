// member.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
// http://madebits.com                                 //
/////////////////////////////////////////////////////////////

#ifndef MEMBER_H
#define MEMBER_H
#include <utility>
#include <string>
#include <iostream>

namespace com_vpcepa{
namespace actionGroup{
using namespace std;
/**
 *  Member class wraps the pointers of different member objects (like Menu*).
 *  Member class also tries to be Nice (actually it is extra-Nice).
 *  This is not required for its specific object types (like Menu).
 *  Note that a member objects does not own the specific member objects.
 *
 *  Each member object class T must have these methods:
 *  string (T::*pf)() e.g., string Menu::getName();
 *  void (T::*pf)(bool) e.g., void Menu::setState(bool newState);
 *
 *  Also operator<< must be defined for specific member objects T.
 * 
 *  Member wrapper class keeps only a pointer, thus there is not any significant size grow.
 */
template <typename T>
class Member; 

/**
 * If you put this declaration of operator<< after the Member class,
 * logically nothing changes.
 * But bcc32 compiler will give a linker error in such case.
 * gcc on the other hand is not affected at all by this.
 */
template <typename T>
ostream& operator<<(ostream& os, const Member<T>& m){
		return os << "Member[" << *(static_cast<T*>(m)) << "]";
} 

template <typename T>
class Member{
	T* member;
	void copyThis(const Member&) throw();

	static string (T::*mname)(); 
	static void (T::*mstate)(bool);
public:
	// to be called before all other methods
	static void setNameMethod(string (T::*)());
	static void setStateMethod(void (T::*)(bool));
	bool isInitialized();	
public:
	Member() throw(domain_error);
	Member(T*) throw(domain_error);
	Member(const Member<T>&) throw();
	Member<T>& operator= (const Member<T>&) throw();
	bool operator== (const Member<T>&) const throw();
	bool operator!= (const Member&) const throw();
	bool operator< (const Member<T>&) const throw();
	operator T* () const; // used to get the member
	// T* getMember() const throw();
	string getName() const;
	Member<T> activate(bool) const throw();
	~Member() throw();
	friend ostream& operator<< <T>(ostream&, const Member<T>&);
};

//------------------------------------------------------------

// Well, below is the way of C++ to specify that Member<T> class
// uses two functions 'name' and 'state', which should be present in the
// specific member objects. Member<t> should be told which these
// functions are. Their signature in the specific classes must look like this:
//
// string (T::*pf)() e.g., string Menu::getName();
// void (T::*pf)(bool) e.g., void Menu::setState(bool newState);
//
// These two functions are required to be present in the specific member classes.
// Using pointers instead of names allows greater flexibility, since this two
// methods can have then any name in the specific objects classes. Only their signatures
// must much the ones above.
//
// The static methods shown below should be called to specify these two functions
// before any Member<T> could be created. (If not an exception will be thrown by
// the constructor).
// Member<T>::setNameMethod()
// Member<T>::setStateMethod()

template <typename T>
string (T::*Member<T>::mname)() = 0;

template <typename T>
void (T::*Member<T>::mstate)(bool) = 0;

template <typename T>
void Member<T>::setNameMethod(string (T::*pf)()){
	Member<T>::mname = pf;
}

template <typename T>
void Member<T>::setStateMethod(void (T::*pf)(bool)){
	Member<T>::mstate = pf;
}

template <typename T>
string Member<T>::getName() const {
	return (member->*Member<T>::mname)();
}

template <typename T>
Member<T> Member<T>::activate(bool newState) const throw() {
	if(member != 0)
		(member->*Member<T>::mstate)(newState);
	return *this;
}

template <typename T>
bool Member<T>::isInitialized(){
	return (Member<T>::mname != 0 && Member<T>::mstate != 0);
}

//------------------------------------------------------------

template <typename T>
Member<T>::Member() throw(domain_error) : member(0) {
	if(!isInitialized())
		throw domain_error("#Error: Set 'name' and 'state' methods first!");
}

template <typename T>
Member<T>::Member(T* m) throw(domain_error) : member(m) {
	if(!isInitialized())
		throw domain_error("#Error: Set 'name' and 'state' methods first!");
}

template <typename T>
Member<T>::~Member() throw() {}

template <typename T>
Member<T>::Member(const Member<T>& m) throw() {
	copyThis(m);
}

template <typename T>
void Member<T>::copyThis(const Member<T>& m) throw() {
	if(this != &m)
		member = static_cast<T*>(m);
}

template <typename T>
Member<T>& Member<T>::operator= (const Member<T>& m) throw() {
	copyThis(m);
	return *this;
}

template <typename T>
bool Member<T>::operator== (const Member<T>& m) const throw() {
	return getName() == m.getName();
} 

template <typename T>
bool Member<T>::operator!= (const Member& m) const throw() {
	return getName() != m.getName();
} 

template <typename T>
bool Member<T>::operator< (const Member<T>& m) const throw() {
	if(member == 0) return true;
	return (getName() < m.getName());
}

template <typename T>
Member<T>::operator T*() const {
	return member;
}

}}
#endif
//EOF
