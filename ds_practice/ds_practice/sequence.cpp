#include "sequence.h"

void Sequence::start() {
	cur_index = 0;
}

void Sequence::advance() {
	assert(cur_index + 1 < seq_size);
	cur_index += 1;
}

void Sequence::insert(const value_type& entry) {
	assert(seq_size < CAPACITY);
	for (size_t i = seq_size; i > cur_index; i -= 1) {
		data[i] = data[i - 1];
	}
	data[cur_index] = entry;
	seq_size += 1;
}

void Sequence::attach(const value_type& entry) {
	assert(seq_size < CAPACITY);
	if (cur_index != seq_size) cur_index += 1;
	insert(entry);
}

void Sequence::remove_current() {
	for (size_t i = cur_index; i < seq_size; i += 1) {
		data[i] = data[i + 1];
	}
	seq_size -= 1;
	if (cur_index > 0) cur_index -= 1;
}

void Sequence::print() const {
	for (size_t i = 0; i < seq_size; i += 1) {
		cout << data[i] << ' ';
	}
	cout << endl;
}
