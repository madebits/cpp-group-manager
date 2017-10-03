// menu.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#ifndef MENU_H
#define MENU_H
#include <string>
#include <iostream>

namespace com_vpcepa{
namespace actionGroup{
using namespace std;
/**
 *  A specific member class that represents a simple menu-item model.
 *  It shows how used specific control objects (menus) can be used with
 *  this synchronizing system. In order to be used with the rest of generic code
 *  a menu class should only expose two required actions (methods), one for reading
 *  its unique string ID, that can have any name but this signature:
 *    string getName();
 *  and one for settings its state, that can also have any name but this signature:
 *    void setState(bool aState);
 *  See 'member.h' wrapper class for more details.
 */
class Menu{
	string mName;   // also unique menu id here
	mutable bool mState; // true means enabled
	string mAction; // fake action, a real class must save a functon pointer here, or a list of such
	void copyThis(const Menu&);
public:
	Menu() : mName(""), mAction("no action"), mState(true) {}
	Menu(string = "", string aAction = "no action", bool = true);
	Menu(const Menu&);
	Menu& operator= (const Menu&);
	// accessors
	string getName() const { return mName; }
	bool getState() const  { return mState;}
	string getAction() const { return mAction; }
	// mutators
	void setState(bool aState);
	void setName(string);
	void setAction(string);
	friend ostream& operator<<(ostream&, const Menu&);
};

ostream& operator<<(ostream&, const Menu&);

}}
#endif
//EOF
