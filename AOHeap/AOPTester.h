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

static long int  AOPNodeNum = 1000000;
static long int  AOPKeyChangeNum = 10;

static const long int  AOPBatchInsSize = 10;
static const long int  AOPBandWidth = 1000;

static const long int  AOPDAryArity = 10;
static const long int  AOPRandSeed = 10;
static const bool AOPStatClocks = true;

enum AOPHeapKeyType : unsigned char {
  AOPHeapKeyRand = 1,
  AOPHeapKeyInc = 2,
  AOPHeapKeyDec = 4,
  AOPHeapKeyBandInc = 8,
};

template <typename KeyType, typename DataType>
struct AOHBoostNode{
  KeyType key;
  long int nid;
  DataType data;
  bool operator<(const AOHBoostNode& other) const {
          return key > other.key;
  }
};

template <typename KeyType, typename DataType>
class AOPTester {
public:
  long int popedCounter;
  long int nodeNum;
  long int keyChangeNum;
  unsigned int randSeed;
  AOPHeapKeyType keyType;
  clock_t insertTime;
  clock_t popTime;
  clock_t increaseTime;
  clock_t decreaseTime;
  
  long int bandWidth;
  
  clock_t startTime;
  
  void init(unsigned long nc, int inlarge, AOPHeapKeyType type, int seed, long int width) {
    popedCounter = 0;
    keyType = type;
    nodeNum = nc;
    keyChangeNum = inlarge;
    randSeed = seed;
    insertTime = 0;
    popTime = 0;
    increaseTime = 0;
    decreaseTime = 0;
    bandWidth = width;
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
    
  long int getKey(long int& nid) {
    long int key;
    if(keyType == AOPHeapKeyRand) {
      key = nodeNum*(rand()%(nodeNum)/(float)(nodeNum));
    } else if(keyType == AOPHeapKeyInc) {
      key =  nid;
    } else if(keyType == AOPHeapKeyDec) {
      key = nodeNum - nid;
    } else {
      key = nid + bandWidth*(rand()%(bandWidth)/(float)(bandWidth));
    }
    return key;
  }
  
  virtual void nodeAssert() {
    assert(popedCounter == nodeNum);
  }
  
  void batchInsert(long int& nid) {
    for(int i=0; i< AOPBatchInsSize && nid<nodeNum; i++) {
      long int key = getKey(nid);
      insert(key, nid);
      nid++;
    }
  }
  void insertAll() {
    for(long int nid=0; nid<nodeNum; nid++){
      long int key = getKey(nid);
      insert(key, nid);
    }
  }
  virtual void insert(KeyType& key, long int& nid) {}
  
  void batchDecrease(long int& counter) {
    for(int i=0; i< keyChangeNum && counter++<nodeNum*keyChangeNum; i++) {
      long int nid = rand()%(nodeNum);
      long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
      if (keydlt>0) {
        decrease(nid, keydlt);
      }
    }
  }
  void decreaseAll() {
    for (unsigned long counter =0; counter<nodeNum*keyChangeNum ; counter++) {
      long int nid = rand()%(nodeNum);
      long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
      if (keydlt>0) {
        decrease(nid, keydlt);
      }
    }
  }
  virtual void decrease(long int& nid, KeyType& keydlt) {}
  
  void batchIncrease(long int& counter) {
    for(int i=0; i< keyChangeNum && counter++<nodeNum*keyChangeNum; i++) {
      long int nid = rand()%(nodeNum);
      long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
      assert(keydlt>=0);
      if (keydlt>0) {
        increase(nid, keydlt);
      }
    }
  }
  void increaseAll() {
    for (unsigned long counter =0; counter<nodeNum*keyChangeNum; counter++) {
      long int nid = rand()%(nodeNum);
      long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
      if (keydlt>0) {
        increase(nid, keydlt);
      }
    }
  }
  virtual void increase(long int& nid, long int& keydlt) { }
  
  void insertIntertwinedPop() {
    long int nid = 0;
    do {
      batchInsert(nid);
      popOne();
    }while(!empty());
    nodeAssert();
  }
  
  void increaseIntertwinedPop() {
    long int changeNum = 0;
    while(!empty()) {
      for (unsigned long counter =0; counter<keyChangeNum && changeNum++ < keyChangeNum*nodeNum; counter++) {
        long int nid = rand()%(nodeNum);
        long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
        if (keydlt>0) {
          increase(nid, keydlt);
        }
      }
      popOne();
    }
    nodeAssert();
  }
  
  void decreaseIntertwinedPop() {
    long int changeNum = 0;
    while(!empty()) {
      for (unsigned long counter =0; counter<keyChangeNum && changeNum++ < keyChangeNum*nodeNum; counter++) {
        long int nid = rand()%(nodeNum);
        long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
        if (keydlt>0) {
          decrease(nid, keydlt);
        }
      }
      popOne();
    }
    nodeAssert();
  }
  
  void decIntertwinedWithIncAndPop() {
    long int changeNum = 0;
    while(!empty()) {
      for (unsigned long counter =0; counter<nodeNum && changeNum++ < keyChangeNum*nodeNum; counter++) {
        long int nid = rand()%(nodeNum);
        long int keydlt = nodeNum*(rand()%(nodeNum)/(float)(nodeNum))/15;
        if (keydlt>0) {
          if(keydlt % 5 ==0) {
            decrease(nid, keydlt);
          } else {
            increase(nid, keydlt);
          }
        }
      }
      popOne();
    }
    nodeAssert();
  }
  
  void insertIntertwinedWithIncAndPop() {//最差情况
    long int nid = 0;
    long int counter = 0;
    do {
      batchInsert(nid);
      batchIncrease(counter);
      popOne();
    }while(!empty());
    nodeAssert();
  }
  
  void insertAndDecIntertwinedWithIncAndPop() {
    long int nid = 0;
    long int incc = 0;
    long int decc = 0;
    do {
      batchInsert(nid);
      batchDecrease(decc);
      batchIncrease(incc);
      popOne();
    }while(!empty());
    nodeAssert();
  }
  
  void popAll() {
    while(popOne()) {}
    nodeAssert();
  }
  virtual bool popOne() {
    return false;
  }
};

template <typename KeyType, typename DataType>
class FibonacciTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::fibonacci_heap<AOHBoostNode<KeyType, DataType>> AOPFibonacciHeap;
  AOPFibonacciHeap fibonacciHeap;
  std::vector<typename AOPFibonacciHeap::handle_type> handles;
public:
  FibonacciTester(unsigned long nc, int inlarge,
                  AOPHeapKeyType type,
                  int seed, long int bandWidth) {
    init(nc, inlarge, type, seed, bandWidth);
    srand(seed);
    handles.resize(nc);
  }
  bool empty() {
    return fibonacciHeap.empty();
  }
  auto top() {
    return fibonacciHeap.top();
  }
  void insert(long int& key, long int& nid) {
    startStatTime();
    handles[nid] = fibonacciHeap.push({key, nid});
    finishStatTime(insertTime);
  }
  void decrease(long int& nid, long int& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPFibonacciHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key - keydlt, nid};
      fibonacciHeap.decrease(handles[nid], new_value);
    }
   finishStatTime(decreaseTime);
  }
  void increase(long int& nid, long int& keydlt) {
    AOPTester<KeyType, DataType>::startStatTime();
    if(handles[nid]!=typename AOPFibonacciHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key + keydlt, nid};
      fibonacciHeap.increase(handles[nid], new_value);
    }
    AOPTester<KeyType, DataType>::finishStatTime(increaseTime);
  }
  bool popOne() {
    AOPTester<KeyType, DataType>::startStatTime();
    if(!empty()) {
      auto value = fibonacciHeap.top();
      handles[value.nid] = typename AOPFibonacciHeap::handle_type();
      fibonacciHeap.pop();
      AOPTester<KeyType, DataType>::finishStatTime(popTime);
      popedCounter++;
      return true;
    }
    return false;
  }
};

template <typename KeyType, typename DataType>
class PairingTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::pairing_heap<AOHBoostNode<KeyType, DataType>> AOPPairingHeap;
  AOPPairingHeap pairingHeap;
  std::vector<typename AOPPairingHeap::handle_type> handles;

public:
  PairingTester(unsigned long nc, int inlarge, AOPHeapKeyType type, int seed, long int bandWidth) {
    init(nc, inlarge, type, seed, bandWidth);
    srand(seed);
    handles.resize(nc);
  }
  bool empty() {
    return pairingHeap.empty();
  }
  auto top() {
    return pairingHeap.top();
  }
  void insert(KeyType& key, long int& nid) {
    startStatTime();
    handles[nid] = pairingHeap.push({key, nid});
    finishStatTime(insertTime);
  }
  void decrease(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPPairingHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key - keydlt, nid};
      pairingHeap.decrease(handles[nid], new_value);
    }
    finishStatTime(decreaseTime);
  }
  void increase(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPPairingHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {handles[nid].node_->value.key + keydlt, nid};
      pairingHeap.increase(handles[nid], new_value);
    }
    finishStatTime(increaseTime);
  }
  bool popOne() {
    startStatTime();
    if(!empty()) {
      auto value = pairingHeap.top();
      handles[value.nid] = typename AOPPairingHeap::handle_type();
      pairingHeap.pop();
      finishStatTime(popTime);
      popedCounter++;
      return true;
    }
    return false;
  }
};


template <typename KeyType, typename DataType>
class DAryTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::d_ary_heap<AOHBoostNode<KeyType, DataType>, boost::heap::arity<AOPDAryArity>, boost::heap::mutable_<true>> AOPDAryHeap;
  AOPDAryHeap DAryHeap;
  std::vector<typename AOPDAryHeap::handle_type> handles;

public:
  DAryTester(unsigned long nc, int inlarge, AOPHeapKeyType type, int seed, long int bandWidth) {
    init(nc, inlarge, type, seed, bandWidth);
    srand(seed);
    handles.resize(nc);
  }
  bool empty() {
    return DAryHeap.empty();
  }
  auto top() {
    return DAryHeap.top();
  }
  void insert(KeyType& key, long int& nid) {
    startStatTime();
    handles[nid] = DAryHeap.push({key, nid});
    finishStatTime(insertTime);
  }
  void decrease(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPDAryHeap::handle_type()) {
      assert((*(handles[nid])).nid==nid);
      AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
      DAryHeap.decrease(handles[nid], new_value);
    }
    finishStatTime(decreaseTime);
  }
  void increase(long int& nid, long int& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPDAryHeap::handle_type()) {
      auto key = (*(handles[nid])).key;
      const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
      DAryHeap.increase(handles[nid], new_value);
//      fh.update(handles[nid], new_value);
    }
    finishStatTime(increaseTime);
  }
  bool popOne() {
    startStatTime();
    if(!empty()) {
      auto value = DAryHeap.top();
      handles[value.nid] =typename AOPDAryHeap::handle_type();
      DAryHeap.pop();
      finishStatTime(popTime);
      popedCounter++;
      return true;
    }
    return false;
  }
};
 

template <typename KeyType, typename DataType>
class BinomialTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::binomial_heap<AOHBoostNode<KeyType, DataType>> AOPBinomialHeap;
  AOPBinomialHeap binomialHeap;
  std::vector<typename AOPBinomialHeap::handle_type> handles;

public:
  BinomialTester(unsigned long nc, int inlarge, AOPHeapKeyType type, int seed, long int bandWidth) {
    init(nc, inlarge, type, seed, bandWidth);
    srand(seed);
    handles.resize(nc);
  }
  bool empty() {
    return binomialHeap.empty();
  }
  auto top() {
    return binomialHeap.top();
  }
  void insert(KeyType& key, long int& nid) {
    startStatTime();
    handles[nid] = binomialHeap.push({key, nid});
    finishStatTime(insertTime);
  }
  void decrease(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPBinomialHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
      binomialHeap.decrease(handles[nid], new_value);
    }
    finishStatTime(decreaseTime);
  }
  void increase(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPBinomialHeap::handle_type()) {
      auto key = (*(handles[nid])).key;
      const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
      binomialHeap.increase(handles[nid], new_value);
//      fh.update(handles[nid], new_value);
    }
    finishStatTime(increaseTime);
  }
  bool popOne() {
    startStatTime();
    if(!empty()) {
      auto value = binomialHeap.top();
      handles[value.nid] =typename AOPBinomialHeap::handle_type();
      binomialHeap.pop();
      finishStatTime(popTime);
      popedCounter++;
      return true;
    }
    return false;
  }
};


template <typename KeyType, typename DataType>
class SkewTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::init;

  typedef boost::heap::skew_heap<AOHBoostNode<KeyType, DataType>, boost::heap::mutable_<true>> AOPSkewHeap;
  AOPSkewHeap skewHeap;
  std::vector<typename AOPSkewHeap::handle_type> handles;

public:
  SkewTester(unsigned long nc, int inlarge,
             AOPHeapKeyType type,
             int seed, long int bandWidth) {
    init(nc, inlarge, type, seed, bandWidth);
    srand(seed);
    handles.resize(nc);
  }
  bool empty() {
    return skewHeap.empty();
  }
  auto top() {
    return skewHeap.top();
  }
  void insert(KeyType& key, long int& nid) {
    startStatTime();
    handles[nid] = skewHeap.push({key, nid});
    finishStatTime(insertTime);
  }
  void decrease(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPSkewHeap::handle_type()) {
      AOHBoostNode<KeyType, DataType> new_value = {(*(handles[nid])).key - keydlt, nid};
      skewHeap.decrease(handles[nid], new_value);
    }
    finishStatTime(decreaseTime);
  }
  void increase(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(handles[nid]!=typename AOPSkewHeap::handle_type()) {
      auto key = (*(handles[nid])).key;
      const AOHBoostNode<KeyType, DataType> new_value = {key + keydlt, nid};
      skewHeap.increase(handles[nid], new_value);
//      fh.update(handles[nid], new_value);
    }
    finishStatTime(increaseTime);
  }
  bool popOne() {
    startStatTime();
    if(!empty()) {
      auto value = skewHeap.top();
      handles[value.nid] =typename AOPSkewHeap::handle_type();
      skewHeap.pop();
      finishStatTime(popTime);
      popedCounter++;
      return true;
    }
    return false;
  }
};

template <typename KeyType, typename DataType>
class AOHeapTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::keyChangeNum;
  using AOPTester<KeyType, DataType>::init;
 
  AOHeap<KeyType, DataType> * aoHeap;
  std::vector<AOHeapNode<KeyType, DataType>*> NodePointer;
public:
  
  AOHeapTester(unsigned long nc, int inlarge,
               AOPHeapKeyType type,
               int seed, long int width) {
    NodePointer.resize(nc);
    init(nc, inlarge, type, seed, width);
    srand(seed);
    aoHeap = new AOHeap<KeyType, DataType>(AOPNodeNum);
  }
  bool empty() {
    return aoHeap->empty();
  }
  auto top() {
    return aoHeap->top();
  }
  void insert(KeyType& key, long int& nid) {
    startStatTime();
    NodePointer[nid] = aoHeap->insert(key, nid);
    finishStatTime(insertTime);
  }
  
  void decrease(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(!keyChangeNum)return;
    if(keydlt == 0) return;
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    if(node) {
      node->key -= keydlt;
      aoHeap->decrease(node);
      finishStatTime(decreaseTime);
    }
  }
  void increase(long int& nid, KeyType& keydlt) {
    startStatTime();
    if(!keyChangeNum)return;
    AOHeapNode<KeyType, DataType>* node = NodePointer[nid];
    if(node) {
      node->key += keydlt;
      aoHeap->increase(node);
      finishStatTime(increaseTime);
    }
  }
  bool popOne() {
    startStatTime();
    if(!empty()) {
      auto node = aoHeap->top();
      long int key = node->key;
      NodePointer[node->nid] = NULL;
      if(aoHeap->prevalue <= key){
        aoHeap->prevalue = key;
      } else {
        assert(0);
      }
      aoHeap->pop();
      finishStatTime(popTime);
      popedCounter++;
      return true;
    }
    return false;
  }
};


template <typename KeyType, typename DataType>
class BinaryHeapTester : public AOPTester<KeyType, DataType> {
  using AOPTester<KeyType, DataType>::insertTime;
  using AOPTester<KeyType, DataType>::decreaseTime;
  using AOPTester<KeyType, DataType>::increaseTime;
  using AOPTester<KeyType, DataType>::popTime;
  
  using AOPTester<KeyType, DataType>::startStatTime;
  using AOPTester<KeyType, DataType>::finishStatTime;
  
  using AOPTester<KeyType, DataType>::popedCounter;
  using AOPTester<KeyType, DataType>::keyChangeNum;
  using AOPTester<KeyType, DataType>::init;
 
  std::vector<struct AOHBoostNode<KeyType, DataType>> frontier;
public:
  
  BinaryHeapTester(unsigned long nc, int inlarge,
                   AOPHeapKeyType type,
                   int seed, long int bandWidth) {
    init(nc, inlarge, type, seed, bandWidth);
    srand(seed);
  }
  bool empty() {
    return frontier.empty();
  }
  auto top() {
    return frontier.back();
  }
  void insert(KeyType& key, long int& nid) {
    startStatTime();
    struct AOHBoostNode<KeyType, DataType> node;
    node.key = key;
    node.nid = nid;
    frontier.push_back(node);
    auto comparator = [&] (struct AOHBoostNode<KeyType, DataType>& left,
                           struct AOHBoostNode<KeyType, DataType>& right) {
        return left.key > right.key;
    };
    push_heap(begin(frontier), end(frontier), comparator);
    finishStatTime(insertTime);
  }
  
  void decrease(long int& nid, KeyType& key) {
    startStatTime();
    struct AOHBoostNode<KeyType, DataType> node;
    node.key = key;
    node.nid = nid;
    frontier.push_back(node);
    auto comparator = [&] (struct AOHBoostNode<KeyType, DataType>& left,
                           struct AOHBoostNode<KeyType, DataType>& right) {
        return left.key > right.key;
    };
    push_heap(begin(frontier), end(frontier), comparator);
    finishStatTime(decreaseTime);
  }
  void increase(long int& nid, KeyType& key) {
    startStatTime();
    struct AOHBoostNode<KeyType, DataType> node;
    node.key = key;
    node.nid = nid;
    frontier.push_back(node);
    auto comparator = [&] (struct AOHBoostNode<KeyType, DataType>& left,
                           struct AOHBoostNode<KeyType, DataType>& right) {
        return left.key > right.key;
    };
    push_heap(begin(frontier), end(frontier), comparator);
    finishStatTime(increaseTime);
  }
  bool popOne() {
    startStatTime();
    if(!empty()) {
      auto comparator = [&] (struct AOHBoostNode<KeyType, DataType>& left,
                             struct AOHBoostNode<KeyType, DataType>& right) {
          return left.key > right.key;
      };
      pop_heap(begin(frontier), end(frontier), comparator);
      auto back = frontier.back();
      frontier.pop_back();
      popedCounter++;
      finishStatTime(popTime);
      return true;
    }
    return false;
  }
  void nodeAssert() {
    
  }
};

#endif /* AOPTester_h */
