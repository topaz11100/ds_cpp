#include "dynamic_bag.h"

DynamicBag::DynamicBag(size_t length) {
	data = new bag_type[length];
	this->length = 0;
	capacity = length;
}

DynamicBag::DynamicBag(const DynamicBag& other) {
	data = new bag_type[other.capacity];
	copy(other.data, other.data + other.length, data);
	length = other.length;
	capacity = other.capacity;
}

DynamicBag& DynamicBag::operator=(const DynamicBag& other) {
	if (this == &other) return *this;
	make_capacity(other.length);
	copy(other.data, other.data + other.length, data);
	length = other.length;
	return *this;
}

void DynamicBag::make_capacity(size_t new_capacity) {
	if (capacity >= new_capacity) return;
	bag_type* new_data = new bag_type[new_capacity];
	copy(data, data + length, new_data);
	delete[] data;
	data = new_data;
	capacity = new_capacity;
}

void DynamicBag::insert(const bag_type& entry) {
	if (length == capacity) make_capacity(2 * capacity);
	data[length] = entry;
	length += 1;
}

size_t DynamicBag::count(const bag_type& target) const {
	size_t result = 0;
	for (size_t i = 0; i < length; i += 1) {
		if (data[i] == target) result += 1;
	}
	return result;
}

bool DynamicBag::erase_one(const bag_type& target) {
	assert(!isEmpty());

	size_t delindex = 0;
	while (delindex < length && data[delindex] != target) delindex += 1;

	if (delindex == length) return false;

	length -= 1;
	data[delindex] = data[length];
	return true;
}

size_t DynamicBag::erase(const bag_type& target) {
	assert(!isEmpty());
	size_t new_length = 0;
	for (size_t i = 0; i < length; i += 1) {
		if (data[i] == target) continue;
		data[new_length] = data[i];
		new_length += 1;
	}
	size_t result = length - new_length;
	length = new_length;
	return result;
}

void DynamicBag::print() const {
	for (size_t i = 0; i < length; i += 1) {
		cout << data[i] << " ";
	}
	cout << endl;
}

void DynamicBag::operator+=(const DynamicBag& other) {
	make_capacity(capacity + other.length);
	copy(other.data, other.data + other.length, data + length);
	length += other.length;
	capacity += other.length;
}

DynamicBag& operator+(const DynamicBag& b1, const DynamicBag& b2) {
	DynamicBag a{ b1 };
	a += b2;
	return a;
}