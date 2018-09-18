//
//  SingleLinkedList.h
//  Data Structure - LinkedList
//
//  Created by Morgan Weiss on 8/7/2018
//  Copyright © 2018 Morgan Weiss. All rights reserved.
// 
// Note: You know pointer != nullptr is the same as pointer in a boolean context? 
// The same for pointer == nullptr and !pointer.
//

#ifndef DOUBLELINKEDLIST_h
#define DOUBLELINKEDLIST_h


template <class T>
class DoubleLinkedList {
private:

	struct Node {
		T data;
		std::unique_ptr<Node> next = nullptr;
		Node* previous = nullptr;


		template<typename... Args, typename = std::enable_if_t<std::is_constructible<T, Args&&...>::value>>
		explicit Node(std::unique_ptr<Node>&& next, Node* previous, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args&&...>::value)
			: data{ std::forward<Args>(args)... }, previous{previous}, next{ std::move(next) } {}

		// disable if noncopyable<T> for cleaner error msgs
		explicit Node(const T& x, std::unique_ptr<Node>&& p = nullptr)
			: data(x)
			, next(std::move(p)) {}

		// disable if nonmovable<T> for cleaner error msgs
		explicit Node(T&& x, std::unique_ptr<Node>&& p = nullptr)
			: data(std::move(x))
			, next(std::move(p)) {}
	};
	std::unique_ptr<Node> head = nullptr;
	Node* tail = nullptr;

	void do_pop_front() {
		head = std::move(head->next);
		if (!tail) tail = head.get(); // update tail if list was empty before
	}

public:
	// Constructors
	DoubleLinkedList() = default;											// empty constructor 
	DoubleLinkedList(DoubleLinkedList const &source);						// copy constructor

																			// Rule of 5
	DoubleLinkedList(DoubleLinkedList &&move) noexcept;						// move constructor
	DoubleLinkedList& operator=(DoubleLinkedList &&move) noexcept;			// move assignment operator
	~DoubleLinkedList() noexcept;

	// Overload operators
	DoubleLinkedList& operator=(DoubleLinkedList const &rhs);

	// Create an iterator class
	class iterator;
	iterator begin();
	iterator end();
	iterator before_begin();

	// Create const iterator class
	class const_iterator;
	const_iterator cbegin() const;
	const_iterator cend() const;
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator before_begin() const;
	const_iterator cbefore_begin() const;

	// Reverse iteator 
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	reverse_iterator rbegin() noexcept { return { end() }; }
	const_reverse_iterator rbegin() const noexcept { return { end() }; }
	const_reverse_iterator crbegin() const noexcept { return { end() }; }

	reverse_iterator rend() noexcept { return { begin() }; }
	const_reverse_iterator rend() const noexcept { return { begin() }; }
	const_reverse_iterator crend() const noexcept { return { begin() }; }

	// Memeber functions
	void swap(DoubleLinkedList &other) noexcept;
	bool empty() const { return head.get() == nullptr; }
	int size() const;

	template<typename... Args>
	void emplace_back(Args&&... args);

	template<typename... Args>
	void emplace_front(Args&&... args);

	template<typename... Args>
	iterator emplace(const_iterator pos, Args&&... args);

	void push_back(const T &theData);
	void push_back(T &&theData);
	void push_front(const T &theData);
	void push_front(T &&theData);
	iterator insert(const_iterator pos, const T& theData);
	iterator insert(const_iterator pos, T&& theData);
	void clear();
	void pop_front();
	void pop_back();
	iterator erase(const_iterator pos);
	bool search(const T &x);


};

template <class T>
class DoubleLinkedList<T>::iterator {
	Node* node = nullptr;
	bool end_reached = true;

public:
	friend class DoubleLinkedList<T>;

	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T * ;
	using reference = T & ;

	iterator(Node* node = nullptr, bool end_reached = false) : node{ node }, end_reached{ end_reached }  {}

	operator const_iterator() const noexcept { return const_iterator{ node }; }
	bool operator!=(iterator other) const noexcept;
	bool operator==(iterator other) const noexcept;

	T& operator*() const { return node->data; }
	T* operator->() const { return &node->data; }

	iterator& operator++();
	iterator operator++(int);
	iterator& operator--();
	iterator operator--(int);
};

template <class T>
class DoubleLinkedList<T>::const_iterator {
	Node* node = nullptr;
	bool end_reached = true;

public:
	friend class DoubleLinkedList<T>;

	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = const T *;
	using reference = const T &;

	const_iterator() = default;
	const_iterator(Node* node, bool end_reached = false) : node{ node }, end_reached { end_reached }  {}


	bool operator!=(const_iterator other) const noexcept;
	bool operator==(const_iterator other) const noexcept;

	const T& operator*() const { return node->data; }
	const T* operator->() const { return &node->data; }

	const_iterator& operator++();
	const_iterator operator++(int);
	const_iterator& operator--();
	const_iterator operator--(int);
};

template <class T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList<T> const &source) {
	for (Node* loop = source.head.get(); loop != nullptr; loop = loop->next.get()) {
		push_back(loop->data);
	}
}

template <class T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList<T>&& move) noexcept {
	move.swap(*this);
}

template <class T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList<T> &&move) noexcept {
	move.swap(*this);
	return *this;
}

template <class T>
DoubleLinkedList<T>::~DoubleLinkedList() {
	clear();
}

template <class T>
void DoubleLinkedList<T>::clear() {
	while (head) {
		do_pop_front();
	}
}

template <class T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList const &rhs) {
	SingleLinkedList copy{ rhs };
	swap(copy);
	return *this;
}

template <class T>
void DoubleLinkedList<T>::swap(DoubleLinkedList &other) noexcept {
	using std::swap;
	swap(head, other.head);
	swap(tail, other.tail);
}

template <class T>
int DoubleLinkedList<T>::size() const {
	int size = 0;
	for (auto current = head.get(); current != nullptr; current = current->next.get()) {
		size++;
	}
	return size;
}

template <class T>
template <typename... Args>
void DoubleLinkedList<T>::emplace_back(Args&&... args) {
	if (!head) emplace_front(std::forward<Args>(args)...);
	else {
		tail->next = std::make_unique<Node>(nullptr, tail, std::forward<Args>(args)...);
		tail = tail->next.get();
	}
}

template <class T>
template <typename... Args>
void DoubleLinkedList<T>::emplace_front(Args&&... args) {
	head = std::make_unique<Node>(std::move(head), nullptr, std::forward<Args>(args)...);
	if (!tail) tail = head.get(); // update tail if list was empty before
}


template <class T>
template <typename... Args>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::emplace(const_iterator pos, Args&&... args) {
	if (pos.end_reached) {
		emplace_back(std::forward<Args>(args)...);
		return end();
	}

	if (!head) {
		emplace_front(std::forward<Args>(args)...);
		return begin();
	}
	std::unique_ptr<Node> newNode = std::make_unique<Node>(std::forward<Args>(args)...);
	newNode->previous = pos.node->previous;
	newNode->next = std::move(pos.node->previous->next);
	pos.node->previous = newNode.get();
	newNode->previous->next = std::move(newNode);

	return  {pos.node->previous}; 
}

template <class T>
void DoubleLinkedList<T>::push_back(const T &theData) {
	std::unique_ptr<Node> newNode = std::make_unique<Node>(std::move(theData));
	newNode->previous = tail;

	if (!head) {
		head = std::move(newNode);
		tail = head.get();
	}
	else {
		tail->next = std::move(newNode);
		tail = tail->next.get();
	}
}

template <class T>
void DoubleLinkedList<T>::push_back(T &&thedata) {
	std::unique_ptr<Node> newNode = std::make_unique<Node>(std::move(thedata));
	newNode->previous = tail;

	if (!head) {
		head = std::move(newNode);
		tail = head.get();
	}

	else {
		tail->next = std::move(newNode);
		tail = tail->next.get();
	}
}


template <class T>
void DoubleLinkedList<T>::push_front(const T &theData) {
	head = std::make_unique<Node>(std::move(head), nullptr, theData);

	if (!(head->next)) {
		tail = head.get();
	}
}

template <class T>
void DoubleLinkedList<T>::push_front(T &&theData) {
	head = std::make_unique<Node>(std::move(head),nullptr,std::move(theData));

	if (!(head->next)) {
		tail = head.get();
	}
}


template <class T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::insert(const_iterator pos, const T& theData) {
	return emplace(pos, theData);
}

template <class T>
typename  DoubleLinkedList<T>::iterator DoubleLinkedList<T>::insert(const_iterator pos, T&& theData) {
	return emplace(pos, std::move(theData));
}

template <class T>
void DoubleLinkedList<T>::pop_front() {
	if (empty()) {
		throw std::out_of_range("List is Empty!!! Deletion is not possible.");
	}

	do_pop_front();
}

template <class T>
void DoubleLinkedList<T>::pop_back() {
	if (!head) {
		return;
	}

	if (head) {
		auto current = head.get();
		Node* prev = nullptr;
		while (current->next) {
			prev = current;
			current = current->next.get();
		}
		tail = prev;
		prev->next = nullptr;
	}
	else {
		throw std::out_of_range("The list is empty, nothing to delete.");
	}
}

template <class T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::erase(const_iterator pos) {
	if (pos.end_reached) {
		pop_back();
		return end();
	}

	if (pos.node && pos.node->next) {
		pos.node->next = std::move(pos.node->previous->next);
		return { pos.node->previous };
	}

	return begin();
}

template <class T>
bool DoubleLinkedList<T>::search(const T &x) {
	return std::find(begin(), end(), x) != end();
}

template <typename T>
std::ostream& operator<<(std::ostream &str, DoubleLinkedList<T>& list) {
	for (auto const& item : list) {
		str << item << "\t";
	}
	return str;
}


// Iterator Implementaion////////////////////////////////////////////////
template <class T>
typename DoubleLinkedList<T>::iterator& DoubleLinkedList<T>::iterator::operator++() {
	if (!node) return *this;

	if (node->next) {
		node = node->next.get();
	}
	
	else {
		end_reached = true;			// keep last node, so we can go backwards if required
	}

	return *this;
}

template<typename T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::iterator::operator++(int) {
	auto copy = *this;
	++*this;
	return copy;
}

template <class T>
typename DoubleLinkedList<T>::iterator& DoubleLinkedList<T>::iterator::operator--() {
	if (!node) return *this;

	if (end_reached) {
		end_reached = false;
	}

	else if (node->previous) {
		node = node->previous.get();
	}

	return *this;
}

template<typename T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::iterator::operator--(int) {
	auto copy = *this;
	--*this;
	return copy;
}

template<typename T>
bool DoubleLinkedList<T>::iterator::operator==(iterator other) const noexcept {
	if (end_reached) return other.end_reached;
	
	if (other.end_reached) return false;

	return node == other.node;
}

template<typename T>
bool DoubleLinkedList<T>::iterator::operator!=(iterator other) const noexcept {
	return !(*this == other);
}

template<class T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::begin() {
	return head.get();
}

template<class T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::end() {
	return {tail, true};
}

template <class T>
typename DoubleLinkedList<T>::iterator DoubleLinkedList<T>::before_begin() {
	return { head.get(), false };
}

// Const Iterator Implementaion////////////////////////////////////////////////
template <class T>
typename DoubleLinkedList<T>::const_iterator& DoubleLinkedList<T>::const_iterator::operator++() {
	if (!node) return *this;

	if (node->next) {
		node = node->next.get();
	}

	else {
		end_reached = true;			// keep last node, so we can go backwards if required
	}

	return *this;
}

template<typename T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::const_iterator::operator++(int) {
	auto copy = *this;
	++*this;
	return copy;
}

template <class T>
typename DoubleLinkedList<T>::const_iterator& DoubleLinkedList<T>::const_iterator::operator--() {
	if (!node) return *this;

	if (end_reached) {
		end_reached = false;
	}

	else if (node->previous) {
		node = node->previous.get();
	}

	return *this;
}

template<class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::const_iterator::operator--(int) {
	auto copy = *this;
	--*this;
	return copy;
}

template<class T>
bool DoubleLinkedList<T>::const_iterator::operator==(const_iterator other) const noexcept {
	if (end_reached) return other.end_reached;

	if (other.end_reached) return false;

	return node == other.node;
}

template<class T >
bool DoubleLinkedList<T>::const_iterator::operator!=(const_iterator other) const noexcept {
	return !(*this == other);
}


template <class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::begin() const {
	return head.get();
}

template <class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::end() const {
	return {tail, true};
}

template <class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::cbegin() const {
	return begin();
}

template <class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::cend() const {
	return end();
}

template <class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::before_begin() const {
	return { head.get(), true };
}

template <class T>
typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::cbefore_begin() const {
	return before_begin();
}


#endif