#include "linked_list_tool.h"

size_t length(const BasicNode* head_ptr) {
	size_t answer = 0;
	for (; head_ptr != nullptr; head_ptr = head_ptr->link()) {
		answer += 1;
	}
	return answer;
}

void list_head_insert(BasicNode*& head_ptr,
					  const BasicNode::type& entry)
{
	head_ptr = new BasicNode{ entry, head_ptr };
}

void list_insert(BasicNode* previous_ptr,
				 const BasicNode::type& entry)
{
	BasicNode* now = new BasicNode{ entry, previous_ptr->link()};
	previous_ptr->set_link(now);
}

BasicNode* list_search(BasicNode* head_ptr,
					   const BasicNode::type& target)
{
	for (; head_ptr != nullptr; head_ptr = head_ptr->link()) {
		if (head_ptr->data() == target) return head_ptr;
	}
	return nullptr;
}

const BasicNode* list_search(const BasicNode* head_ptr,
							 const BasicNode::type& target)
{
	for (; head_ptr != nullptr; head_ptr = head_ptr->link()) {
		if (head_ptr->data() == target) return head_ptr;
	}
	return nullptr;
}

BasicNode* list_locate(BasicNode* head_ptr, size_t position) {
	for (size_t i = 0; (i < position) && (head_ptr != nullptr); i += 1) {
		head_ptr = head_ptr->link();
	}
	return head_ptr;
}

const BasicNode* list_locate(const BasicNode* head_ptr, size_t position) {
	for (size_t i = 0; (i < position) && (head_ptr != nullptr); i += 1) {
		head_ptr = head_ptr->link();
	}
	return head_ptr;
}

void list_copy(const BasicNode* source_ptr,
			   BasicNode*& head_ptr,
			   BasicNode*& tail_ptr)
{
	if (source_ptr == nullptr) return;

	list_head_insert(head_ptr, source_ptr->data());
	tail_ptr = head_ptr;

	source_ptr = source_ptr->link();
	for (; source_ptr != nullptr; tail_ptr = tail_ptr->link(), source_ptr = source_ptr->link()) {
		list_insert(tail_ptr, source_ptr->data());
	}
}

void list_head_remove(BasicNode*& head_ptr) {
	if (head_ptr == nullptr) return;
	BasicNode* temp = head_ptr->link();
	delete head_ptr;
	head_ptr = temp;
}

void list_remove(BasicNode* previous_ptr) {
	if (previous_ptr->link() == nullptr) return;
	BasicNode* remove = previous_ptr->link();
	previous_ptr->set_link(remove->link());
	delete remove;
}

void list_clear(BasicNode*& head_ptr) {
	while (head_ptr != nullptr) list_head_remove(head_ptr);
}

void list_piece(const BasicNode* start_ptr, const BasicNode* end_ptr,
				BasicNode*& head_ptr, BasicNode*& tail_ptr) {
	head_ptr = new BasicNode(start_ptr->data());
	tail_ptr = head_ptr;
	start_ptr = start_ptr->link();
	for (; start_ptr != end_ptr; start_ptr = start_ptr->link()) {
		BasicNode* temp = new BasicNode(start_ptr->data());
		tail_ptr->set_link(temp);
		tail_ptr = temp;
	}
}