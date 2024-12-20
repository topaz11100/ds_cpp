#pragma once

#include "base.h"
#include "linklist_iter.h"


template <class data_type>
class Queue_static_array {
public:
	static const size_t MAX_SIZE = 5;

	Queue_static_array() : length{ 0 }, head{ 0 }, tail{ 0 } {}

	void enqueue(const data_type& value) {
		if (length == MAX_SIZE)	throw std::out_of_range("Queue is full");
		data[tail] = value;
		increment(tail);
		length += 1;
	}

	data_type dequeue() {
		if (length == 0) throw std::out_of_range("Queue is empty");
		data_type result = data[head];
		increment(head);
		length -= 1;
		return result;
	}

	const data_type& peek() const { 
		if (length == 0) throw std::out_of_range("Queue is empty");
		return data[head];
	}
	bool empty() const { return length == 0; }
	size_t size() const { return length; }

	template <class data_type>
	friend ostream& operator << (ostream& os, Queue_static_array<data_type>& q);

private:
	data_type data[MAX_SIZE];
	size_t head;
	size_t tail;
	size_t length;

	void increment(size_t& index) {
		index = (index + 1) % MAX_SIZE;
	}
};

template <class data_type>
ostream& operator << (ostream& os, Queue_static_array<data_type>& q)
{
	if (q.length == 0) return os;
	size_t i = q.head;
	for (size_t j = 0; j < q.length; ++j) {
		os << q.data[i] << " ";
		q.increment(i);
	}
	return os;
}

template <class data_type>
class Queue_linklist {
public:
	Queue_linklist() : length{ 0 } {}

	void enqueue(const data_type& value);
	data_type dequeue();
	const data_type& peek() const;

	bool empty() const { return length == 0; }
	size_t size() const { return length; }

	template <class data_type>
	friend ostream& operator << (ostream& os, Queue_linklist<data_type>& q);

private:
	Linkedlist<data_type> data;
	size_t length;
};

template <class data_type>
void Queue_linklist<data_type>::enqueue(const data_type& value) {
	data.push_back(value);
	++length;
}

template <class data_type>
data_type Queue_linklist<data_type>::dequeue() {
	if (length == 0) throw std::out_of_range("Queue is empty");
	--length;
	return data.pop_front();
}

template <class data_type>
const data_type& Queue_linklist<data_type>::peek() const {
	if (length == 0) throw std::out_of_range("Queue is empty");
	return data.front();
}

template <class data_type>
ostream& operator << (ostream& os, Queue_linklist<data_type>& q)
{
	for (auto& x : q.data) {
		os << x << " ";
	}
	return os;
}

template <class data_type>
class Queue_vector {
public:
	Queue_vector() : length{ 0 } {}

	void enqueue(const data_type& value) {
		data.push_back(value);
		length += 1;
	}

	data_type dequeue() {
		if (length == 0) throw std::out_of_range("Queue is empty");
		data_type result = data[0];
		data.erase(data.begin());
		length -= 1;
		return result;
	}

	const data_type& peek() const {
		if (length == 0) throw std::out_of_range("Queue is empty");
		return data[0];
	}
	bool empty() const { return length == 0; }
	size_t size() const { return length; }

private:
	vector<data_type> data;
	size_t length;
};