#pragma once

#include "base.h"
#include "linklist_iter.h"


template <class data_type>
class Stack_static_array {
public:
	static const size_t MAX_SIZE = 50;

	Stack_static_array() : length{0} {}

	void push(const data_type& value) {
		if (length == MAX_SIZE)	throw std::out_of_range("Stack is full");
		data[length++] = value;
	}

	data_type pop() {
		if (length == 0) throw std::out_of_range("Stack is empty");
		data_type result = data[--length];
		return result;
	}

	const data_type& top() const { 
		if (length == 0) throw std::out_of_range("Stack is empty"); 
		return data[length - 1];
	}
	bool empty() const { return length == 0; }
	size_t size() const { return length; }

private:
	data_type data[MAX_SIZE];
	size_t length;
};

template <class data_type>
class Stack_linklist {
public:
	Stack_linklist() : length{ 0 } {}

	void push(const data_type& value);
	data_type pop();
	const data_type& top() const;

	bool empty() const { return length == 0; }
	size_t size() const { return length; }

private:
	Linkedlist<data_type> data;
	size_t length;
};

template <class data_type>
void Stack_linklist<data_type>::push(const data_type& value) {
	data.push_front(value);
	++length;
}

template <class data_type>
data_type Stack_linklist<data_type>::pop() {
	if (length == 0) throw std::out_of_range("Stack is empty");
	--length;
	return data.pop_front();
}

template <class data_type>
const data_type& Stack_linklist<data_type>::top() const {
	if (length == 0) throw std::out_of_range("Stack is empty");
	return data.front();
}

template <class data_type>
class Stack_vector {
public:
	Stack_vector() : length{ 0 } {}

	void push(const data_type& value) {
		data.push_back(value);
		length += 1;
	}

	data_type pop() {
		if (length == 0) throw std::out_of_range("Stack is empty");
		data_type result = data[length - 1];
		data.pop_back();
		length -= 1;
		return result;
	}

	const data_type& top() const {
		if (length == 0) throw std::out_of_range("Stack is empty");
		return data[length - 1];
	}
	bool empty() const { return length == 0; }
	size_t size() const { return length; }

private:
	vector<data_type> data;
	size_t length;
};