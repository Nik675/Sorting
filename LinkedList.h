#pragma once


#include <iostream>
#include <memory>

using std::ostream;
using std::shared_ptr;
using std::make_shared;
using std::runtime_error;

template<class Type>
struct Node {
	Type data;
	//Node<Type> *next;
	shared_ptr <Node<Type> > next;
};


/*
	LinkedListIterator is used for writing the test cases for the merge sort.  Do not remove.
*/
template<typename Type>
class LinkedListIterator {
public:
	LinkedListIterator(shared_ptr<Node <Type> > ptr) : current(ptr) {}
	LinkedListIterator<Type>& operator++() { current = current->next; return *this; }
	Type operator*() { return current->data; }
	bool operator==(const LinkedListIterator<Type>& rhs) const { return current == rhs.current; }
	bool operator!=(const LinkedListIterator<Type>& rhs) const { return current != rhs.current; }
	shared_ptr<Node <Type> > getNode() { return current; }

private:
	shared_ptr < Node<Type> >  current;
};


template<class Type>
class LinkedList;

template<class Type>
ostream& operator<< (ostream& out, const LinkedList<Type>& list);


template<class Type>
class LinkedList {
public:
	LinkedList() : front(nullptr), back(nullptr), count(0) {}// Default constructor for the LinkedList.  Creates an empty list by setting front and back of the list to nulllptr
	//~LinkedList();           //Destructor for the LinkedList.  Creates an empty list by setting front and back of the list to nulllptr
	void insert(Type data);  //Adds to the back of the list
	Type peek(int ndx)const;//Looks at an element at the given ndx value
	void remove(int ndx);    //Removes an element at the given ndx position
	friend ostream& operator<< <>(ostream& out, const LinkedList<Type>& list);// Returns an output stream for displaying the LinkedList
	

	void mergeSort();
	LinkedListIterator<Type> begin() { return LinkedListIterator<Type>(front); }

	void mergeSort(shared_ptr<Node <Type> > &head);
	shared_ptr<Node <Type> >  getMiddle(shared_ptr<Node <Type> > head);
	shared_ptr<Node <Type> > mergeInOrder(shared_ptr<Node <Type> > a, shared_ptr<Node <Type> > b);


protected:
	shared_ptr <Node<Type> > front;
	shared_ptr <Node<Type> > back;
	int count;//Number of nodes in list
};

template<class Type>
void LinkedList<Type>::insert(Type data) {
	auto temp = make_shared<Node<Type>>();
	temp->data = data;
	temp->next = nullptr;
	if (!front) {
		front = temp;
		back = temp;
	}
	else {
		back->next = temp;
		back = temp;
	}
	count++;
}
template<class Type>
Type LinkedList<Type>::peek(int ndx)const {
	if (ndx >= count || ndx < 0) {
		throw runtime_error("Out of range");
	}

	int currNodeNum = 0;
	auto currentNode = front;

	while (currNodeNum < ndx) {
		currNodeNum++;
		currentNode = currentNode->next;
	}
	return currentNode->data;
}

template<class Type>
void  LinkedList<Type>::remove(int ndx) {
	if (ndx >= count || ndx < 0) {
		throw runtime_error("Out of range");
	}

	//If removing the front
	if (ndx == 0) {
		auto toDelete = front;
		front = toDelete->next;
		//delete toDelete;
		return;
	}//end if
	int currNodeNum = 0;
	auto currentNode = front;

	while (currNodeNum < ndx - 1) {
		currNodeNum++;
		currentNode = currentNode->next;
	}
	auto toDelete = currentNode->next;
	//Deleting from middle
	if (toDelete->next) {
		currentNode->next = toDelete->next;
	}
	//Delete last node
	else {
		currentNode->next = nullptr;
		back = currentNode;
	}
	count--;
	//delete toDelete;
}//end remove

template<class Type>
ostream& operator<< (ostream& out, const LinkedList<Type>& list) {
	auto curr = list.front;

	while (curr) {
		out << curr->data;
		if (curr->next) {
			out << " ";
		}
		curr = curr->next;
	}
	return out;
}





template<class Type>
void  LinkedList<Type>::mergeSort() {
	mergeSort(front);
}

template<class Type>
void LinkedList<Type>::mergeSort(shared_ptr<Node<Type>>& head) {
    if (!head || !head->next) {
        return; // Base case: 0 or 1 node
    }

    auto mid = getMiddle(head);
    auto left = head;
    auto right = mid->next;
    mid->next = nullptr; // Split the list

    mergeSort(left);  // Sort left half
    mergeSort(right); // Sort right half

    head = mergeInOrder(left, right); // Merge sorted halves
}

template<class Type>
shared_ptr<Node<Type>> LinkedList<Type>::getMiddle(shared_ptr<Node<Type>> head) {
    if (!head) {
        return head;
    }

    auto slow = head;
    auto fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow; // Return the middle node
}

template<class Type>
shared_ptr<Node<Type>> LinkedList<Type>::mergeInOrder(shared_ptr<Node<Type>> a, shared_ptr<Node<Type>> b) {
    if (!a) {
        return b; // If first list is empty, return second list
    }
    if (!b) {
        return a; // If second list is empty, return first list
    }

    shared_ptr<Node<Type>> head;

    if (a->data < b->data) {
        head = a;
        head->next = mergeInOrder(a->next, b);
    } else {
        head = b;
        head->next = mergeInOrder(a, b->next);
    }

    return head;
}

