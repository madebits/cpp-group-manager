// parseclusters.h
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

#ifndef PARSECLUSTERS_H
#define PARSECLUSTERS_H

#include "groupmanager.h"
#include "vutils.h"
#include <map>
#include <exception>
#include <string>
#include <strstream>
#include <fstream>
#include <iostream>

namespace com_vpcepa{
namespace actionGroup{
using namespace std;
/**
 * This class is used to parse an action file, used in groupmanager.h.
 */
class ParseClusters{
	string file;
	ifstream in;
public:
	ParseClusters(const char *);

	class ActionToken{
	public:
		enum operation {FRIEND, ENEMY, MUTUAL_FRIEND, ANTI_ENEMY};
		string name1;
		string name2;
		operation oper;
		string comment;
		ActionToken() : name1(""), name2(""), oper(ActionToken::FRIEND), comment("") {}
	};

	static ActionToken* parseLine(string, unsigned int) throw (domain_error);

	class iterator;
	friend class iterator;
	class iterator{
		ParseClusters* parser;
		ActionToken* token;
		int lineNumber;
		public:
			iterator(ParseClusters*);
			iterator() : token(0), parser(0), lineNumber(0) {};
			bool operator++ ();
			bool operator++ (int i){ return operator++(); }
			bool operator==(const iterator&) const { return parser == 0; }
			bool operator!=(const iterator&) const { return parser != 0; }
			operator bool() const { return bool(parser); }
			ActionToken* current() const { if(parser) return token; else return 0;}
			ActionToken* operator*() const { return current(); }
			ActionToken* operator->() const { return current(); }
	};

	ParseClusters::iterator begin();
	ParseClusters::iterator end() const;
};

template <typename T>
void parseAction(GroupManager<T>& gm, map<string, Member<T> >& m, char * actionFile){
	ParseClusters parser(actionFile);
	ParseClusters::iterator ii = parser.begin();
	while(ii != parser.end()){
		ParseClusters::ActionToken* token = *ii;
		map<string, Member<T> >::iterator i1 = m.find(token->name1);
		map<string, Member<T> >::iterator i2 = m.find(token->name2);
		if(i1 != m.end() && i2 != m.end()){
			Member<T> m1 = (*i1).second;
			Member<T> m2 = (*i2).second;
			if(token->oper == ParseClusters::ActionToken::FRIEND){
				gm.declareFriend(m1, m2);
			} else if (token->oper == ParseClusters::ActionToken::ENEMY){
				gm.declareEnemy(m1, m2);
			} else if (token->oper == ParseClusters::ActionToken::MUTUAL_FRIEND){
				gm.declareMutualFriend(m1, m2);
			} else if (token->oper == ParseClusters::ActionToken::ANTI_ENEMY){
				gm.declareAntiEnemy(m1, m2);
			}
		}
		ii++;	
	}	
}

}}
#endif
// EOF
