#pragma once

#include "base.h"

class Bag {
public:
	typedef int bag_type;
	Bag() {}
	Bag(const Bag& other);

	void insert(const bag_type& entry);

	bool erase_one(const bag_type& target);
	size_t erase(const bag_type& target);

	size_t count(const bag_type& target) const;

	size_t length() const { return s; }
	bool isFull() const { return s == CAPACITY; }
	bool isEmpty() const { return s == 0; }

	void print() const;

	void operator+=(const Bag& b);

	static const size_t CAPACITY{ 50 };
private:
	bag_type data[CAPACITY];
	size_t s{ 0 };

};
Bag operator+(const Bag& b1, const Bag& b2);
