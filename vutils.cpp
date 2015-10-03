// vutils.cpp
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
// http://madebits.com                                 //
/////////////////////////////////////////////////////////////

#include "vutils.h"

using namespace com_vpcepa::utils;
using namespace std;

string com_vpcepa::utils::trim(string s){
	string::size_type i1 = s.find_first_not_of(" ");
	if(i1 == string::npos)
		return string("");
	string::size_type i2 = s.find_last_not_of(" ");
	return s.substr(i1, i2 - i1 + 1);
}

string com_vpcepa::utils::trim(const char * cs){
	string s(cs);
	return com_vpcepa::utils::trim(s);
}


VLog::VLog(char * name) : fname(name) , ofstream() {
	open(fname.c_str());
	writeTimeStamp();
}

string& VLog::name(){
	return fname;
}

char * VLog::writeTimeStamp(){
	long * l = new long;
	*l = time(0);
	char* t = ctime(l);
	delete l;
	(*this) << t << endl;
	return t;
}

//EOF