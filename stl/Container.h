#pragma once
#include "List.h"
#include "Exceptions.h"
#include "Iterator.h"
#include <functional>
#include <initializer_list>


template<typename _KeyType, typename _DataType, typename _Hash = std::hash<_KeyType>>
class UnorderedMap {
private:
	using PairType = std::pair<const _KeyType, _DataType>;
	using ChainType = Chain<_KeyType, _DataType, _Hash>;
	using NodeType = Node<_KeyType, _DataType>;
public:
	using iterator = UnorderedMapIterator<_KeyType, _DataType>;

private:
	int m_buckets;
	double m_load_factor;
	double m_max_load_factor;
	_Hash m_hash_obj;
	NodeType** m_table;    // массив (размера m_buckets) из указателей на начало цепочек (все цепочки лежат в одном списке класса ChainType)
	ChainType m_chains;    // список, в котором лежат цепочки
public:
	UnorderedMap() : m_buckets(8), m_load_factor(0.), m_max_load_factor(1.) {
		m_table = new NodeType * [m_buckets] {};
	}

	explicit UnorderedMap(int p_buckets) : m_load_factor(0.), m_max_load_factor(1.) {
		if (p_buckets < 8) {
			m_buckets = 8;
		}
		else {
			m_buckets = p_buckets;
		}
		m_table = new NodeType * [m_buckets] {};
	}

	template<typename _Iter = iterator>
	UnorderedMap(const _Iter& p_left, const _Iter& p_right) : UnorderedMap() {
		for (auto cur = p_left; cur != p_right; cur++) {
			Insert(*cur);
		}
	}

	UnorderedMap(const UnorderedMap& p_other) : m_buckets(p_other.m_buckets), m_load_factor(p_other.m_load_factor),
		m_max_load_factor(p_other.m_max_load_factor), m_hash_obj(p_other.m_hash_obj) {
		m_table = new NodeType * [m_buckets] {};
		const NodeType* cur_ptr = p_other.m_chains.GetHead();
		while (cur_ptr) {
			Insert(cur_ptr->m_pair);
			cur_ptr = cur_ptr->m_next;
		}
	}

	UnorderedMap(const std::initializer_list<PairType>& p_list) : UnorderedMap(p_list.size()) {
		for (auto& elem : p_list) {
			Insert(elem);
		}
	};

	UnorderedMap(UnorderedMap&& p_other) : m_buckets(p_other.m_buckets), m_load_factor(p_other.m_load_factor),
		m_max_load_factor(p_other.m_max_load_factor), m_hash_obj(p_other.m_hash_obj), m_table(p_other.m_table) {
		m_chains = move(p_other.m_chains);
		p_other.m_buckets = 8;                                   // “.к. p_other у нас может быть результатом функции move,
		p_other.m_max_load_factor = 1;                           // мы не можем проигнорировать заполнение этих полей,
		p_other.m_load_factor = 0;                               // ведь в таком случае объект, к которому была применена функци€ move
		p_other.m_table = new NodeType * [p_other.m_buckets]{};  // может перестать быть валидным.
	}

	~UnorderedMap() {
		m_chains.Clear();
		delete[] m_table;
	}

	iterator begin() {
		return iterator(m_chains.GetHead());
	}

	iterator end() {
		return iterator();
	}

	UnorderedMap& operator=(const UnorderedMap& p_other) {
		if (this == &p_other) {
			return *this;
		}
		m_chains.Clear();
		delete[] m_table;
		m_table = new NodeType * [p_other.m_buckets]{};
		const NodeType* cur_ptr = p_other.m_chains.GetHead();
		while (cur_ptr) {
			Insert(cur_ptr->m_pair);
			cur_ptr = cur_ptr->m_next;
		}
		return *this;
	}

	UnorderedMap& operator=(UnorderedMap&& p_other) {
		if (this = &p_other) {
			return *this;
		}
		m_chains.Clear();
		delete[] m_table;
		m_chains = move(p_other.m_chains);
		m_buckets = p_other.m_buckets;
		m_load_factor = p_other.m_load_factor;
		m_max_load_factor = p_other.m_max_load_factor;
		m_hash_obj = p_other.m_hash_obj;
		m_table = p_other.m_table;
		p_other.m_buckets = 8;
		p_other.m_max_load_factor = 1;
		p_other.m_table = new NodeType * [p_other.m_buckets]{};
		p_other.m_load_factor = 0;
		return *this;
	}


	const iterator Insert(const PairType& p_pair) {
		int bucket_number = m_hash_obj(p_pair.first) % m_buckets;
		int current_elements = m_chains.Size();
		if ((current_elements + 1) / static_cast<double>(m_buckets) >= m_max_load_factor) {
			Reserve(current_elements + 1);
		}
		return m_chains.Insert(p_pair, m_table[bucket_number], bucket_number);
	}

	const iterator Insert(PairType&& p_pair) {
		int bucket_number = m_hash_obj(p_pair.first) % m_buckets;
		int current_elements = m_chains.Size();
		if ((current_elements + 1) / static_cast<double>(m_buckets) >= m_max_load_factor) {
			Reserve(current_elements + 1);
		}
		return m_chains.Insert(p_pair, m_table[bucket_number], bucket_number);
	}

	const _DataType& At(const _KeyType& p_key) const {
		int bucket_number = m_hash_obj(p_key) % m_buckets;
		return m_chains.At(m_table[bucket_number], p_key, bucket_number);
	}

	/*
	 онстантной версии нет, т.к. operator[] при вызове по несуществующему ключу создает в контейнере
	пару со значением second по умолчанию. -> т.е. operator[] в любом случае способен измен€ть контейнер.
	*/
	_DataType& operator[](const _KeyType& p_key) {
		int bucket_number = m_hash_obj(p_key) % m_buckets;
		int current_elements = m_chains.Size();
		if ((current_elements + 1) / static_cast<double>(m_buckets) >= m_max_load_factor) {
			Reserve(current_elements + 1);
		}
		return m_chains.GetReferenceToElement(m_table[bucket_number], p_key, bucket_number);
	}

	void Erase(const _KeyType& p_key) {
		int bucket_number = m_hash_obj(p_key) % m_buckets;
		m_chains.Erase(m_table[bucket_number], p_key, bucket_number);
	}

	iterator Erase(const iterator& p_iter) {
		int bucket_number = p_iter.GetBucketNumber();
		return m_chains.Erase(p_iter, m_table[bucket_number], bucket_number);
	}

	double MaxLoadFactor() const {
		return m_max_load_factor;
	}

	void MaxLoadFactor(double p_max_load_factor) {
		if (p_max_load_factor <= 0.0) {
			throw InvalidValueError("InvalidValueError: invalid hash load factor.");
		}
		m_max_load_factor = p_max_load_factor;
	}

	void Reserve(int p_num) {
		if (p_num <= 0) {
			throw InvalidValueError("InvalidValueError: invalid hash bucket count.");
		}
		delete[] m_table;
		m_buckets = p_num * static_cast<int>(2 / m_max_load_factor);
		m_table = new NodeType * [m_buckets] {};
		ChainType temp_chains = std::move(m_chains);
		NodeType* cur_ptr = temp_chains.GetHead();
		while (cur_ptr) {
			Insert(cur_ptr->m_pair);
			cur_ptr = cur_ptr->m_next;
		}
	}

	bool Empty() const {
		return !m_chains.Size();
	}

	int Size() const {
		return m_chains.Size();
	}

	void Clear() {
		m_chains.Clear();
		m_load_factor = 0;
		for (int i = 0; i < m_buckets; i++) {
			m_table[i] = nullptr;
		}
	}

	int GetBucketCount() const {
		return m_buckets;
	}

	double GetLoadFactor() const {
		return Size() / static_cast<double>(m_buckets);
	}
};
