#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>


using namespace std;




#define DEFINE_ACCESSOR_AND_MUTATOR(type, name) \
  type name() {                                 \
    return name ## _;                           \
  }                                             \
  void set_ ## name(type val) {                 \
    name ## _ = val;                            \
  }                                             \

class State {
public:
	using TransType = map<char, State *>;

	DEFINE_ACCESSOR_AND_MUTATOR(int, maxlen)
		DEFINE_ACCESSOR_AND_MUTATOR(int, minlen)
		DEFINE_ACCESSOR_AND_MUTATOR(int, first_endpos)
		DEFINE_ACCESSOR_AND_MUTATOR(State *, link)
		DEFINE_ACCESSOR_AND_MUTATOR(bool, accept)
		DEFINE_ACCESSOR_AND_MUTATOR(TransType, trans)
		DEFINE_ACCESSOR_AND_MUTATOR(bool, visited)
		DEFINE_ACCESSOR_AND_MUTATOR(int, path_num)

		bool has_trans(char c) const {
		return trans_.count(c) > 0;
	}
	State *trans(char c) const {
		return trans_.at(c);
	}
	void set_trans(char c, State *v) {
		trans_[c] = v;
	}
	~State() {
		trans_.clear();
		link_ = nullptr;
	}
private:
	int path_num_=1;//路径数
	int maxlen_ = 0;
	int minlen_ = 0;
	int first_endpos_ = -1;
	bool visited_ = false;
	State *link_ = nullptr;
	map<char, State *> trans_;

	bool accept_ = false;
};



class SAM
{
public:
	SAM() { }
	void add_symbol_to_sam_for_built(char c);
	void create_sam(const string &T) {
		start = new State;
		last = start;

		for (char c : T) {
			add_symbol_to_sam_for_built(c);
		}
		auto temp = last;
		while (temp != start) {
			temp->set_accept(true);
			temp = temp->link();
		}
	}
	void insert_new_symbol(char const ch) { 
		auto temp = last;
		while (temp != start) {
			temp->set_accept(false);
			temp = temp->link();
		}
		add_symbol_to_sam_for_built(ch);
		temp = last;
		while (temp != start) {
			temp->set_accept(true);
			temp = temp->link();
		}
	}
	void clear();
	void verify_suffix(const string&S);
	void verify_substring(const string&S);
	int get_unique_substring_num();
	void path_count_help(State* begin);
	State* get_last() { return last; }
	//int get_occured_times(const string&s);
	int get_first_position(const string&S);
private:
	State *start = nullptr;
	State *last = nullptr;
};
void SAM::path_count_help(State* begin) {
	if (!begin)
	{
		cout << "Empty start.\n";
		return;
	}
	State*cur = begin;
	vector<State*> vec;//待访问的序列，组织为堆栈

	vec.push_back(cur);

	while (vec.size()) {
		cur = vec[vec.size() - 1];
		vec.pop_back();
		for (auto it : (cur->trans())) {
			vec.push_back(it.second);
			(it.second)->set_path_num((it.second)->path_num() + cur->path_num());
		}
	}
	
}

void SAM::add_symbol_to_sam_for_built(char c) {
	if (!start)
	{
		start = new State;
		last = start;
	}
	auto cur = new State;
	cur->set_maxlen(last->maxlen() + 1);
	cur->set_first_endpos(last->first_endpos() + 1);

	auto p = last;
	while (p && !p->has_trans(c)) {
		p->set_trans(c, cur);
		p = p->link();
	}
	last = cur;
	if (p == nullptr) {
		cur->set_link(start);
		cur->set_minlen(1);
		return;
	}

	auto q = p->trans(c);
	if (p->maxlen() + 1 == q->maxlen()) {
		cur->set_link(q);
		cur->set_minlen(q->maxlen() + 1);
	}
	else {
		auto sq = new State;
		sq->set_maxlen(p->maxlen() + 1);
		sq->set_trans(q->trans());
		sq->set_first_endpos(q->first_endpos());

		while (p && p->has_trans(c) && p->trans(c) == q) {
			p->set_trans(c, sq);
			p = p->link();
		}

		sq->set_link(q->link());
		sq->set_minlen(sq->link()->maxlen() + 1);

		q->set_link(sq);
		q->set_minlen(sq->maxlen() + 1);

		cur->set_link(sq);
		cur->set_minlen(sq->maxlen() + 1);
	}
	return;
}

void SAM::verify_suffix(const string&S)
{
	if (!start)
	{
		cout << "Empty SAM.\n";
		return;
	}
	auto cur = start;
	for (size_t i = 0; i < S.size(); i++)
	{
		if (cur->has_trans(S[i]))
			cur = cur->trans(S[i]);
		else
		{
			cout << "Not a suffix!\n";
			return;
		}
	}
	if (cur->accept())
		cout << "It is a suffix!\n";
	else cout << "Not a suffix!\n";
}

void SAM::verify_substring(const string&S)
{
	if (!start)
	{
		cout << "Empty SAM.\n";
		return;
	}
	auto cur = start;
	for (size_t i = 0; i < S.size(); i++)
	{
		if (cur->has_trans(S[i]))
			cur = cur->trans(S[i]);
		else
		{
			cout << "Not a substring!\n";
			return; 
		}
	}
	cout << "It is a substring!\n";
}
int SAM::get_unique_substring_num() {
	if (!start)
	{
		cout << "Empty SAM.\n";
		return -1;
	}
	set<State*> nodes;//收集图中所有节点的直接地址
	State*cur = start;
	vector<State*> vec;//待访问的序列，组织为堆栈

	vec.push_back(cur);

	
	while (vec.size()) {
		cur = vec[vec.size() - 1];
		cur->set_visited(true);
		nodes.insert(cur);
		vec.pop_back();
		for (auto it : (cur->trans()))
			if (!((it.second)->visited()))
				vec.push_back(it.second);
	}
	for (auto it : nodes)
		it->set_visited(false);
	map<State*, State*> map_from_asm_copy;
	for (auto it : nodes) {
		State* copy = new State;
		map_from_asm_copy[it] = copy;
	}//建立映射关系
	map<State*,char> from_start;
	for (auto it : nodes) {
		for (auto i : it->trans()) {
			if (!(from_start.count(map_from_asm_copy[i.second])))
				from_start[map_from_asm_copy[i.second]] = 0;
			(map_from_asm_copy[i.second])->set_trans(from_start[map_from_asm_copy[i.second]]++, map_from_asm_copy[it]);
		}
	}
	path_count_help(map_from_asm_copy[last]);
	return (map_from_asm_copy[start])->path_num()-1;
}
void SAM::clear() {
	if (!start)
	{
		cout << "Empty SAM.\n";
		return ;
	}
	set<State*> nodes;//收集图中所有节点的直接地址
	State*cur = start;
	vector<State*> vec;//待访问的序列，组织为堆栈

	vec.push_back(cur);


	while (vec.size()) {
		cur = vec[vec.size() - 1];
		cur->set_visited(true);
		nodes.insert(cur);
		vec.pop_back();
		for (auto it : (cur->trans()))
			if (!((it.second)->visited()))
				vec.push_back(it.second);
	}
	for (auto it:nodes)
		delete it;
	nodes.clear();
	start = last = nullptr;
}
int SAM::get_first_position(const string&S) {
	if (!start)
	{
		cout << "Empty SAM.\n";
		return -1;
	}
	auto cur = start;
	for (size_t i = 0; i < S.size(); i++)
	{
		if (cur->has_trans(S[i]))
			cur = cur->trans(S[i]);
		else
		{
			cout << "Not a substring!\n";
			return -1;
		}
	}
	
	return cur->first_endpos();
}
//int SAM::get_occured_times(const string&S) {
//	if (!start)
//	{
//		cout << "Empty SAM.\n";
//		return -1;
//	}
//	auto cur = start;
//	for (size_t i = 0; i < S.size(); i++)
//	{
//		if (cur->has_trans(S[i]))
//			cur = cur->trans(S[i]);
//		else
//		{
//			cout << "Not a substring.\n";
//			return -1;
//		}
//	}
//	State*source = cur;
//	set<State*> nodes;//收集图中所有节点的直接地址
//	cur = start;
//	vector<State*> vec;//待访问的序列，组织为堆栈
//
//	vec.push_back(cur);
//
//
//	while (vec.size()) {
//		cur = vec[vec.size() - 1];
//		cur->set_visited(true);
//		nodes.insert(cur);
//		vec.pop_back();
//		for (auto it : (cur->trans()))
//			if (!((it.second)->visited()))
//				vec.push_back(it.second);
//	}
//	for (auto it : nodes)
//		it->set_visited(false);
//	map<State*, int> link_num;
//	for (auto it : nodes) {
//		link_num[it->link()]=0;
//	}
//	for (auto it : nodes) {
//		if (it->link())
//			link_num[it->link()] += 1;
//	}
//	for (auto i : nodes) {
//		if (link_num[i]==0)
//		{
//			cout << "dsa";
//			State*te = i;
//			while (te) {
//				if (te->link())
//					(te->link())->set_path_num((te->link())->path_num() + te->path_num());
//				te = te->link();
//			}
//		}
//	}
//	int num = 0;
//	if (link_num[source]<=1)
//		num= source->path_num();
//	else num= source->path_num()-1;
//	for (auto i : nodes)
//		i->set_path_num(1);
//	return num;
//}
