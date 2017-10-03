// groupmanager.cpp
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
//                                  //
/////////////////////////////////////////////////////////////

template <typename T>
GroupManager<T>::GroupManager(const GroupManager<T>& gm){
	copyThis(gm);
}

template <typename T>
GroupManager<T>::~GroupManager(){
	purge();
}

template <typename T>
GroupManager<T>& GroupManager<T>::operator= (const GroupManager<T>& gm){
		copyThis(gm);
		return *this;
}

template <typename T>
GroupManager<T>::operator GroupManager<T>::GMList& (){
	return groupList;
};


template <typename T>
void GroupManager<T>::purge(){
	typename GroupManager<T>::GMIterator ii = this->begin();
	while(ii != this->end()){
		Group<T>*g = *ii;
		delete g;
		*ii = 0;
		ii++;
	}
}

template <typename T>
void GroupManager<T>::copyThis(const GroupManager<T>& gm){
	if(this != &gm){
		purge();
		groupList.clear();
		GMList tl(gm.groupList); // removes const!
		GMIterator ii = tl.begin();
		while(ii != tl.end()){
			// deep copy is done here
			Group<T>* g = new Group<T>();
			*g = const_cast<Group<T> >(*(*ii));
			groupList.push_back(g);
			ii++;
		}
		glog = gm.glog; // share the same log file
		flog = gm.flog; // and log status
	}
}

template <typename T>
void GroupManager<T>::reset(){
	if(log()) (*glog) << "GM ACTION: reset" << endl;
	purge();
	groupList.clear();
}

template <class T>
struct Belong : public binary_function<Group<T>*, Member<T>, bool> {
	bool operator()(const Group<T>* g, const Member<T>& m) const {
		return g->has(m);
	}
};

template <typename T>
GroupManager<T>::GMIterator GroupManager<T>::find(const Member<T>& m) {
	return find_if(this->begin(), this->end(), bind2nd(Belong<T>(), m));
	//return find_if(this->begin(), this->end(), bind2nd(mem_fun_ref(&Group<T>::has), m));
}

template <typename T>
Group<T>* GroupManager<T>::createNewGroup(const Member<T>& m){
	// create the group {m} only if it does not exists
	Group<T>* g = 0;
	GMIterator ii = this->begin();
	while(ii != this->end()){
		if((*ii)->has(m) && ((*ii)->size() == 1)){
			g = (*ii);
		}
		ii++;
	}
	if(g == 0){
		g = new Group<T>();
		Member<T> t = m; // const removed!
		g->add(t);
		groupList.push_back(g);
	}
	return g;
}

template <typename T>
void GroupManager<T>::print(ostream& os) {
	os << "GroupManager (" << groupList.size() << ")\n";
	os << "<<--------------\n";
	GMIterator ii = this->begin();
	int i = 0;
	while(ii != this->end()){
		os << ++i << " - ";
		(*ii)->print(os);
		ii++;
	}
	os << "--------------->>\n";
}

template <typename T>
void GroupManager<T>::exclusiveActivate(const Member<T>& m){
	if(log()) (*glog) << "GM ACTION: exclusive activation of: " << m.getName() << endl;
	// same effect as this code sequence:
	// 	deactivate();
	// 	activate(m);
	// but slightly more efficient
	if(groupList.empty()){
		return;
	}
	GMIterator ii = this->begin();
	while(ii != this->end()){
		if((*ii)->has(m)){
			(*ii)->activate(true);
		} else {
			(*ii)->activate(false);
		}
		ii++;
	}
}

template <typename T>
void GroupManager<T>::activate(const Member<T>& m){
	if(log()) (*glog) << "GM ACTION: activation of: " << m.getName() << endl;
	if(groupList.empty()){
		return;
	}
	GMIterator ii = this->begin();
	while(ii != this->end()){
		if((*ii)->has(m)){
			(*ii)->activate(true);
		}
		ii++;
	}
	//GMIterator ii = this->find(m);
	//if(ii != this->end())(*ii)->activate(true);
}

template <typename T>
void GroupManager<T>::deactivate(){
	if(log()) (*glog) << "GM ACTION: deactivate all!" << endl;
	if(groupList.empty()){
		return;
	}
	for_each(this->begin(), this->end(), bind2nd(mem_fun(&Group<T>::activate), false));
}

template <typename T>
void GroupManager<T>::declareEnemy(const Member<T>& m1, const Member<T>& m2) {
	logAction("-", m1, m2);
	if( m1 == m2){
		createNewGroup(m1); // creates {m1} only if it does not exist
		return;
	}
	GMIterator i1 = this->find(m1);
	GMIterator i2 = this->find(m2);
	if((i1 != this->end()) && (i2!= this->end()) && (*i1 == *i2)){
		try{
			if((*i2)->size() > 1){
				(*i2)->remove(m1);
				createNewGroup(m1);
			}
		}catch(domain_error e){
			cerr << e.what() << endl;
		}
	}
	if(i1 == this->end()){
		createNewGroup(m1);
	}
	if(i2 == this->end()){
		createNewGroup(m2);
	}
}

template <typename T>
void GroupManager<T>::declareFriend(const Member<T>& m1, const Member<T>& m2) {
	logAction("+", m1, m2);
	if( m1 == m2){
		GMIterator i1 = this->find(m1);
		if(i1 == this->end())
			createNewGroup(m1);
		return;
	}
	GMIterator i1 = this->find(m1);
	GMIterator i2 = this->find(m2);
	if((i1 == this->end()) && (i2 == this->end())){
		Group<T>* g = createNewGroup(m1);
		Member<T> t = m2; // const removed!
		g->add(t);
	} else if((i1 != this->end()) && (i2 != this->end())) {
		if(i1 == i2){ // already same group
			return;
		}
		Group<T>* g = mergeGroups(*i1, *i2);
		// removing old groups and freeing resources
		Group<T>* t = const_cast<Group<T>* >(*i1);
		delete t;
		groupList.erase(i1);
		//i2 = this->find(m2); // reset index: not really necessary i2 is still valid
		t = const_cast<Group<T>* >(*i2);
		delete t;
		groupList.erase(i2);
		groupList.push_back(g);
	} else {
		Group<T>* g = ( i1 != this->end()) ? *i1 : *i2;
		Member<T> t = m1; // const removed!
		g->add(t);
		t = m2;
		g->add(t);
	}
}

/**
 * Implements '<*>' relation. If we have groups {m1, m2};{m3,m4}
 * then m1<*>m3 creates this new state: {m1,m2};{m1,m3,m4}.
 * In particular {m1};{m2} and m1<*>m2 brings: {m1};{m1,m2}. That is m1 goes to m2.
 * Note that this relation is NOT symmetrical:
 * m1<*>m2 != m2<*>m1
 */
template <typename T>
void GroupManager<T>::declareMutualFriend(const Member<T>& m1, const Member<T>& m2) {
	logAction("*", m1, m2);
	if( m1 == m2){
		GMIterator i1 = this->find(m1);
		if(i1 == this->end())
			createNewGroup(m1);
		return;
	}
	//GMIterator i1 = this->find(m1);
	GMIterator i2 = this->find(m2);
	Group<T>* g;
	if(i2 == this->end()){
		g = createNewGroup(m1);
	} else {
		g = *i2;
	}
	if(m1 != m2){
		Member<T> t = m1; // const removed!
		g->add(t);
	}
}

template <typename T>
void GroupManager<T>::declareAntiEnemy(const Member<T>& m1, const Member<T>& m2) {
	logAction("%", m1, m2);
	GMIterator i2 = this->find(m2);
	if(i2 == this->end()){
		createNewGroup(m2);
		return;
	}
	if((*i2)->has(m1)){
		(*i2)->remove(m1);
		if((*i2)->size() == 0){
			Group<T>* g = *i2;
			groupList.erase(i2);
			delete g;
		}
	}
}

template <typename T>
void GroupManager<T>::setLogStream(ostream& l){
	if(glog == 0){
		glog = &l;
	}
}
template <typename T> ostream& GroupManager<T>::getLogStream(){ return *glog; }
template <typename T> void GroupManager<T>::log(bool f){ flog = f; }
template <typename T> bool GroupManager<T>::log(){
	if(glog == 0) return false;
	else return flog;
}
template <typename T>
void GroupManager<T>::logAction(const char* operation, const Member<T>& m1, const Member<T>& m2){
	if(log()){
		(*glog) << "GM ACTION: " << m1.getName() << "<" << operation << ">" << m2.getName() << endl;
	}
}


//EOF