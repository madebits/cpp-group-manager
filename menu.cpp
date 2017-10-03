// menu.cpp
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#include "menu.h"

using namespace com_vpcepa::actionGroup;
using namespace std;

// full name required by the compiler
ostream& com_vpcepa::actionGroup::operator<<(ostream& os, const Menu& m){
	return os << "Menu(" << m.mName << ")=<" << ((m.mState == true) ? "true" : "false") << ">";
}

Menu::Menu(string aName, string aAction, bool aState){
	setName(aName);
	setState(aState);
	setAction(aAction);
}

Menu::Menu(const Menu& m) {
	copyThis(m);
}

void Menu::copyThis(const Menu& m) {
	if(this != &m){
		mName = m.getName();
		mState = m.getState();
		mAction = m.getAction();
	}
}

Menu& Menu::operator= (const Menu& m) {
	copyThis(m);
	return *this;
}

void Menu::setName(string aName) { mName = aName; }

void Menu::setState(bool aState) { mState = aState; }

void Menu::setAction(string aAction) { mAction = aAction; }

// EOF