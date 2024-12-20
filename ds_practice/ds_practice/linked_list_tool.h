#pragma once

#include "node.h"

size_t length(const BasicNode* head_ptr);

void list_head_insert(BasicNode*& head_ptr,
					  const BasicNode::type& entry);

void list_insert(BasicNode* previous_ptr,
				 const BasicNode::type& entry);

BasicNode* list_search(BasicNode* head_ptr,
					   const BasicNode::type& target);

const BasicNode* list_search(const BasicNode* head_ptr,
							 const BasicNode::type& target);

BasicNode* list_locate(BasicNode* head_ptr, size_t position);

const BasicNode* list_locate(const BasicNode* head_ptr, size_t position);

void list_copy(const BasicNode* source_ptr,
			   BasicNode*& head_ptr,
			   BasicNode*& tail_ptr);

void list_head_remove(BasicNode*& head_ptr);

void list_remove(BasicNode* previous_ptr);

void list_clear(BasicNode*& head_ptr);

void list_piece(const BasicNode* start_ptr, const BasicNode* end_ptr,
				BasicNode*& head_ptr, BasicNode*& tail_ptr);
