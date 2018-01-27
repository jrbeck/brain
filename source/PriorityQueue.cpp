#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() {}

PriorityQueue::~PriorityQueue() {}

void PriorityQueue::clear() {
  mHeap.clear();
}

size_t PriorityQueue::size() {
  return mHeap.size();
}

void PriorityQueue::insert(const Event& event) {
  mHeap.push_back(event);
  eventDecreaseKey((int)mHeap.size() - 1);
}

bool PriorityQueue::extract(Event* event) {
  if (mHeap.size() == 0) {
    return false;
  }

  if (event != nullptr) {
    *event = mHeap[0];
  }

  mHeap[0] = mHeap[mHeap.size() - 1];
  mHeap.pop_back();
  eventMinHeapify(0);
  return true;
}

bool PriorityQueue::peek(Event* event) {
  if (mHeap.size() == 0) {
    return false;
  }

  if (event != nullptr) {
    *event = mHeap[0];
  }

  return true;
}

void PriorityQueue::eventDecreaseKey(int i) {
  int parent;

  while (i > 0 && mHeap[parent = heapParent(i)].time > mHeap[i].time) {
    Event temp = mHeap[i];
    mHeap[i] = mHeap[parent];
    mHeap[parent] = temp;

    i = parent;
  }
}

void PriorityQueue::eventMinHeapify(int i) {
  int leftIndex = heapLeft(i);
  int rightIndex = heapRight(i);

  int smallest;

  if (leftIndex < (int)mHeap.size() && mHeap[leftIndex].time < mHeap[i].time) {
    smallest = leftIndex;
  }
  else {
    smallest = i;
  }

  if (rightIndex < (int)mHeap.size() && mHeap[rightIndex].time < mHeap[smallest].time) {
    smallest = rightIndex;
  }

  if (smallest != i) {
    Event temp = mHeap[i];
    mHeap[i] = mHeap[smallest];
    mHeap[smallest] = temp;

    eventMinHeapify(smallest);
  }
}

inline int PriorityQueue::heapParent(int i) {
  return ((i + 1) >> 1) - 1;
}

inline int PriorityQueue::heapLeft(int i) {
  return ((i + 1) << 1) - 1;
}

inline int PriorityQueue::heapRight(int i) {
  return (i + 1) << 1;
}
