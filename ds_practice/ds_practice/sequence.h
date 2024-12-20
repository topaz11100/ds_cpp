#pragma once

#include "base.h"

class Sequence {
public:
	typedef int value_type;
	static const size_t CAPACITY = 50;

	Sequence() {}

	void start();
	void advance();

	void insert(const value_type& entry);
	void attach(const value_type& entry);

	void remove_current();

	size_t size() const { return seq_size; }
	bool is_item() const { return 0 <= cur_index && cur_index < size(); }
	value_type current() const { return data[cur_index]; }

	void print() const;

private:
	size_t seq_size{ 0 };
	size_t cur_index{ 0 };

	value_type data[CAPACITY];

};
