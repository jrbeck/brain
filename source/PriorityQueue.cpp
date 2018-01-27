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
  decreaseKey(mHeap.size() - 1);
}

bool PriorityQueue::extract(Event* event) {
  size_t heapSize = mHeap.size();
  if (heapSize == 0) {
    return false;
  }

  if (event != nullptr) {
    *event = mHeap[0];
  }

  mHeap[0] = mHeap[heapSize - 1];
  mHeap.pop_back();
  minHeapify(0);
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

void PriorityQueue::decreaseKey(size_t index) {
  size_t parentIndex;

  while (index != 0 && mHeap[parentIndex = heapParent(index)].time > mHeap[index].time) {
    Event temp = mHeap[index];
    mHeap[index] = mHeap[parentIndex];
    mHeap[parentIndex] = temp;
    index = parentIndex;
  }
}

void PriorityQueue::minHeapify(size_t index) {
  size_t leftIndex = heapLeft(index);
  size_t rightIndex = heapRight(index);

  size_t smallestIndex;
  size_t heapSize = mHeap.size();

  if (leftIndex < heapSize && mHeap[leftIndex].time < mHeap[index].time) {
    smallestIndex = leftIndex;
  }
  else {
    smallestIndex = index;
  }

  if (rightIndex < heapSize && mHeap[rightIndex].time < mHeap[smallestIndex].time) {
    smallestIndex = rightIndex;
  }

  if (smallestIndex != index) {
    Event temp = mHeap[index];
    mHeap[index] = mHeap[smallestIndex];
    mHeap[smallestIndex] = temp;
    minHeapify(smallestIndex);
  }
}

inline size_t PriorityQueue::heapParent(size_t index) {
  if (index == 0) {
    // should this explode with an overflow instead?
    return 0;
  }
  return ((index + 1) >> 1) - 1;
}

inline size_t PriorityQueue::heapLeft(size_t index) {
  return ((index + 1) << 1) - 1;
}

inline size_t PriorityQueue::heapRight(size_t index) {
  return (index + 1) << 1;
}
