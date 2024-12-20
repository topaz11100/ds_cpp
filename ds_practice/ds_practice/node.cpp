#include "node.h"

BasicNode::BasicNode(const type& data, BasicNode* link) {
	data_field = data;
	link_field = link;
}

void BasicNode::set_data(const type& data) {
	data_field = data;
}

void BasicNode::set_link(BasicNode* link) {
	link_field = link;
}

BasicNode::type& BasicNode::data() {
	return data_field;
}

const BasicNode::type& BasicNode::data() const{
	return data_field;
}

BasicNode* BasicNode::link() {
	return link_field;
}

const BasicNode* BasicNode::link() const{
	return link_field;
}

ostream& operator << (ostream& os, const BasicNode& node) {
	os << node.data();
	return os;
}

Node::Node(const type& data,
		   Node* pre_link,
		   Node* next_link)
{
	this->data_field = data;
	this->pre_link = pre_link;
	this->next_link = next_link;
}

void Node::set_data(const type& data) {
	data_field = data;
}

void Node::set_pre(Node* pre) {
	pre_link = pre;
}

void Node::set_next(Node* next) {
	next_link = next;
}

Node::type& Node::data() {
	return data_field;
}

const Node::type& Node::data() const {
	return data_field;
}

Node* Node::pre() {
	return pre_link;
}

const Node* Node::pre() const {
	return pre_link;
}

Node* Node::next() {
	return next_link;
}

const Node* Node::next() const {
	return next_link;
}

ostream& operator << (ostream& os, const Node& node) {
	os << node.data();
	return os;
}