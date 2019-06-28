#include <iostream>
#include <queue>
#include <mutex>

using namespace std;

/* This class implements a thread-safe queue data structure
*/
class MessageQueue {
 public:
  void push(string message) {
    queueMutex_.lock();
    innerQueue_.push(message);
    queueMutex_.unlock();
  } 
  string top() {
    queueMutex_.lock();
    string result = innerQueue_.front();
    queueMutex_.unlock();
    return result;
  }
  void pop() {
    queueMutex_.lock();
    innerQueue_.pop();
    queueMutex_.unlock();
  }
  bool empty() {
    return innerQueue_.empty();
  }

 private:
  queue <string> innerQueue_;
  mutex queueMutex_;
};