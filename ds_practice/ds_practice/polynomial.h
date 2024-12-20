#pragma once

#include "base.h"

class Polynomial {
public:
	typedef double type;
	static const size_t INITIAL_length{ 5 };

	Polynomial(const size_t& length = INITIAL_length);
	~Polynomial() { delete[] data; }

	void set_term(const type& c, const size_t& d);

	type operator()(const type& x);
	friend ostream& operator<<(ostream& os, const Polynomial& t);

private:
	type* data;
	size_t length;

	void resize(const size_t& new_length);
	size_t highest_order();
};

inline bool isZero(double num, double epsilon = 1e-10) {
	return fabs(num) < epsilon;
}

