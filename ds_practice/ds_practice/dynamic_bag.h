#pragma once

#include "base.h"

class DynamicBag {
public:
	typedef int bag_type;

	DynamicBag(size_t length = INITIAL_CAPACITY);
	DynamicBag(const DynamicBag& other);
	DynamicBag& operator=(const DynamicBag& other);

	~DynamicBag() { delete[] data; }

	void insert(const bag_type& entry);

	size_t count(const bag_type& target) const;

	bool erase_one(const bag_type& target);
	size_t erase(const bag_type& target);

	size_t size() const { return length; }
	bool isEmpty() const { return length == 0; }

	void print() const;

	void operator+=(const DynamicBag& other);

	static const size_t INITIAL_CAPACITY = 8;
private:
	bag_type* data;
	size_t length;
	size_t capacity;

	void make_capacity(size_t new_size);
};
DynamicBag& operator+(const DynamicBag& b1, const DynamicBag& b2);