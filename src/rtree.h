#pragma once
#ifndef RTREE_H
#define RTREE_H

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <functional>



#define RTREE_TEMPLATE template<class KEYTYPE, class ELEMTYPE>
#define RTREE_QUAL RTree<KEYTYPE, ELEMTYPE>

template<class KEYTYPE, class ELEMTYPE>
class RTree {
public:
	class Node {
		KEYTYPE m_key;
		ELEMTYPE m_value;
	public:
		Node *m_left = NULL;
		Node *m_right = NULL;
	public:
		Node(KEYTYPE key, ELEMTYPE value) {
			this->m_key = key;
			this->m_value = value;
		}

		/* Setters */
		void set_key(KEYTYPE key) { this->m_key = key; }
		void set_value(ELEMTYPE  value) { this->m_value = value; }
		/* Getters */
		KEYTYPE get_key() { return this->m_key; }
		ELEMTYPE get_value() { return this->m_value; }

		void insert (Node* node, KEYTYPE key, ELEMTYPE value);
		Node* search(Node *node, KEYTYPE key) {
			if (node == NULL) {
				return NULL;
			}
			if (node->m_key == key)
				return node;

			return (key < node->m_key) ?
					search(node->m_left, key) : search(node->m_right, key);
		}
	};

public:
	Node* m_head = NULL;
public:
	RTree();
	RTree(const RTree& other);
//	virtual ~RTree();


	void Insert (KEYTYPE key, ELEMTYPE value);
	ELEMTYPE Search(KEYTYPE key);
};

RTREE_TEMPLATE
RTREE_QUAL::RTree(){
	this->m_head = NULL;
}

RTREE_TEMPLATE
void RTREE_QUAL::Insert(KEYTYPE key, ELEMTYPE value) {
	if (this->m_head == NULL) {
		this->m_head = new Node(key, value);
	} else {
		this->m_head->insert(this->m_head, key, value);
	}
}

RTREE_TEMPLATE
ELEMTYPE RTREE_QUAL::Search(KEYTYPE key) {
	Node* node = this->m_head->search(this->m_head, key);
	return node->get_value();
}

RTREE_TEMPLATE
void RTREE_QUAL::Node::insert(Node* node, KEYTYPE key, ELEMTYPE value){
	if (key < node->get_key()) {
		if (node->m_left == NULL) {
			node->m_left = new Node(key, value);
		} else {
			insert(node->m_left, key, value);
		}
	} else if (key >= node->get_key()) {
		if (node->m_right == NULL) {
			node->m_right = new Node(key, value);
		} else {
			insert(node->m_right, key, value);
		}
	}
}

#endif // !RTREE_H
