#include "bag.h"

Bag::Bag(const Bag& other) {
	copy(other.data, other.data + other.s, data);
	s += other.s;
}

void Bag::insert(const bag_type& entry) {
	assert(!isFull());
	data[s] = entry;
	s += 1;
}

size_t Bag::count(const bag_type& target) const{
	size_t result = 0;
	for (size_t i = 0; i < s; i += 1) {
		if (data[i] == target) result += 1;
	}
	return result;
}

bool Bag::erase_one(const bag_type& target) {
	assert(!isEmpty());

	size_t delindex = 0;
	for (; delindex < s; delindex += 1) {
		if (data[delindex] == target) break;
	}

	if (delindex == s) return false;

	s -= 1;
	data[delindex] = data[s];
	return true;
}

size_t Bag::erase(const bag_type& target) {
	assert(!isEmpty());
	size_t new_s = 0;
	for (size_t i = 0; i < s; i += 1) {
		if (data[i] == target) continue;
		data[new_s] = data[i];
		new_s += 1;
	}
	size_t result = s - new_s;
	s = new_s;
	return result;
}

void Bag::print() const {
	for (size_t i = 0; i < s; i += 1) {
		cout << data[i] << ' ';
	}
	cout << '\n';
}

void Bag::operator+=(const Bag& b) {
	assert(s + b.s <= CAPACITY);
	copy(b.data, b.data + b.s, data + s);
	s += b.s;
}

Bag operator+(const Bag& b1, const Bag& b2) {
	size_t s = b1.length() + b2.length();
	assert(s <= Bag::CAPACITY);
	Bag result(b1);
	result += b2;
	return result;
}
