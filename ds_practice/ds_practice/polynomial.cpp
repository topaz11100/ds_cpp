#include "polynomial.h"

Polynomial::Polynomial(const size_t& length) {
	data = new type[length + 1]();
	this->length = length + 1;
}

void Polynomial::set_term(const type& c, const size_t& d) {
	if (d >= length) resize(d);
	data[d] = c;
	size_t h = highest_order();
	if (h != length - 1) resize(h);
}

Polynomial::type Polynomial::operator()(const type& x) {
	type result = 0;
	for (size_t i = length; i > 0; i -= 1) {
		result += data[i] * powl(x, i);
	}
	return result + data[0];
}

void Polynomial::resize(const size_t& new_length) {
	type* new_data = new type[new_length + 1]();
	size_t end_length = new_length > length ? length : new_length + 1;
	copy(data, data + end_length, new_data);
	delete[] data;
	data = new_data;
	length = new_length + 1;
}

size_t Polynomial::highest_order() {
	for (size_t i = length - 1; i > 0; i -= 1) {
		if (!isZero(data[i])) return i;
	}
	return 0;
}

ostream& operator<<(ostream& os, const Polynomial& t) {
	for (int i = t.length; i >= 0; i -= 1) {
		if (isZero(t.data[i])) continue;
		char sign = t.data[i] < 0 ? '\0' : '+';
		os << ' ' << sign << t.data[i] << "x^" << i;
	}
	return os;
}