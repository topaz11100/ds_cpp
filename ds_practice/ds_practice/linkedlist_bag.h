#pragma once

#include "base.h"
#include "node.h"
#include "linked_list_tool.h"



class LinkBag {
public:
	typedef BasicNode::type type;

	LinkBag();
	LinkBag(const LinkBag& source);
	~LinkBag();
	LinkBag& operator =(const LinkBag& source);
	void operator +=(const LinkBag& addend);

	void insert(const type& entry);

	bool erase_one(const type& target);
	size_t erase(const type& target);
	void clear();
	
	size_t size() const;
	size_t count(const type& target) const;
	type grab() const;

	friend ostream& operator << (ostream& os, const LinkBag& b);
private:
	BasicNode* head_ptr;
	size_t length;
};

LinkBag operator +(const LinkBag& b1, const LinkBag& b2);

ostream& operator << (ostream& os, const LinkBag& b);

