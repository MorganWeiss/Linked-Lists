//
//  SingleLinkedList.h
//  Data Structure - LinkedList
//
//  Created by Morgan Weiss on 7/24/2018
//  Copyright © 2018 Morgan Weiss. All rights reserved.
//

#ifndef SINGLELINKEDLIST_h
#define SINGLELINKEDLIST_h



template <class T>
class SingleLinkedList {
private:

	struct Node {
		T data;
		std::unique_ptr<Node> next = nullptr;

		template<typename... Args, typename = std::enable_if_t<std::is_constructible<T, Args&&...>::value>>
		explicit Node(std::unique_ptr<Node>&& next, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args&&...>::value)
			: data{ std::forward<Args>(args)... }, next{ std::move(next) } {}

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
	SingleLinkedList() = default;                                           // empty constructor 
	SingleLinkedList(SingleLinkedList const &source);                       // copy constructor

																			// Rule of 5
	SingleLinkedList(SingleLinkedList &&move) noexcept;                     // move constructor
	SingleLinkedList& operator=(SingleLinkedList &&move) noexcept;          // move assignment operator
	~SingleLinkedList() noexcept;

	// Overload operators
	SingleLinkedList& operator=(SingleLinkedList const &rhs);

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

	// Memeber functions
	void swap(SingleLinkedList &other) noexcept;
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
	iterator insert_after(const_iterator pos, const T& theData);
	iterator insert_after(const_iterator pos, T&& theData);
	void clear();
	void pop_front();
	void pop_back();
	iterator erase_after(const_iterator pos);
	bool search(const T &x);



};

template <class T>
class SingleLinkedList<T>::iterator {
	Node* node = nullptr;
	bool before_begin = false;

public:
	friend class SingleLinkedList<T>;

	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T * ;
	using reference = T & ;

	iterator(Node* node = nullptr, bool before = false) : node{ node }, before_begin{ before } {}

	operator const_iterator() const noexcept { return const_iterator{ node, before_begin }; }
	bool operator!=(iterator other) const noexcept;
	bool operator==(iterator other) const noexcept;

	T& operator*() const { return node->data; }
	T* operator->() const { return &node->data; }

	iterator& operator++();
	iterator operator++(int);
};

template <class T>
class SingleLinkedList<T>::const_iterator {
	Node* node = nullptr;
	bool before_begin = false;

public:
	friend class SingleLinkedList<T>;

	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = const T * ;
	using reference = const T & ;

	const_iterator() = default;
	const_iterator(Node* node, bool before = false) : node{ node }, before_begin{ before } {}

	
	bool operator!=(const_iterator other) const noexcept;
	bool operator==(const_iterator other) const noexcept;

	const T& operator*() const { return node->data; }
	const T* operator->() const { return &node->data; }

	const_iterator& operator++();
	const_iterator operator++(int);
};


template <class T>
SingleLinkedList<T>::SingleLinkedList(SingleLinkedList<T> const &source) {
	for (Node* loop = source.head.get(); loop != nullptr; loop = loop->next.get()) {
		emplace_back(loop->data);
	}
}

template <class T>
SingleLinkedList<T>::SingleLinkedList(SingleLinkedList<T>&& move) noexcept {
	move.swap(*this);
}

template <class T>
SingleLinkedList<T>& SingleLinkedList<T>::operator=(SingleLinkedList<T> &&move) noexcept {
	move.swap(*this);
	return *this;
}

template <class T>
SingleLinkedList<T>::~SingleLinkedList() noexcept {
	clear();
}

template <class T>
void SingleLinkedList<T>::clear() {
	while (head) {
		do_pop_front();
	}
}

template <class T>
SingleLinkedList<T>& SingleLinkedList<T>::operator=(SingleLinkedList const &rhs) {
	SingleLinkedList copy{ rhs };
	swap(copy);
	return *this;
}

template <class T>
void SingleLinkedList<T>::swap(SingleLinkedList &other) noexcept {
	using std::swap;
	swap(head, other.head);
	swap(tail, other.tail);
}

template <class T>
int SingleLinkedList<T>::size() const {
	int size = 0;
	for (auto current = head.get(); current != nullptr; current = current->next.get()) {
		size++;
	}
	return size;
}


template <class T>
template <typename... Args>
void SingleLinkedList<T>::emplace_back(Args&&... args) {
	std::unique_ptr<Node> newnode = std::make_unique<Node>(std::forward<Args>(args)...);

	if (!head) {
		head = std::move(newnode);
		tail = head.get();
	}

	else {
		tail->next = std::move(newnode);
		tail = tail->next.get();
	}
}

template <class T>
template <typename... Args>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::emplace(const_iterator pos, Args&&... args) {
	if (pos.before_begin) {
		emplace_front(std::forward<Args>(args)...);
		return begin();
	}

	if (pos.node) {
		pos.node->next = std::make_unique<Node>(std::move(pos.node->next), std::forward<Args>(args)...);  // Creating a new node that has the old next pointer with the new value and assign it to the next pointer of the current node 
		if (pos.node == tail) tail = tail->next.get();
		return { pos.node->next.get() };
	}
	throw std::out_of_range{ "end iterator got passed to insert!" };
}

template <class T>
void SingleLinkedList<T>::push_back(const T &theData) {
	std::unique_ptr<Node> newNode = std::make_unique<Node>(theData);

	if (!head) {
		head = std::move(theData);
		tail = head.get();
	}
	else {
		tail->next = std::move(theData);
		tail = tail->next.get();
	}
}

template <class T>
void SingleLinkedList<T>::push_back(T &&theData) {
	std::unique_ptr<Node> newNode = std::make_unique<Node>(std::move(theData));

	if (!head) {
		head = std::move(theData);
		tail = head.get();
	}
	else {
		tail->next = std::move(theData);
		tail = tail->next.get();
	}
}


template <class T>
template <typename... Args>
void SingleLinkedList<T>::emplace_front(Args&&... args) {
	head = std::make_unique<Node>(std::move(head), std::forward<Args>(args)...);
	if (!tail) tail = head.get(); // update tail if list was empty before
}


template <class T>
void SingleLinkedList<T>::push_front(const T &theData) {
	std::unique_ptr<Node> newNode = std::make_unique<Node>(theData);
	newNode->next = std::move(head);
	head = std::move(newNode);

	if (!tail) {
		tail = head.get();
	}
}

template <class T>
void SingleLinkedList<T>::push_front(T &&theData) {
	std::unique_ptr<Node> newNode = std::make_unique<Node>(std::move(theData));
	newNode->next = std::move(head);
	head = std::move(newNode);

	if (!tail) {
		tail = head.get();
	}
}

template <class T>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::insert_after(const_iterator pos, const T& theData) {
	return emplace(pos, theData);
}

template <class T>
typename  SingleLinkedList<T>::iterator SingleLinkedList<T>::insert_after(const_iterator pos, T&& theData)
{
	return emplace(pos, std::move(theData));
}

template <class T>
void SingleLinkedList<T>::pop_front() {
	if (empty()) {
		return;
	}
	do_pop_front();
}

template <class T>
void SingleLinkedList<T>::pop_back() {
	if (!head) return;

	auto current = head.get();
	Node* previous = nullptr;

	while (current->next) {
		previous = current;
		current = current->next.get();
	}
	if (previous) {
		previous->next = nullptr;
	}
	else {
		head = nullptr;
	}
	tail = previous;
	previous->next = nullptr;
}

template <class T>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::erase_after(const_iterator pos) {
	if (pos.before_begin) {
		pop_front();
		return begin();
	}

	if (pos.node && pos.node->next) {
		pos.node->next = std::move(pos.node->next->next);
		return { pos.node->next.get() };
	}

	return end();
}

template <class T>
bool SingleLinkedList<T>::search(const T &x) {
	return std::find(begin(), end(), x) != end();
}

template <class T>
std::ostream& operator<<(std::ostream &str, SingleLinkedList<T>& list) {
	for (auto const& item : list) {
		str << item << "\t";
	}
	return str;
}

// Iterator Implementaion////////////////////////////////////////////////
template <class T>
typename SingleLinkedList<T>::iterator& SingleLinkedList<T>::iterator::operator++() {
	if (before_begin) before_begin = false;
	else node = node->next.get();

	return *this;
}

template<typename T>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::iterator::operator++(int) {
	auto copy = *this;
	++*this;
	return copy;
}

template<typename T>
bool SingleLinkedList<T>::iterator::operator==(iterator other) const noexcept {
	return node == other.node && before_begin == other.before_begin;
}

template<typename T>
bool SingleLinkedList<T>::iterator::operator!=(iterator other) const noexcept {
	return !(*this == other);
}



template<class T>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::begin() {
	return head.get();
}

template<class T>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::end() {
	return {};
}

template <class T>
typename SingleLinkedList<T>::iterator SingleLinkedList<T>::before_begin() {
	return { head.get(), true };
}

// Const Iterator Implementaion////////////////////////////////////////////////
template <class T>
typename SingleLinkedList<T>::const_iterator& SingleLinkedList<T>::const_iterator::operator++() {
	if (before_begin) before_begin = false;
	else node = node->next.get();

	return *this;
}

template<typename T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::const_iterator::operator++(int) {
	auto copy = *this;
	++*this;
	return copy;
}

template<typename T>
bool SingleLinkedList<T>::const_iterator::operator==(const_iterator other) const noexcept {
	return node == other.node && before_begin == other.before_begin;
}

template<typename T>
bool SingleLinkedList<T>::const_iterator::operator!=(const_iterator other) const noexcept {
	return !(*this == other);
}


template <class T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::begin() const {
	return head.get();
}

template <class T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::end() const {
	return {};
}

template <class T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::cbegin() const {
	return begin();
}

template <class T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::cend() const {
	return end();
}

template <class T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::before_begin() const {
	return { head.get(), true };
}

template <class T>
typename SingleLinkedList<T>::const_iterator SingleLinkedList<T>::cbefore_begin() const {
	return before_begin();
}

#endif /* SingleLinkedList_h*/