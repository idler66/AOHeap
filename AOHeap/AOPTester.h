//
//  AOPTester.h
//  Adjacent ordered heap
//
//  Created by 嘟嘟 on 2024/12/25.
//

#ifndef AOPTester_h
#define AOPTester_h

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/skew_heap.hpp>

#include "AOHeap.h"


static const long int  AOPDAryArity = 3;
static const bool AOPStatClocks = true;

template <typename KeyType, typename DataType>
struct AOHBoostNode{
  KeyType key;
  unsigned long nid;
  DataType data;
  bool operator<(const AOHBoostNode& other) const {
          return key > other.key;
  }
};

template <typename KeyType, typename DataType>
class AOPTester {
public:
  long int nodeNum;
  clock_t startTime;
  
  virtual std::string name() { assert(0); return ""; };
  
  virtual void decrease(unsigned long& nid, KeyType& keydlt) {}
  virtual void increase(unsigned long& nid, KeyType& keydlt) {}
  
  virtual void decreaseWithKey(unsigned long& nid, KeyType& key) {}
  virtual void increaseWithKey(unsigned long& nid, KeyType& key) {}

  virtual void insert(KeyType& key, unsigned long& nid) {}

  void init(unsigned long nc) {
    nodeNum = nc;
  }
  
  virtual bool empty() {
    return false;
  }
  
  void startStatTime() {
    if(AOPStatClocks) {
      startTime = clock();
    }
  }
  void finishStatTime(clock_t& var) {
    if(AOPStatClocks) {
      var += clock() - startTime;
    }
  }
    
  virtual bool popOne() {
    return false;
  }
};

template <typename KeyType, typename DataType>
class FibonacciTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::fibonacci_heap<AOHBoostNode<KeyType, DataType>> AOPFibonacciHeap;
  AOPFibonacciHeap fibonacciHeap;
  std::vector<typename AOPFibonacciHeap::handle_type> handles;
public:
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "Fibonacci";
  }
  
  FibonacciTester(unsigned long nc) {
    init(nc);
    handles.resize(nc);
  }
  bool empty() {
    return fibonacciHeap.empty();
  }
  auto top() {
    return fibonacciHeap.top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    handles[nid] = fibonacciHeap.push({key, nid});
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPFibonacciHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key - keydlt, nid};
      fibonacciHeap.decrease(handles[nid], new_value);
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPFibonacciHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key + keydlt, nid};
      fibonacciHeap.increase(handles[nid], new_value);
    }
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPFibonacciHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      fibonacciHeap.decrease(handles[nid], new_value);
    }
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPFibonacciHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      fibonacciHeap.increase(handles[nid], new_value);
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = fibonacciHeap.top();
      handles[value.nid] = typename AOPFibonacciHeap::handle_type();
      fibonacciHeap.pop();
      return true;
    }
    return false;
  }
};

template <typename KeyType, typename DataType>
class PairingTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::pairing_heap<AOHBoostNode<KeyType, DataType>> AOPPairingHeap;
  AOPPairingHeap pairingHeap;
  std::vector<typename AOPPairingHeap::handle_type> handles;

public:
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "Pairing";
  }
  ~PairingTester() {
    while(popOne()) {
      assert(0);
    }
    handles.clear();
  }
  PairingTester(unsigned long nc) {
    init(nc);
    handles.resize(nc);
  }
  bool empty() {
    return pairingHeap.empty();
  }
  auto top() {
    return pairingHeap.top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    handles[nid] = pairingHeap.push({key, nid});
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPPairingHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key - keydlt, nid};
      pairingHeap.decrease(handles[nid], new_value);
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPPairingHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key + keydlt, nid};
      pairingHeap.increase(handles[nid], new_value);
    }
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPPairingHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      pairingHeap.decrease(handles[nid], new_value);
    }
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPPairingHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      pairingHeap.increase(handles[nid], new_value);
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = pairingHeap.top();
      handles[value.nid] = typename AOPPairingHeap::handle_type();
      pairingHeap.pop();
      return true;
    }
    return false;
  }
};


template <typename KeyType, typename DataType>
class DAryTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::d_ary_heap<AOHBoostNode<KeyType, DataType>, boost::heap::arity<AOPDAryArity>, boost::heap::mutable_<true>> AOPDAryHeap;
  AOPDAryHeap DAryHeap;
  std::vector<typename AOPDAryHeap::handle_type> handles;

public:
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "DAry";
  }
  ~DAryTester() {
    while(popOne()) {
      assert(0);
    }
    handles.clear();
  }
  DAryTester(unsigned long nc) {
    init(nc);
    handles.resize(nc);
  }
  bool empty() {
    return DAryHeap.empty();
  }
  auto top() {
    return DAryHeap.top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    handles[nid] = DAryHeap.push({key, nid});
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPDAryHeap::handle_type()) {
      assert((*(handles[nid])).nid==nid);
      AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
      DAryHeap.decrease(handles[nid], new_value);
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPDAryHeap::handle_type()) {
      auto key = (*(handles[nid])).key;
      const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
      DAryHeap.increase(handles[nid], new_value);
//      fh.update(handles[nid], new_value);
    }
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPDAryHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      DAryHeap.decrease(handles[nid], new_value);
    }
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPDAryHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      DAryHeap.increase(handles[nid], new_value);
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = DAryHeap.top();
      handles[value.nid] =typename AOPDAryHeap::handle_type();
      DAryHeap.pop();
      return true;
    }
    return false;
  }
};
 

template <typename KeyType, typename DataType>
class BinomialTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::binomial_heap<AOHBoostNode<KeyType, DataType>> AOPBinomialHeap;
  AOPBinomialHeap binomialHeap;
  std::vector<typename AOPBinomialHeap::handle_type> handles;

public:
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "Binomial";
  }
  ~BinomialTester() {
    while(popOne()) {
      assert(0);
    }
    handles.clear();
  }
  BinomialTester(unsigned long nc) {
    init(nc);
    handles.resize(nc);
  }
  bool empty() {
    return binomialHeap.empty();
  }
  auto top() {
    return binomialHeap.top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    handles[nid] = binomialHeap.push({key, nid});
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPBinomialHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
      binomialHeap.decrease(handles[nid], new_value);
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPBinomialHeap::handle_type()) {
      auto key = (*(handles[nid])).key;
      const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
      binomialHeap.increase(handles[nid], new_value);
//      fh.update(handles[nid], new_value);
    }
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPBinomialHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      binomialHeap.decrease(handles[nid], new_value);
    }
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPBinomialHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      binomialHeap.increase(handles[nid], new_value);
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = binomialHeap.top();
      handles[value.nid] =typename AOPBinomialHeap::handle_type();
      binomialHeap.pop();
      return true;
    }
    return false;
  }
};


template <typename KeyType, typename DataType>
class SkewTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::skew_heap<AOHBoostNode<KeyType, DataType>, boost::heap::mutable_<true>> AOPSkewHeap;
  AOPSkewHeap skewHeap;
  std::vector<typename AOPSkewHeap::handle_type> handles;

public:
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "Skew";
  }
  ~SkewTester() {
    while(popOne()) {
      assert(0);
    }
    handles.clear();
  }
  SkewTester(unsigned long nc) {
    init(nc);
    handles.resize(nc);
  }
  bool empty() {
    return skewHeap.empty();
  }
  auto top() {
    return skewHeap.top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    handles[nid] = skewHeap.push({key, nid});
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPSkewHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
      skewHeap.decrease(handles[nid], new_value);
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    if(handles[nid]!=typename AOPSkewHeap::handle_type()) {
      auto key = (*(handles[nid])).key;
      const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
      skewHeap.increase(handles[nid], new_value);
//      fh.update(handles[nid], new_value);
    }
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPSkewHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      skewHeap.decrease(handles[nid], new_value);
    }
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    if(handles[nid]!=typename AOPSkewHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {key, nid};
      skewHeap.increase(handles[nid], new_value);
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = skewHeap.top();
      handles[value.nid] =typename AOPSkewHeap::handle_type();
      skewHeap.pop();
      return true;
    }
    return false;
  }
};

template <typename KeyType, typename DataType>
class AOHeapTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::init;
 
  AOHeap<KeyType, DataType> * aoHeap;
  std::vector<AOHeapNode<KeyType, DataType>*> NodePointer;
public:
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "AOH";
  }
  ~AOHeapTester() {
    while(popOne()) {
      assert(0);
    }
//    handles.clear();
  }
  AOHeapTester(unsigned long nc) {
    NodePointer.resize(nc);
    init(nc);
    aoHeap = new AOHeap<KeyType, DataType>(nc);
  }
  bool empty() {
    return aoHeap->empty();
  }
  auto top() {
    return aoHeap->top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    NodePointer[nid] = aoHeap->insert(key, nid);
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
//    if(!keyChangeNum)return;
    if(keydlt == 0) return;
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    if(node) {
      node->key -= keydlt;
      aoHeap->decrease(node);
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
//    if(!keyChangeNum)return;
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    if(node) {
      node->key += keydlt;
      aoHeap->increase(node);
    }
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    if(node) {
      node->key = key;
      aoHeap->decrease(node);
    }
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    if(node) {
      node->key = key;
      aoHeap->increase(node);
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto node = aoHeap->top();
      long int key = node->key;
      NodePointer[node->nid] = NULL;
      if(aoHeap->prevalue <= key){
        aoHeap->prevalue = key;
      } else {
//        assert(0);
      }
      aoHeap->pop();
      return true;
    }
    return false;
  }
};

#endif /* AOPTester_h */
