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
static const bool PerMethodStatClocks = true;

template <typename KeyType>
struct AOHBoostNode{
  KeyType key;
  unsigned long nid;
  bool operator<(const AOHBoostNode& other) const {
          return key > other.key;
  }
  bool operator()(const AOHBoostNode& a, const AOHBoostNode &b) const {
    return a.key > b.key;
  }
};

template <typename KeyType>
class AOPTester {
public:
  clock_t startTime;
  
  virtual std::string name() { assert(0); return ""; };
  
  virtual void decrease(unsigned long& nid, KeyType& keydlt) {}
  virtual void increase(unsigned long& nid, KeyType& keydlt) {}
  
  virtual void insert(KeyType& key, unsigned long& nid) {}
  
  virtual HeapMarkType getHeapMarkType() { return HeapAOHType; }
  
  virtual void checkDataItem(unsigned long& nid, double& key) {}
  
  virtual bool empty() {
    return false;
  }
  
  void startStatTime() {
    if(!PerMethodStatClocks) {
      startTime = clock();
    }
  }
  void finishStatTime(clock_t& var) {
    if(!PerMethodStatClocks) {
      var += clock() - startTime;
    }
  }
    
  virtual bool popOne() {
    return false;
  }
};

template <typename KeyType>
class FibonacciTester : public AOPTester<KeyType> {

  typedef boost::heap::fibonacci_heap<AOHBoostNode<KeyType>> AOPFibonacciHeap;
  AOPFibonacciHeap fibonacciHeap;
  std::unordered_map<unsigned long, typename AOPFibonacciHeap::handle_type> handles;
public:
  using AOPTester<KeyType>::startStatTime;
  using AOPTester<KeyType>::finishStatTime;
  
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
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    auto handle = fibonacciHeap.push({key, nid});
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkInsertType, getHeapMarkType());
    }
    handles[nid] = handle;
  }
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {handles[nid].node_->value.key - keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    fibonacciHeap.decrease(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkDecreaseType, getHeapMarkType());
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {handles[nid].node_->value.key + keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    fibonacciHeap.increase(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkIncreaseType, getHeapMarkType());
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = fibonacciHeap.top();
//      handles[value.nid] = typename AOPFibonacciHeap::handle_type();
      double key = value.key;
      handles.erase(value.nid);
      clock_t start;
      if(PerMethodStatClocks) {
        start = clock();
      }
      fibonacciHeap.pop();
      if(PerMethodStatClocks) {
        SharedHeapManager().update(clock()-start, MethodMarkPopType, getHeapMarkType());
      }
      return true;
    }
    return false;
  }
    
  HeapMarkType getHeapMarkType() {
    return HeapFibonacciType;
  }
   
  void checkDataItem(unsigned long& nid, double& key) {
    nid = fibonacciHeap.top().nid;
    key = fibonacciHeap.top().key;
  }
};

template <typename KeyType>
class PairingTester : public AOPTester<KeyType> {

  typedef boost::heap::pairing_heap<AOHBoostNode<KeyType>> AOPPairingHeap;
  AOPPairingHeap pairingHeap;
  std::unordered_map<unsigned long, typename AOPPairingHeap::handle_type> handles;

public:
  using AOPTester<KeyType>::startStatTime;
  using AOPTester<KeyType>::finishStatTime;
  
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
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    auto handle = pairingHeap.push({key, nid});
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkInsertType, getHeapMarkType());
    }
    handles[nid] = handle;
  }
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {handles[nid].node_->value.key - keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    pairingHeap.decrease(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkDecreaseType, getHeapMarkType());
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {handles[nid].node_->value.key + keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    pairingHeap.increase(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkIncreaseType, getHeapMarkType());
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = pairingHeap.top();
      handles[value.nid] = typename AOPPairingHeap::handle_type();
      clock_t start;
      if(PerMethodStatClocks) {
        start = clock();
      }
      pairingHeap.pop();
      if(PerMethodStatClocks) {
        SharedHeapManager().update(clock()-start, MethodMarkPopType, getHeapMarkType());
      }
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapPairingType;
  }
  
  void checkDataItem(unsigned long& nid, double& key) {
    nid = pairingHeap.top().nid;
    key = pairingHeap.top().key;
  }
};


template <typename KeyType>
class DAryTester : public AOPTester<KeyType> {
  
  std::vector<double> seqsKey;
  std::vector<unsigned long> seqsID;
  std::vector<unsigned long> seqsType;

  typedef boost::heap::d_ary_heap<AOHBoostNode<KeyType>, boost::heap::arity<AOPDAryArity>, boost::heap::mutable_<true>, boost::heap::compare<AOHBoostNode<KeyType>>> AOPDAryHeap;
  AOPDAryHeap DAryHeap;
  std::unordered_map<unsigned long, typename AOPDAryHeap::handle_type> handles;

public:
  using AOPTester<KeyType>::startStatTime;
  using AOPTester<KeyType>::finishStatTime;
  
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
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    auto handle = DAryHeap.push({key, nid});
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkInsertType, getHeapMarkType());
    }
    handles[nid] = handle;
  }
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {(*(handles[nid])).key - keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    DAryHeap.decrease(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkDecreaseType, getHeapMarkType());
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    auto key = (*(handles[nid])).key;//nid = 8879409,6166820,
    const AOHBoostNode<KeyType> new_value = {key + keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    DAryHeap.increase(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkIncreaseType, getHeapMarkType());
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = DAryHeap.top();
      handles.erase(value.nid);//[value.nid] =typename AOPDAryHeap::handle_type();
      clock_t start;
      if(PerMethodStatClocks) {
        start = clock();
      }
      DAryHeap.pop();
      if(PerMethodStatClocks) {
        SharedHeapManager().update(clock()-start, MethodMarkPopType, getHeapMarkType());
      }
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapDAryType;
  }
   
  void checkDataItem(unsigned long& nid, double& key) {
    nid = DAryHeap.top().nid;
    key = DAryHeap.top().key;
  }
};
 

template <typename KeyType>
class BinomialTester : public AOPTester<KeyType> {

  typedef boost::heap::binomial_heap<AOHBoostNode<KeyType>> AOPBinomialHeap;
  AOPBinomialHeap binomialHeap;
  std::unordered_map<unsigned long, typename AOPBinomialHeap::handle_type> handles;

public:
  using AOPTester<KeyType>::startStatTime;
  using AOPTester<KeyType>::finishStatTime;
  
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
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    auto handle = binomialHeap.push({key, nid});
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkInsertType, getHeapMarkType());
    }
    handles[nid] = handle;
  }
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {(*(handles[nid])).key - keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    binomialHeap.decrease(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkDecreaseType, getHeapMarkType());
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    auto key = (*(handles[nid])).key;
    const AOHBoostNode<KeyType> new_value = {key + keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    binomialHeap.increase(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkIncreaseType, getHeapMarkType());
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto value = binomialHeap.top();
//      handles[value.nid] =typename AOPBinomialHeap::handle_type();
      handles.erase(value.nid);
      clock_t start;
      if(PerMethodStatClocks) {
        start = clock();
      }
      binomialHeap.pop();
      if(PerMethodStatClocks) {
        SharedHeapManager().update(clock()-start, MethodMarkPopType, getHeapMarkType());
      }
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapBinomialType;
  }
   
  void checkDataItem(unsigned long& nid, double& key) {
    nid = binomialHeap.top().nid;
    key = binomialHeap.top().key;
  }
};


template <typename KeyType>
class SkewTester : public AOPTester<KeyType> {

  typedef boost::heap::skew_heap<AOHBoostNode<KeyType>, boost::heap::mutable_<true>> AOPSkewHeap;
  AOPSkewHeap skewHeap;
  std::unordered_map<unsigned long, typename AOPSkewHeap::handle_type> handles;

public:
  using AOPTester<KeyType>::startStatTime;
  using AOPTester<KeyType>::finishStatTime;
  
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
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    auto handle = skewHeap.push({key, nid});
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkInsertType, getHeapMarkType());
    }
    handles[nid] = handle;
  }
  
  bool elementExist(unsigned long& nid) {
    return handles.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    AOHBoostNode<KeyType> new_value = {(*(handles[nid])).key - keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    skewHeap.decrease(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkDecreaseType, getHeapMarkType());
    }
  }
  void increase(unsigned long& nid, KeyType& keydlt) {
    auto key = (*(handles[nid])).key;
    const AOHBoostNode<KeyType> new_value = {key + keydlt, nid};
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    skewHeap.increase(handles[nid], new_value);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkIncreaseType, getHeapMarkType());
    }
  }
    
  bool popOne() {
    if(!empty()) {
      auto value = skewHeap.top();
//      handles[value.nid] =typename AOPSkewHeap::handle_type();
      handles.erase(value.nid);
      clock_t start;
      if(PerMethodStatClocks) {
        start = clock();
      }
      skewHeap.pop();
      if(PerMethodStatClocks) {
        SharedHeapManager().update(clock()-start, MethodMarkPopType, getHeapMarkType());
      }
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapSkewType;
  }

  void checkDataItem(unsigned long& nid, double& key) {
    nid = skewHeap.top().nid;
    key = skewHeap.top().key;
  }
};

template <typename KeyType>
class AOHeapTester : public AOPTester<KeyType> {
  
  AOHeap<KeyType> * aoHeap;
  std::unordered_map<unsigned long, AOHeapNode<KeyType>*> NodePointer;
public:
  
  using AOPTester<KeyType>::startStatTime;
  using AOPTester<KeyType>::finishStatTime;
  
  std::string name() {
    return "AOH";
  }
  ~AOHeapTester() {
    while(popOne()) {
      assert(0);
    }
  }
  AOHeapTester() {
    aoHeap = new AOHeap<KeyType>();
  }
  bool empty() {
    return aoHeap->empty();
  }
  auto top() {
    return aoHeap->top();
  }
  void insert(KeyType& key, unsigned long& nid) {
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    auto handle = aoHeap->insert(key, nid);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkInsertType, getHeapMarkType());
    }
    NodePointer[nid] = handle;
  }
  
  bool elementExist(unsigned long& nid) {
    return NodePointer.count(nid) != 0;
  }
  
  void decrease(unsigned long& nid, KeyType& keydlt) {
    if(keydlt == 0) return;
    AOHeapNode<KeyType>* node = NodePointer[nid];
    node->key -= keydlt;
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    aoHeap->decrease(node);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkDecreaseType, getHeapMarkType());
    }
  }
  
  void increase(unsigned long& nid, KeyType& keydlt) {
    AOHeapNode<KeyType>* node = NodePointer[nid];
    node->key += keydlt;
    clock_t start;
    if(PerMethodStatClocks) {
      start = clock();
    }
    aoHeap->increase(node);
    if(PerMethodStatClocks) {
      SharedHeapManager().update(clock()-start, MethodMarkIncreaseType, getHeapMarkType());
    }
  }
  
  bool popOne() {
    if(!empty()) {
      auto node = aoHeap->top();
      NodePointer.erase(node->nid);
      clock_t start;
      if(PerMethodStatClocks) {
        start = clock();
      }
      aoHeap->pop();
      if(PerMethodStatClocks) {
        SharedHeapManager().update(clock()-start, MethodMarkPopType, getHeapMarkType());
      }
      return true;
    }
    return false;
  }
  
  HeapMarkType getHeapMarkType() {
    return HeapAOHType;
  }
  void checkDataItem(unsigned long& nid, double& key) {
    nid = aoHeap->top()->nid;
    key = aoHeap->top()->key;
  }
};

#endif /* AOPTester_h */
