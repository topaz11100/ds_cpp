#pragma once

#include "base.h"

template<class data_type>
class Binode {
public:
	Binode(const data_type& data_field = data_type(),
		   Binode* back_node = nullptr,
		   Binode* next_node = nullptr);

	data_type& data() { return data_field; }
	const data_type& data() const { return data_field; }

	void set_link(Binode* back, Binode* next) { back_node = back; next_node = next; }

	Binode* back() { return back_node; }
	const Binode* back() const { return back_node; }

	Binode* next() { return next_node; }
	const Binode* next() const { return next_node; }
	
private:
	data_type data_field;
	Binode* back_node;
	Binode* next_node;
};

template<class data_type>
Binode<data_type>::Binode(const data_type& data_field, Binode<data_type>* back_node, Binode<data_type>* next_node) {
	this->data_field = data_field;
	this->back_node = back_node;
	this->next_node = next_node;
}

template<class data_type>
class Linkedlist {
public:
	Linkedlist() :head{ nullptr }, tail{ nullptr }, length{ 0 } {}
	~Linkedlist();
	
	data_type pop_back();
	data_type pop_front();
	void push_back(const data_type& data);
	void push_front(const data_type& data);
	void remove(const data_type& data);
	void clear();

	const data_type& front() const { return head->data(); }
	const data_type& back() const { return tail->data(); }

	class iterator;
	auto find(const data_type& d);
	iterator insert(iterator i, const data_type& d);
	iterator erase(iterator i);

	iterator begin() { return iterator{ head }; }
	iterator end() { return iterator{ nullptr }; }
	size_t size() const { return length; }

	template<class data_type>
	friend ostream& operator<<(ostream& os, Linkedlist<data_type>& l);

private:
	Binode<data_type>* head;
	Binode<data_type>* tail;
	size_t length;
};

template<class data_type>
ostream& operator<< (ostream& os, Linkedlist<data_type>& l) {
	if (l.size() == 0) return os;
	auto i = l.begin();
	auto j = typename Linkedlist<data_type>::iterator{ l.tail };
	for (; i != j ; ++i) os << *i << " ";
	os << *j;
	return os;
}

template<class data_type>
class Linkedlist<data_type>::iterator {
	friend class Linkedlist<data_type>;
public:
	iterator(Binode<data_type>* node = nullptr) :now{ node } {}
	iterator& operator++() { now = now->next(); return *this; }
	iterator& operator--() { now = now->back(); return *this; }
	data_type& operator*() { return now->data(); }
	const data_type& operator*() const { return now->data(); }
	bool operator==(const iterator& i) const { return now == i.now; }
	bool operator!=(const iterator& i) const { return now != i.now; }
private:
	Binode<data_type>* now;
};

template<class data_type>
auto Linkedlist<data_type>::find(const data_type& d) {
	for (iterator i = begin(); i != end(); ++i) {
		if (*i == d) return i;
	}
	return end();
}

template<class data_type>
typename Linkedlist<data_type>::iterator Linkedlist<data_type>::insert(iterator i, const data_type& d) {
	Binode<data_type>* temp;
	if (length == 0) {
		temp = new Binode<data_type>{ d, nullptr, nullptr };
		head = temp; tail = temp; length += 1;
		return iterator{ temp };
	}
	if (i == end()) {
		temp = new Binode<data_type>{ d, tail, nullptr };
		tail->set_link(tail->back(), temp);
		tail = temp;
	}
	else if (i == begin()) {
		temp = new Binode<data_type>{ d, nullptr, i.now };
		head = temp;
		i.now->set_link(temp, i.now->next());
	}
	else {
		temp = new Binode<data_type>{ d, i.now->back(), i.now };
		i.now->set_link(temp, i.now->next());
		--i; --i; i.now->set_link(i.now->back(), temp);
	}
	length += 1;
	return iterator{ temp };
}

template<class data_type>
typename Linkedlist<data_type>::iterator Linkedlist<data_type>::erase(iterator i) {
	if (length == 0 || i == end()) throw out_of_range{ "\nLinkList erase\n" };
	Binode<data_type>* del = i.now;
	if (length == 1) {
		delete del;
		head = nullptr; tail = nullptr; length = 0;
		return end();
	}
	else if (i.now == tail) {
		tail = (--i).now;
		tail->set_link(tail->back(), nullptr);
		delete del;
		i = end();
	}
	else if (i == begin()) {
		head = (++i).now;
		head->set_link(nullptr, head->next());
		delete del;
		i = begin();
	}
	else {
		--i; i.now->set_link(i.now->back(), i.now->next()->next());
		++i; i.now->set_link(i.now->back()->back(), i.now->next());
		delete del;
	}
	length -= 1;
	return i;
}

template<class data_type>
data_type Linkedlist<data_type>::pop_back() {
	data_type result = tail->data();
	erase(iterator{ tail });
	return result;
}

template<class data_type>
data_type Linkedlist<data_type>::pop_front() {
	data_type result = head->data();
	erase(iterator{ head });
	return result;
}

template<class data_type>
void Linkedlist<data_type>::push_back(const data_type& data) {
	insert(end(), data);
}

template<class data_type>
void Linkedlist<data_type>::push_front(const data_type& data) {
	insert(begin(), data);
}

template<class data_type>
void Linkedlist<data_type>::remove(const data_type& data) {
	iterator temp = find(data);
	if (temp == end()) return;
	erase(temp);
}

template<class data_type>
void Linkedlist<data_type>::clear() {
	while (length > 0) erase(begin());
}

template<class data_type>
Linkedlist<data_type>::~Linkedlist() {
	clear();
}
