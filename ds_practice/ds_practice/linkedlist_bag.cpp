#include "linkedlist_bag.h"

LinkBag::LinkBag() :head_ptr{ nullptr }, length{ 0 } {}

LinkBag::LinkBag(const LinkBag& source) {
	BasicNode* temp;
	list_copy(source.head_ptr, head_ptr, temp);
	length = source.length;
}

LinkBag::~LinkBag() {
	list_clear(head_ptr);
	length = 0;
}

LinkBag& LinkBag::operator =(const LinkBag& source) {
	if (this == &source) return *this;
	list_clear(head_ptr);
	length = 0;
	BasicNode* temp;
	list_copy(source.head_ptr, head_ptr, temp);
	length = source.length;
	return *this;
}

void LinkBag::operator +=(const LinkBag& addend) {
	if (addend.length == 0) return;
	BasicNode* temp_h, * temp_t;
	list_copy(addend.head_ptr, temp_h, temp_t);
	temp_t->set_link(head_ptr);
	head_ptr = temp_h;
	length += addend.length;
}

void LinkBag::insert(const type& entry) {
	list_head_insert(head_ptr, entry);
	length += 1;
}

bool LinkBag::erase_one(const type& target) {
	BasicNode* erase_ptr = list_search(head_ptr, target);
	if (erase_ptr == nullptr) return false;

	erase_ptr->set_data(head_ptr->data());
	list_head_remove(head_ptr);
	length -= 1;
	
	return true;
}

size_t LinkBag::erase(const type& target) {
	size_t result = length;
	while (erase_one(target)) {}
	return result - length;
}

void LinkBag::clear() {
	list_clear(head_ptr);
	length = 0;
}

size_t LinkBag::size() const { return length; }

size_t LinkBag::count(const type& target) const {
	size_t result = 0;
	for (BasicNode* i = head_ptr; i != nullptr; i = i->link()) {
		if (i->data() == target) result += 1;
	}
	return result;
}

LinkBag::type LinkBag::grab() const {
	assert(length > 0);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, length - 1);
	BasicNode* p = list_locate(head_ptr, dis(gen));
	return p->data();
}

LinkBag operator +(const LinkBag& b1, const LinkBag& b2) {
	LinkBag result{ b2 };
	result += b1;
	return result;
}

ostream& operator << (ostream& os, const LinkBag& b) {
	os << " ";
	for (BasicNode* i = b.head_ptr; i != nullptr; i = i->link()) {
		os << i->data() << " ";
	}
	return os;
}