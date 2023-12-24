#pragma once
#include "Exceptions.h"
#include "Iterator.h"
#include <functional>
#include <iostream>
#include <iomanip>


template<typename _KeyType, typename _DataType>
class Node {
	using PairType = std::pair<const _KeyType, _DataType>;
public:
	Node(PairType p_pair, int p_bucket_number, Node* p_prev = nullptr, Node* p_next = nullptr) : m_pair(p_pair), m_bucket_number(p_bucket_number), m_prev(p_prev), m_next(p_next) {}
	PairType m_pair;
	Node* m_next;
	Node* m_prev;
	int m_bucket_number;
};


/*
Класс двусвязного списка, хранящего пары объектов типов _KeyType и _DataType
*/
template <typename _KeyType, typename _DataType, typename _Hash>
class Chain {
	using PairType = std::pair<const _KeyType, _DataType>;
	using NodeType = Node<_KeyType, _DataType>;
	using iterator = UnorderedMapIterator<_KeyType, _DataType>;
private:
	NodeType* m_head;
	int m_size;
public:
	Chain() : m_head(nullptr), m_size(0) {}

	Chain(Chain&& p_other): m_head(p_other.m_head), m_size(p_other.m_size) {
		p_other.m_size = 0;
		p_other.m_head = nullptr;
	}

	~Chain() {
		Clear();
	}

	Chain& operator=(Chain&& p_other) {
		if (this == &p_other) {
			return *this;
		}
		Clear();
		m_head = p_other.m_head;
		m_size = p_other.m_size;
		p_other.m_head = nullptr;
		p_other.m_size = 0;
		return *this;
	}

	iterator Insert(const PairType& p_pair, NodeType*& p_node_ptr, int p_bucket_number) {
		if (p_node_ptr) {
			NodeType* cur_ptr = p_node_ptr;
			NodeType* prev_ptr = nullptr;
			while (cur_ptr && cur_ptr->m_bucket_number == p_bucket_number) {
				if (cur_ptr->m_pair.first == p_pair.first) {
					cur_ptr->m_pair.second = p_pair.second;
					return iterator(cur_ptr);
				}
				prev_ptr = cur_ptr;
				cur_ptr = cur_ptr->m_next;
			}
			prev_ptr->m_next = new NodeType(p_pair, p_bucket_number, prev_ptr, cur_ptr);
			if (cur_ptr) {
				cur_ptr->m_prev = prev_ptr->m_next;
			}
			m_size++;
			return iterator(prev_ptr->m_next);
		}
		m_head = new NodeType(p_pair, p_bucket_number, nullptr, m_head);
		if (m_head->m_next) {
			m_head->m_next->m_prev = m_head;
		}
		p_node_ptr = m_head;
		m_size++;
		return iterator(m_head);
	}

	const _DataType& At(NodeType* p_node_ptr, const _KeyType& p_key, int p_bucket_number) const {
		if (p_node_ptr) {
			NodeType* cur_ptr = p_node_ptr;
			while (cur_ptr && cur_ptr->m_bucket_number == p_bucket_number) {
				if (cur_ptr->m_pair.first == p_key) {
					return cur_ptr->m_pair.second;
				}
				cur_ptr = cur_ptr->m_next;
			}
		}
		throw ItemNotFoundError("ItemNotFoundError: element with such key is not present in the container.");
	}

	_DataType& GetReferenceToElement(NodeType* p_node_ptr,const _KeyType& p_key, int p_bucket_number) {
		if (p_node_ptr) {
			NodeType* cur_ptr = p_node_ptr;
			NodeType* prev_ptr = nullptr;
			while (cur_ptr && cur_ptr->m_bucket_number == p_bucket_number) {
				if (cur_ptr->m_pair.first == p_key) {
					return cur_ptr->m_pair.second;
				}
				prev_ptr = cur_ptr;
				cur_ptr = cur_ptr->m_next;
			}
			prev_ptr->m_next = new NodeType(PairType(p_key, _DataType()), p_bucket_number, prev_ptr, cur_ptr);
			if (cur_ptr) {
				cur_ptr->m_prev = prev_ptr->m_next;
			}
			m_size++;
			return prev_ptr->m_next->m_pair.second;
		}

		m_head = new NodeType(PairType(p_key, _DataType()), p_bucket_number, nullptr, m_head);
		if (m_head->m_next) {
			m_head->m_next->m_prev = m_head;
		}
		p_node_ptr = m_head;
		m_size++;
		return m_head->m_pair.second;

	}

	void Erase(NodeType*& p_node_ptr,const _KeyType& p_key, int p_bucket_number) {
		if (p_node_ptr) {
			NodeType* cur_ptr = p_node_ptr;
			while (cur_ptr && cur_ptr->m_bucket_number == p_bucket_number) {
				if (cur_ptr->m_pair.first == p_key) {
					NodeType* prev_ptr = cur_ptr->m_prev;
					NodeType* next_ptr = cur_ptr->m_next;
					if (cur_ptr == p_node_ptr) {
						if (next_ptr == nullptr || next_ptr->m_bucket_number != p_bucket_number) {
							p_node_ptr = nullptr;
						}
						else {
							p_node_ptr = next_ptr;
						}
					}
					if (cur_ptr == m_head) {
						m_head = m_head->m_next;
					}
					if (prev_ptr) {
						prev_ptr->m_next = next_ptr;
					}
					if (next_ptr) {
						next_ptr->m_prev = prev_ptr;
					}
					delete cur_ptr;
					m_size--;
					return;
				}
				cur_ptr = cur_ptr->m_next;
			}
		}
		throw ItemNotFoundError("ItemNotFoundError: element with such key is not present in the container.");
	}

	iterator Erase(const iterator& p_iter, NodeType*& p_node_ptr, int p_bucket_number) {
		if (p_iter == nullptr) {
			throw IteratorError("IteratorError: iterator does not point to an element.");
		}
		NodeType* node_to_remove = p_iter.GetPtr();
		NodeType* prev_ptr = node_to_remove->m_prev;
		NodeType* next_ptr = node_to_remove->m_next;
		if (node_to_remove == p_node_ptr) {
			if (next_ptr == nullptr || next_ptr->m_bucket_number != p_bucket_number) {
				p_node_ptr = nullptr;
			}
			else {
				p_node_ptr = next_ptr;
			}
		}
		if (node_to_remove == m_head) {
			m_head = m_head->m_next;
		}
		if (prev_ptr) {
			prev_ptr->m_next = next_ptr;
		}
		if (next_ptr) {
			next_ptr->m_prev = prev_ptr;
		}
		delete node_to_remove;
		m_size--;
		return iterator(next_ptr);
	}

	void Clear() {
		NodeType* cur_ptr = m_head;
		NodeType* next_ptr;
		while (cur_ptr) {
			next_ptr = cur_ptr->m_next;
			delete cur_ptr;
			cur_ptr = next_ptr;
		}
		m_head = nullptr;
		m_size = 0;
	}

	int Size() const {
		return m_size;
	}

	NodeType* GetHead() {
		return m_head;
	}
};

