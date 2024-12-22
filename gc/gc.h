#pragma once
#include <unistd.h>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <limits>
#include <malloc.h>
#include "debug.h"
#define MEGABYTE_TO_BYTE 1000000

class CollectedHeap;

template <typename T>
class TrackingAllocator {
protected:
  int total_bytes_allocated_;
  int total_bytes_deallocated_;
public:
  CollectedHeap* heap_;
  using value_type = T; // The type of objects the allocator manages
  using pointer = T*;
  using const_pointer = const T*;
  using void_pointer = void*;
  using const_void_pointer = const void*;

  using size_type = std::size_t;   // Type for sizes
  using difference_type = std::ptrdiff_t; // Type for differences
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

  TrackingAllocator() noexcept : heap_(nullptr), total_bytes_allocated_(0), total_bytes_deallocated_(0) {}

  template <typename U>
    TrackingAllocator(const TrackingAllocator<U>& alloc) noexcept
        : heap_(alloc.heap_), total_bytes_allocated_(0), total_bytes_deallocated_(0) {}

  void setHeap(CollectedHeap* heap) {
    heap_ = heap;
  }

  int getCurrentMemory() const {
    return total_bytes_allocated_ - total_bytes_deallocated_;
  }

  T* allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        throw std::bad_alloc();
    total_bytes_allocated_ += n * sizeof(T);
    heap_->addMemory(n * sizeof(T));
    return static_cast<T*>(::operator new(n * sizeof(T)));
  }

  void deallocate(T* p, std::size_t n) noexcept {
    total_bytes_deallocated_ += n * sizeof(T);
    heap_->addMemory(-n * sizeof(T));
    ::operator delete(p);
  }

  template <typename... Args>
  void construct(T* p, Args&&... args) {
      ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
  }

  void destroy(T* p) noexcept {
    p->~T();
  }

  std::size_t max_size() const noexcept {
    return std::numeric_limits<std::size_t>::max() / sizeof(T);
  }

  // Rebind structure to propagate the heap
  template <typename U>
  struct rebind {
      using other = TrackingAllocator<U>;
  };

  bool operator==(const TrackingAllocator& other) const noexcept {
        return heap_ == other.heap_;
    }

  bool operator!=(const TrackingAllocator& other) const noexcept {
      return !(*this == other);
  }

};

template <typename T>
using TrackingVector = std::vector<T, TrackingAllocator<T>>;

template <typename T>
using TrackingSet = std::set<T, std::less<>, TrackingAllocator<T>>;

template <typename T>
using TrackingUnorderedSet = std::unordered_set<T, std::hash<T>, std::equal_to<T>, TrackingAllocator<T>>;

template <typename K, typename V>
using TrackingUnorderedMap = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, TrackingAllocator<std::pair<const K, V>>>;

using TrackingString = std::basic_string<char, std::char_traits<char>, TrackingAllocator<char>>;


template <typename T>
class LinkedList {
  T* head_;
  int size_;
public:
  LinkedList() : head_(nullptr), size_(0) {}

  int size() {
    return size_;
  }

  void push_back(T* node) {
    node->next_ = head_;
    head_ = node;
    size_++;
  }
  // Iterator Class
    class Iterator {
    public:
        T* previous_;
        T* current_;
        // Constructor
        explicit Iterator(T* node) : current_(node), previous_(nullptr) {}
        explicit Iterator(T* node, T* previous) : current_(node), previous_(previous) {}

        // Dereference operator
        T* operator*() const {
            return current_;
        }

        // Pre-increment operator
        Iterator& operator++() {
            if (current_ != nullptr) {
                previous_ = current_;
                current_ = current_->next_;
            }
            return *this;
        }

        // Post-increment operator
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        // Equality operator
        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        // Inequality operator
        bool operator!=(const Iterator& other) const {
            return current_ != other.current_;
        }
    };

    // Begin iterator
    Iterator begin() const {
        return Iterator(head_);
    }

    // End iterator
    Iterator end() const {
        return Iterator(nullptr);
    }

    // Erase method
    Iterator erase(Iterator it) {
      if (it == end()) {
        return end();
      }

      size_--;
      T* node_to_delete = it.current_;
      
      if (node_to_delete == head_) { // Erasing head
        head_ = head_->next_;
        return begin();
      }

      T* previous = it.previous_;
      previous->next_ = node_to_delete->next_;
      return Iterator(previous->next_, previous);
    }

};


class CollectedHeap;



// Any object that inherits from collectable can be created and tracked by the
// garbage collector.
class Collectable{
 public:
  virtual ~Collectable() = default;
  bool marked_ = false;
  CollectedHeap* heap_;

 private:
  // Any private fields you add to the Collectable class will be accessible by
  // the CollectedHeap (since it is declared as friend below).  You can think of
  // these fields as the header for the object, which will include metadata that
  // is useful for the garbage collector.
  void mark() { marked_ = true; }
  void unmark() { marked_ = false; }
  Collectable* next_= nullptr;

 protected:
  /*
  The mark phase of the garbage collector needs to follow all pointers from the
  collectable objects, check if those objects have been marked, and if they
  have not, mark them and follow their pointers.  The simplest way to implement
  this is to require that collectable objects implement a follow method that
  calls heap.markSuccessors(...) on all collectable objects that this object
  points to.  markSuccessors() is the one responsible for checking if the
  object is marked and marking it.
  */
  virtual void follow(CollectedHeap& heap) = 0;
  virtual void calculateBaseSizeBytes() = 0;
  virtual size_t getCurrentSize() = 0;
  virtual void initializeDynamicMemory(CollectedHeap* heap) = 0;
  void setHeap(CollectedHeap* heap) {
    heap_ = heap;
  }
  int base_size_bytes_;

  friend CollectedHeap;
  friend LinkedList<Collectable>;
};

/*
  This class keeps track of the garbage collected heap. The class must:
  - provide and implement method(s) for allocating objects that will be
    supported by garbage collection
  - keep track of all currently allocated objects
  - provide and implement a method that performs mark and sweep to deallocate
    objects that are not reachable from a given set of objects
*/
class CollectedHeap {
  int max_memory_bytes_;
  int current_memory_bytes_ = sizeof(*this);

 public:
  LinkedList<Collectable> objects_{};
  CollectedHeap() : max_memory_bytes_(4*MEGABYTE_TO_BYTE) {}
  CollectedHeap(int max_memory_bytes) : max_memory_bytes_(max_memory_bytes) {}

  size_t getMemoryUsage() {
    std::ifstream statm("/proc/self/statm");
    size_t totalMemory;
    statm >> totalMemory; // Total memory in pages
    return totalMemory * sysconf(_SC_PAGESIZE); // Convert to bytes
  }
  size_t getCurrentHeapUsage() {
      struct mallinfo2 info = mallinfo2();
      return info.uordblks; // Bytes allocated by malloc
  }

  void dump() {
    // Current tracked memory usage
    int trackedUsage = current_memory_bytes_;

    // Total memory usage of the process
    int totalUsage = getCurrentHeapUsage();
    int allocatedMemory = getMemoryUsage();

    // Number of objects currently registered in the heap
    size_t objectCount = objects_.size();

    // Print heap statistics
    std::cout << "-------------------" << std::endl;
    std::cout << "CollectedHeap Dump:" << std::endl;
    std::cout << "Tracked Memory Usage: " << trackedUsage / 1000 << " kb" << std::endl;
    std::cout << "Total Process Allocated Memory Usage: " << allocatedMemory / 1000 << " kb" << std::endl;
    std::cout << "Total Process Runtime Memory Usage: " << totalUsage / 1000 << " kb" << std::endl;
    std::cout << "Max Memory Capacity: " << max_memory_bytes_ / 1000 << " kb" << std::endl;
    std::cout << "Number of Registered Objects: " << objectCount << std::endl;
    std::cout << "-------------------" << std::endl << std::endl;

    // Print details of registered objects
    // std::cout << "Registered Objects:" << std::endl;
    // for (const auto& obj : objects_) {
    //     std::cout << "  - Object at " << obj
    //               << ", Base Size: " << obj->base_size_bytes_
    //               << " bytes" << std::endl;
    // }
}


  /*
  This method allocates an object of type T, passing args to the constructor.
  T must be a subclass of Collectable.  Before returning the
  object, it should be registered so that it can be deallocated later.
  */
  template <typename T, typename... Args>
  T* allocate(Args&&... args) {
    // DEBUG_PRINT("Allocating object of type " << typeid(T).name());
    static_assert(std::is_constructible_v<T, Args...>);
    T* obj = new T(std::forward<Args>(args)...);
    objects_.push_back(obj);
    obj->setHeap(this);
    obj->calculateBaseSizeBytes();
    addMemory(obj->base_size_bytes_);
    obj->initializeDynamicMemory(this);
    return obj;
  }

  int getSize() {
    return current_memory_bytes_;
  }

  bool isFull() {
    return current_memory_bytes_ >= max_memory_bytes_ * 0.5;
  }

  void addMemory(int bytes) {
    // DEBUG_PRINT("Adding " << bytes << " bytes to the heap");
    current_memory_bytes_ += bytes;
  }

  /*
  For performance reasons, you may also want to specialize the allocate
  methods to allocate particular kinds of objects...
  */

  /*
  This is the method that is called by the follow(...) method of a Collectable
  object.  This is how a Collectable object lets the garbage collector know
  about other Collectable objects pointed to by itself.
  */
  void markSuccessors(Collectable* next) {
    if (next->marked_) return;
    next->mark();
    next->follow(*this);
  }

  /*
  The gc method should be periodically invoked by your VM (or by other methods
  in CollectedHeap) whenever the VM decides it is time to reclaim memory.  This
  method triggers the mark and sweep process.  How frequently the VM invokes
  this method should be computed based on the -mem parameter passed to the VM.
  Keep in mind, however, that your VM could be using some extra memory that is
  not managed by the garbage collector, so make sure you account for this.

  The Iterator type should support comparison, assignment and the ++ operator.
  It should also be able to dereference an iterator to get a Collectable
  object.  This method will take iterators marking the [begin, end) range of
  the rootset as arguments.
  */
  template <typename Iterator>
  void gc(Iterator begin, Iterator end) {
    // Mark phase
    for (Iterator it = begin; it != end; ++it) {
      markSuccessors(*it);
    }

    // Sweep phase
    for (auto it = objects_.begin(); it != objects_.end();) {
      if (!(*it)->marked_) {
        current_memory_bytes_ -= (*it)->base_size_bytes_;
        auto obj = *it;
        it = objects_.erase(it);
        delete obj;
      } else {
        (*it)->unmark();
        ++it;
      }
    }
  }
};



