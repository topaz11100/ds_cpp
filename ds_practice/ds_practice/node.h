#pragma once

#include "base.h"

class BasicNode {
public:
	typedef double type;

	BasicNode(const type& data = type(), BasicNode* link = nullptr);

	void set_data(const type& data);
	void set_link(BasicNode* link);

	type& data();
	const type& data() const;

	BasicNode* link();
	const BasicNode* link() const;

private:
	type data_field;
	BasicNode* link_field;
};

ostream& operator << (ostream& os, const BasicNode& node);

class Node {
public:
	typedef double type;

	Node(const type& data = type(),
		 Node* pre_link = nullptr,
		 Node* next_link = nullptr);

	void set_data(const type& data);
	void set_pre(Node* pre);
	void set_next(Node* next);

	type& data();
	const type& data() const;

	Node* pre();
	const Node* pre() const;

	Node* next();
	const Node* next() const;

private:
	type data_field;
	Node* pre_link;
	Node* next_link;
};

ostream& operator << (ostream& os, const Node& node);