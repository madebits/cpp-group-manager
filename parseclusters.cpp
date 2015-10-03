// parseclusters.cpp
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
// http://madebits.com                                 //
/////////////////////////////////////////////////////////////

#include "parseclusters.h"

using namespace com_vpcepa::utils;
using namespace com_vpcepa::actionGroup;
using namespace std;

ParseClusters::ParseClusters(const char * fileName) : file(fileName), in(fileName) {}

ParseClusters::iterator ParseClusters::begin() { return iterator(this); }

ParseClusters::iterator ParseClusters::end() const { return iterator(); }

ParseClusters::ActionToken* ParseClusters::parseLine(string line, unsigned int lineNumber) throw (domain_error){
	ActionToken* token = new ActionToken();

	if(trim(line) == "") return token;

	bool fCommentOnly = false;
	string::size_type ci = line.find("#");
	if(ci == string::npos){ // there is not comment on this line
		ci = line.length();
	} else {
		token->comment = line.substr(ci);
		if(trim(line.substr(0, ci)) == ""){
			fCommentOnly = true;
		}
	}

	string::size_type i1 = line.find("<");
	if(i1 == string::npos || i1 > ci){
		if(!fCommentOnly){
			// no '<' then abort
			//cout << "#parse error: " + Number<int>::toString(lineNumber) << endl;
			delete token;
			throw domain_error("#Parse error at line:" + Number<int>::toString(lineNumber) + " [" + line + "]");
		}
	} else {
		token->name1 = trim(line.substr(0, i1));
	}

	string::size_type i2 = line.find(">", i1);
	if(i2 == string::npos || i2 > ci){
		if(!fCommentOnly){
			// no '>' then abort
			//cout << "#parse error: " + Number<int>::toString(lineNumber) << endl;
			delete token;
			throw domain_error("#Parse error at line:" + Number<int>::toString(lineNumber) + " [" + line + "]");
		}
	} else {
		i2++; // skip '>'
		token->name2 = trim(line.substr(i2, ci - i2));
		string temp = trim(line.substr(i1, i2 - i1));
		if(temp == "<+>"){
			token->oper = ActionToken::FRIEND;
		} else if(temp == "<->"){
			token->oper = ActionToken::ENEMY;
		} else if(temp == "<*>"){
			token->oper = ActionToken::MUTUAL_FRIEND;
		} else if(temp == "<%>"){
			token->oper = ActionToken::ANTI_ENEMY;
		} else {
			delete token;
			throw domain_error("#Parse error at line:" + Number<int>::toString(lineNumber) + " [" + line + "]");
		}
	}

	return token;
}


ParseClusters::iterator::iterator(ParseClusters* p) : token(0), parser(p), lineNumber(0){
	operator++();
}

bool ParseClusters::iterator::operator++ (){
	string line;
	token = 0;
	bool next = false;
	while(getline(parser->in, line)){
		lineNumber++;
		try {
			token = ParseClusters::parseLine(line, lineNumber);
		} catch(domain_error e){
			cerr << parser->file << e.what() << endl; 
		}
		if(token != 0 && (token->name1 != "" && token->name2 != "")){
			next = true;
			break;
		} else {
			if(token != 0)
				delete token;
				token = 0;
		}
	}
	if(!next){
		if(lineNumber == 0){
			cerr << parser->file << "#Error: File not found or corrupted!" << endl;
		}
		parser = 0;
	}
	return next;
}

//EOF