#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

template<typename Item>
class Collection {
public:
  Collection();
  Collection(int size);
  Collection(Collection<Item>& a);
  virtual int size();
  virtual Item get(int ndx) const;
  virtual void add(Item e);
  virtual void removeEnd();
  virtual Collection<Item>& operator=(Collection<Item>& a);
  virtual Item& operator[](int ndx);
  virtual Collection<Item>& operator+(Item e) { add(e); return *this; }
  virtual Collection<Item>& operator-(Item e) { for (int i = 0; i < e; i++) removeEnd(); return *this; }
  virtual Collection<Item>& operator<<(Item e);
  friend ostream& operator<<(ostream& out, const Collection& c) {
    for (int i = 0; i < c.currentSize; i++) {
      out << c.elementArray[i];
      if (i < c.currentSize - 1) out << " ";
    }
    return out;
  }
  void swap(int, int);
  void sortByBubble();
  void sortBySelection();
  void sortByInsertion();
  void sortByQuick() { quicksort(0, currentSize - 1); }
  void quicksort(int start, int end);
  int partition(int start, int end);
  int findMinInRange(int start);

protected:
  int capacity;
  int currentSize;
  std::unique_ptr<Item[]> elementArray;
  static const int initialCapacity = 8;
  void expand();
};

template<typename Item>
Collection<Item>::Collection() {
  capacity = initialCapacity;
  currentSize = 0;
  elementArray = make_unique<Item[]>(capacity);
}

template<typename Item>
Collection<Item>::Collection(int size) {
  capacity = size;
  currentSize = 0;
  elementArray = make_unique<Item[]>(capacity);
}

template<typename Item>
Collection<Item>::Collection(Collection<Item>& a) {
  capacity = a.capacity;
  currentSize = a.currentSize;
  elementArray = make_unique<Item[]>(capacity);
  for (int i = 0; i < a.currentSize; i++) {
    elementArray[i] = a.elementArray[i];
  }
}

template<typename Item>
int Collection<Item>::size() {
  return currentSize;
}

template<typename Item>
Item Collection<Item>::get(int ndx) const {
  return elementArray[ndx];
}

template<typename Item>
void Collection<Item>::add(Item e) {
  if (currentSize == capacity) expand();
  elementArray[currentSize] = e;
  currentSize++;
}

template<typename Item>
void Collection<Item>::removeEnd() {
  currentSize--;
  if (currentSize < 0) throw runtime_error("Empty List");
}

template<typename Item>
Item& Collection<Item>::operator[](int ndx) {
  return elementArray[ndx];
}

template<typename Item>
Collection<Item>& Collection<Item>::operator=(Collection<Item>& a) {
  auto newElementArray = std::make_unique<Item[]>(a.capacity);
  for (int i = 0; i < a.currentSize; i++) newElementArray[i] = a.elementArray[i];
  elementArray = move(newElementArray);
  capacity = a.capacity;
  currentSize = a.currentSize;
  return *this;
}

template<typename Item>
Collection<Item>& Collection<Item>::operator<<(Item e) {
  add(e);
  return *this;
}

template<typename Item>
void Collection<Item>::expand() {
  auto newElementArray = std::make_unique<Item[]>(capacity * 2);
  for (int i = 0; i < capacity; i++) newElementArray[i] = elementArray[i];
  elementArray = std::move(newElementArray);
  capacity *= 2;
}

template<typename Item>
void Collection<Item>::swap(int first, int second) {
  Item temp = elementArray[first];
  elementArray[first] = elementArray[second];
  elementArray[second] = temp;
}

template<typename Item>
void Collection<Item>::sortByBubble() {
  for (int i = 0; i < currentSize - 1; i++) {
    for (int j = 0; j < currentSize - i - 1; j++) {
      if (elementArray[j] > elementArray[j + 1]) swap(j, j + 1);
    }
  }
}

template<typename Item>
void Collection<Item>::sortByInsertion() {
  for (int i = 1; i < currentSize; i++) {
    Item key = elementArray[i];
    int j = i - 1;
    while (j >= 0 && elementArray[j] > key) {
      elementArray[j + 1] = elementArray[j];
      j--;
    }
    elementArray[j + 1] = key;
  }
}

template<typename Item>
int Collection<Item>::partition(int start, int end) {
  Item pivot = elementArray[end]; // Use the last element as the pivot
  int i = start - 1; // Index of smaller element
  for (int j = start; j < end; j++) {
    if (elementArray[j] < pivot || (elementArray[j] == pivot && j < end)) {
      i++;
      swap(i, j);
    }
  }
  swap(i + 1, end); // Place pivot in the correct position
  return i + 1;
}

template<typename Item>
void Collection<Item>::quicksort(int start, int end) {
  if (start >= end) return;
  int pivotIndex = partition(start, end);
  quicksort(start, pivotIndex - 1);
  quicksort(pivotIndex + 1, end);
}

template<typename Item>
int Collection<Item>::findMinInRange(int start) {
  int minIndex = start;
  for (int i = start + 1; i < currentSize; i++) {
    if (elementArray[i] < elementArray[minIndex]) minIndex = i;
  }
  return minIndex;
}

template<typename Item>
void Collection<Item>::sortBySelection() {
  for (int i = 0; i < currentSize - 1; i++) {
    int minIndex = findMinInRange(i);
    swap(i, minIndex);
  }
}





