// membercollection.cpp
/////////////////////////////////////////////////////////////
// GroupManager - Group Deduction System                   //
// (c) Copyright by Vasian CEPA. All rights reserved.      //
// http://madebits.com                                 //
/////////////////////////////////////////////////////////////

template <typename T>
MemberCollection<T>::MemberCollection(const MemberCollection<T>& mc){
	copyThis(mc);
}

template <typename T>
MemberCollection<T>& MemberCollection<T>::operator= (const MemberCollection<T>& mc){
	copyThis(mc);
	return *this;
}

template <typename T>
void MemberCollection<T>::copyThis(const MemberCollection<T>& mc){
	if(canLog()) getLog() << "MC: ID(" <<  id << ") Creating a Copy!" << endl;
	if(this != &mc){
		m = mc.m;
		gm = mc.gm;
	}
}

template <typename T>
MemberCollection<T>::RESULT MemberCollection<T>::add(T * t) throw(domain_error) {
	try{
		add(MEMBER(t));
	}catch(domain_error){
		throw;
	}
}

template <typename T>
MemberCollection<T>::RESULT MemberCollection<T>::add(Member<T>& mm) throw(domain_error) {
	if(canLog()) getLog() << "MC: ID(" <<  id << ") Adding " << mm.getName() << endl;
	MAP::iterator ii = m.find(mm.getName());
	if(ii != m.end()){
		throw domain_error("#Error: Already member in collection!");
	}
	return m.insert(make_pair(mm.getName(), mm));
}

template <typename T>
MemberCollection<T>::operator MemberCollection<T>::MAP& (){ 
	return m;
}

template <typename T>
MemberCollection<T>::operator GroupManager<T>& () { return gm; }

template <typename T>
MemberCollection<T>::VECTOR* MemberCollection<T>::getMembersVector(){
	if(canLog()) getLog() << "MC: ID(" <<  id << ") Returning a vector - do NOT forget to delete it!" << endl;
	vector<MEMBER >* v = new vector<MEMBER >();
	MAP::iterator ii = m.begin();
	while(ii != m.end()){
		v->push_back((*ii).second);
		ii++;
	}
	return v;
}

template <typename T>
void MemberCollection<T>::deleteMembersVector(const MemberCollection<T>::VECTOR* v){
	if(canLog()) getLog() << "MC: ID(" <<  id << ") Deleting a previously returned vector!" << endl;
	if(v != 0)
		delete v;
}

template <typename T>
void MemberCollection<T>::print(ostream & os){
	const VECTOR* v = getMembersVector();
	os << "\n<<************************************";
	os << "\nMCOLLECTION ID(" << id << ") START [[";
	os << "\n\nMembers ("<< v->size() << ") :\n";
	os << "<<--------------\n";
	copy(v->begin(), v->end(), ostream_iterator<MEMBER >(os, "\n"));
	os << "--------------->>\n";
	deleteMembersVector(v);
	os << "\n\n" << gm;
	os << "\n]] MCOLLECTION END";
	os << "\n*************************************>>\n";
}

template <class T>
struct RemoveGroup : public binary_function<Group<T>*, Member<T>, bool> {
	bool operator()(const Group<T>* g, const Member<T>& m) const;
};

template <class T>
bool RemoveGroup<T>::operator()(const Group<T>* g, const Member<T>& m) const {
	Group<T>* tg = const_cast<Group<T>* >(g);
	if(tg->has(m)){
		tg->remove(m);
		if(tg->size() == 0){
			delete tg;
			return true;
		}
	}
	return false;
}

/**
 * If ever needed to manipulate groups directly this function can serve
 * as an example of doing it, correctly.
 * It removes e Member<T> m, from the MemberCollection only after it is removed from all
 * groups that contain it in the given GroupMananger;
 */
template <typename T>
void MemberCollection<T>::remove(const MEMBER& mm) throw(domain_error) {
	if(canLog()) getLog() << "MC: ID(" <<  id << ") Removing " << mm.getName() << endl;

	MAP::iterator im = m.find(mm.getName());
	if(im == m.end()){
		throw domain_error("#Error: Member not found in collection!");
	}

	GroupManager<T>::GMList& lst = static_cast<GroupManager<T>::GMList&>(gm);
	GroupManager<T>::GMIterator ii = remove_if(
		lst.begin(),
		lst.end(),
		bind2nd(RemoveGroup<T>(), mm));
	lst.erase(ii, lst.end());
	m.erase(im);
}

template <typename T>
MemberCollection<T>::MEMBER& MemberCollection<T>::findMember(string& name) throw(domain_error) {
	MAP::iterator im = m.find(name);
	if(im == m.end()) throw domain_error("#Error: Member not found in collection!");
	else return (*im).second;	
}

// we log to the same file as GroupManager here:

template <typename T>
void MemberCollection<T>::setLog(ostream& log) { gm.setLogStream(log); } 

template <typename T>
ostream& MemberCollection<T>::getLog(){ return gm.getLogStream(); }

template <typename T>
void MemberCollection<T>::canLog(bool cl) { gm.log(cl); }

template <typename T>
bool MemberCollection<T>::canLog() { return gm.log(); };

//EOF