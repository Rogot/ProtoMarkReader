#include "rtree.h"

//template<class KEYTYPE, class ELEMTYPE>
//RTree<KEYTYPE, ELEMTYPE>::RTree(){
//	this->m_head = NULL;
//}
//
//template<class KEYTYPE, class ELEMTYPE>
//void RTree<KEYTYPE, ELEMTYPE>::Insert(KEYTYPE key, ELEMTYPE value) {
//	if (this->m_head == NULL) {
//		this->m_head = new Node(key, value);
//	} else {
//		this->m_head->insert(this->m_head, key, value);
//	}
//}
//
//template<class KEYTYPE, class ELEMTYPE>
//void RTree<KEYTYPE, ELEMTYPE>::Node::insert(Node* node, KEYTYPE key, ELEMTYPE value){
//	if (key < node->get_key()) {
//		if (node->m_left == NULL) {
//			node->m_left = new Node(key, value);
//		} else {
//			insert(node->m_left, key, value);
//		}
//	} else if (key >= node->get_key()) {
//		if (node->m_right == NULL) {
//			node->m_right = new Node(key, value);
//		} else {
//			insert(node->m_right, key, value);
//		}
//	}
//}
