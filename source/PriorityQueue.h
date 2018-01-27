#pragma once

#include <cassert>
#include "vector"

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

  void eventDecreaseKey(int i);
  void eventMinHeapify(int i);
  inline int heapParent(int i);
  inline int heapLeft(int i);
  inline int heapRight(int i);

private:
  std::vector<Event> mHeap;
};
