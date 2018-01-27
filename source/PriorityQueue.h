#pragma once

#include <vector>

struct Event {
  double time;
  int target;
  double charge;
};

class PriorityQueue {
public:
  PriorityQueue();
  ~PriorityQueue();

  void clear();
  size_t size();

  void insert(const Event& event);
  bool extract(Event* event);
  bool peek(Event* event);

  void decreaseKey(size_t index);
  void minHeapify(size_t index);
  inline size_t heapParent(size_t index);
  inline size_t heapLeft(size_t index);
  inline size_t heapRight(size_t index);

private:
  std::vector<Event> mHeap;
};
