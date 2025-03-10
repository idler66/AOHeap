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
#include "AOPSeqsGenerator.h"

#include "CommonDef.h"

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
  clock_t startTime;
  
  virtual std::string name() { assert(0); return ""; };
  
  virtual void decrease(unsigned long& nid, KeyType& keydlt) {}
  virtual void increase(unsigned long& nid, KeyType& keydlt) {}
  
  virtual void decreaseWithKey(unsigned long& nid, KeyType& key) {}
  virtual void increaseWithKey(unsigned long& nid, KeyType& key) {}

  virtual void insert(KeyType& key, unsigned long& nid) {}
  
  virtual HeapMarkType getHeapMarkType() { return HeapAOHType; }
  
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

  typedef boost::heap::fibonacci_heap<AOHBoostNode<KeyType, DataType>> AOPFibonacciHeap;
  AOPFibonacciHeap fibonacciHeap;
  std::unordered_map<unsigned long, typename AOPFibonacciHeap::handle_type> handles;
public:
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  std::string name() {
    return "Fibonacci";
  }
  
  FibonacciTester() {
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
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    printf("\n == decrease %ld   %f ==", nid, handles[nid].node_->value.key - keydlt);
    AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key - keydlt, nid};
    fibonacciHeap.decrease(handles[nid], new_value);
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    printf("\n == increase %ld   %f ==", nid, handles[nid].node_->value.key + keydlt);
    AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key + keydlt, nid};
    fibonacciHeap.increase(handles[nid], new_value);
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    fibonacciHeap.decrease(handles[nid], new_value);
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    fibonacciHeap.increase(handles[nid], new_value);
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = fibonacciHeap.top();
//      handles[value.nid] = typename AOPFibonacciHeap::handle_type();
      handles.erase(value.nid);
      fibonacciHeap.pop();
      assert(!elementExist(value.nid));
      printf("\n == fibonacci pop %ld , %f ==", value.nid, value.key);
      return true;
    }
    return false;
  }
    
  HeapMarkType getHeapMarkType() {
    return HeapFibonacciType;
  }
};

template <typename KeyType, typename DataType>
class PairingTester : public AOPTester<KeyType, DataType> {

  typedef boost::heap::pairing_heap<AOHBoostNode<KeyType, DataType>> AOPPairingHeap;
  AOPPairingHeap pairingHeap;
  std::unordered_map<unsigned long, typename AOPPairingHeap::handle_type> handles;

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
  PairingTester() {
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
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    printf("\n pairing decrease  %ld , %f", nid, (*(handles[nid])).key - keydlt);
    AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key - keydlt, nid};
    pairingHeap.decrease(handles[nid], new_value);
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    printf("\n pairing increase  %ld , %f", nid, (*(handles[nid])).key + keydlt);
    AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key + keydlt, nid};
    pairingHeap.increase(handles[nid], new_value);
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    pairingHeap.decrease(handles[nid], new_value);
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    pairingHeap.increase(handles[nid], new_value);
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = pairingHeap.top();
      handles[value.nid] = typename AOPPairingHeap::handle_type();
      pairingHeap.pop();
      printf("\n pairingHeap pop %ld , %f", value.nid, value.key);
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapPairingType;
  }
};


template <typename KeyType, typename DataType>
class DAryTester : public AOPTester<KeyType, DataType> {

  typedef boost::heap::d_ary_heap<AOHBoostNode<KeyType, DataType>, boost::heap::arity<AOPDAryArity>, boost::heap::mutable_<true>> AOPDAryHeap;
  AOPDAryHeap DAryHeap;
  std::unordered_map<unsigned long, typename AOPDAryHeap::handle_type> handles;

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
  DAryTester() {
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
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
    DAryHeap.decrease(handles[nid], new_value);
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    printf("\n DAry increase  %ld , %f", nid, (*(handles[nid])).key + keydlt);

    auto key = (*(handles[nid])).key;
    const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
    DAryHeap.increase(handles[nid], new_value);
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    DAryHeap.decrease(handles[nid], new_value);
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    DAryHeap.increase(handles[nid], new_value);
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = DAryHeap.top();
      handles.erase(value.nid);//[value.nid] =typename AOPDAryHeap::handle_type();
      printf("\n DAry pop %ld , %f", value.nid, value.key);
      DAryHeap.pop();
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapDAryType;
  }
};
 

template <typename KeyType, typename DataType>
class BinomialTester : public AOPTester<KeyType, DataType> {

  typedef boost::heap::binomial_heap<AOHBoostNode<KeyType, DataType>> AOPBinomialHeap;
  AOPBinomialHeap binomialHeap;
  std::unordered_map<unsigned long, typename AOPBinomialHeap::handle_type> handles;

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
  BinomialTester() {
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
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
    binomialHeap.decrease(handles[nid], new_value);
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    auto key = (*(handles[nid])).key;
    const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
    binomialHeap.increase(handles[nid], new_value);
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    binomialHeap.decrease(handles[nid], new_value);
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    binomialHeap.increase(handles[nid], new_value);
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = binomialHeap.top();
//      handles[value.nid] =typename AOPBinomialHeap::handle_type();
      handles.erase(value.nid);
//      if(value.nid==6)
      printf("\n binomial pop %ld ,%f", value.nid, value.key);
      binomialHeap.pop();
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapBinomialType;
  }
};


template <typename KeyType, typename DataType>
class SkewTester : public AOPTester<KeyType, DataType> {

  typedef boost::heap::skew_heap<AOHBoostNode<KeyType, DataType>, boost::heap::mutable_<true>> AOPSkewHeap;
  AOPSkewHeap skewHeap;
  std::unordered_map<unsigned long, typename AOPSkewHeap::handle_type> handles;

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
  SkewTester() {
  }
  bool empty() {
    return skewHeap.empty();
  }
  auto top() {
    return skewHeap.top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    printf("\n== skew insert %ld==", nid);
    handles[nid] = skewHeap.push({key, nid});
  }
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    printf("\n== skew decrease  %ld, dlt %f==", nid, keydlt);
    assert(elementExist(nid));
    AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
    skewHeap.decrease(handles[nid], new_value);
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    printf("\n== skew increase  %ld, dlt %f==", nid, keydlt);
    assert(elementExist(nid));
    auto key = (*(handles[nid])).key;
    const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
    skewHeap.increase(handles[nid], new_value);
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    printf("\n== skew decrease  %ld, key %f==", nid, key);
    assert(elementExist(nid));
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    skewHeap.decrease(handles[nid], new_value);
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    printf("\n==skew increase  %ld, key %f==", nid, key);
    assert(elementExist(nid));
    AOHBoostNode<KeyType, DataType> new_value = {key, nid};
    skewHeap.increase(handles[nid], new_value);
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = skewHeap.top();
//      handles[value.nid] =typename AOPSkewHeap::handle_type();
      handles.erase(value.nid);
      printf("\n skew pop %ld", value.nid);
      skewHeap.pop();
      assert(!elementExist(value.nid));
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapSkewType;
  }
};

template <typename KeyType, typename DataType>
class AOHeapTester : public AOPTester<KeyType, DataType> {
  
  AOHeap<KeyType, DataType> * aoHeap;
  std::unordered_map<unsigned long, AOHeapNode<KeyType, DataType>*> NodePointer;
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
  AOHeapTester() {
    aoHeap = new AOHeap<KeyType, DataType>();
  }
  bool empty() {
    return aoHeap->empty();
  }
  auto top() {
    return aoHeap->top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    printf("\n== aop insert  %ld, key %f==", nid, key);
    NodePointer[nid] = aoHeap->insert(key, nid);
  }
  
  bool elementExist(unsigned long& nid) {
    return NodePointer.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    printf("\n== aop decrease  %ld, keydlt %f==", nid, keydlt);
//    if(!keyChangeNum)return;
    assert(elementExist(nid));
    if(keydlt == 0) return;
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    node->key -= keydlt;
    aoHeap->decrease(node);
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
//    if(!keyChangeNum)return;
    printf("\n== aop increase  %ld, keydlt %f==", nid, keydlt);
    assert(elementExist(nid));
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    node->key += keydlt;
    aoHeap->increase(node);
  }
  
  void decreaseWithKey(unsigned long& nid, KeyType& key) {
    printf("\n== aop decrease  %ld, key %f==", nid, key);
    assert(elementExist(nid));
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    node->key = key;
    aoHeap->decrease(node);
  }
  void increaseWithKey(unsigned long& nid, KeyType& key) {
    printf("\n== aop increase  %ld, key %f==", nid, key);
    assert(elementExist(nid));
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    node->key = key;
    aoHeap->increase(node);
  }
  
  bool popOne() {
    if(!empty()) {
      auto node = aoHeap->top();
//      long int key = node->key;
//      NodePointer[node->nid] = NULL;
      printf("\n== aop pop  %ld", node->nid);
      NodePointer.erase(node->nid);
      aoHeap->pop();
      assert(!elementExist(node->nid));
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapAOHType;
  }
};

#endif /* AOPTester_h */
