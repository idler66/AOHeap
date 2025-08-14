//  AOHeap.h
//  Adjacent ordered heap
//  Created by 嘟嘟 on 2024/12/25.

#ifndef AOHeap_h
#define AOHeap_h

#include <cmath>
#include <algorithm>


template <typename KeyType>
struct AOHeapNode {
private:
public:
  struct AOHeapNode *bro, *pre, *next;
  KeyType key;
  int nid;
//  int index;
//  bool inBinaryHeap;
  AOHeapNode() {
    next = NULL;
    bro = NULL;
    pre = NULL;
//    inBinaryHeap = false;
  }
  void setPre(struct AOHeapNode *n) {
    pre = n;
  }
  struct AOHeapNode * getPre() {
    return pre;
  }
  void removePre() {
    if(pre) {
      if(pre->bro == this) {
        pre->removeBro();
      } else if(pre->next == this) {
        pre->removeNext();
      }
      pre = NULL;
    }
  }
  
  void moveBro2Next() {
    next = bro;
    bro = NULL;
  }
  void setNext(struct AOHeapNode *n) {
    next = n;
    if(next) {
      next->setPre(this);
    }
  }
  struct AOHeapNode* getNext() {
    return next;
  }
  void removeNext() {
    if(next) {
      next->setPre(NULL);
      next = NULL;
    }
  }
  void setBro(struct AOHeapNode *n) {
    bro = n;
    if(bro) {
      bro->setPre(this);
    }
  }
  struct AOHeapNode * getBro() {
    return bro;
  }
  void removeBro() {
    if(bro) {
      bro->pre = NULL;
      bro = NULL;
    }
  }
};

template <typename KeyType>
class MinHeap {
private:
  std::vector<AOHeapNode<KeyType> *> heap;
  static bool compare(const AOHeapNode<KeyType> * a,
                      const AOHeapNode<KeyType> * b) {
    return a->key > b->key;
  }
public:
  MinHeap() {}
  void push(AOHeapNode<KeyType> * node) {
//    node->inBinaryHeap = true;
    heap.push_back(node);
    std::push_heap(heap.begin(), heap.end(), compare);
  }
  void pop() {
    auto node = heap.front();
//    node->inBinaryHeap = false;
    std::pop_heap(heap.begin(), heap.end(), compare);
    heap.pop_back();
  }
  AOHeapNode<KeyType> * top() {
    return heap.front();
  }
  bool empty() {
    return heap.empty();
  }
  int size() {
    return heap.size();
  }
};

//static const int ConsolidateListSize = 16;
//static const int ConsolidateListSize = 32;
//static const int ConsolidateListSize = 64;
//static const int ConsolidateListSize = 10000001;
static const int ConsolidateListSize = 64+32;
static int counter = 0;
static int operation = 0;
template <typename KeyType>
class AOListWeaver {
private:
  AOHeapNode<KeyType> * waitingList[ConsolidateListSize];
  int ElementPtr;
  int ReplacePtr;

  std::vector<int> counterList;
  int bigcounter;
  
  void batchInsert(AOHeapNode<KeyType> * sentinel,
                       AOHeapNode<KeyType> * start,
                       AOHeapNode<KeyType> * end) {
    AOHeapNode<KeyType> * root = sentinel->getPre();
    end->setBro(sentinel);
    root->setNext(start);
  }
  
  void normalizeTree(AOHeapNode<KeyType> * tree,
                    MinHeap<KeyType>& groups) {//ordered and no-changing
    //convert tree to standard sub-tree and push remainders into heap
    if(tree->getBro()) {
      if(AOHeapNode<KeyType> * next = tree->getNext()) {
        tree->removeNext();
        groups.push(next);
      }
      tree->moveBro2Next();
    }
  }
  
  AOHeapNode<KeyType> * getNextMinNormalizeTree(int& waitingIndex,
                                  MinHeap<KeyType>& groups) {//ordered and no-changing
//    counter++;///////////////
    AOHeapNode<KeyType> * one = NULL;//array's min
    if(waitingIndex < ElementPtr) one = waitingList[waitingIndex];
    AOHeapNode<KeyType> * two = NULL;//heap's min
    if(!groups.empty()) {
      two = groups.top();
    }
    if(one && two) {//NULL the bigger
      if(one->key > two->key) {
        one = NULL;
      } else {
        two = NULL;
      }
    }
    if(one) {//min in array
      waitingIndex++;
      normalizeTree(one, groups);
      return one;
    }
    if(two) {//in heap
      groups.pop();
      normalizeTree(two, groups);
      return two;
    }
    return NULL;
  }
  
  void noSentinel(AOHeapNode<KeyType> * preSentinel,
                  AOHeapNode<KeyType> * start,
                  int & waitingIndex,
                  MinHeap<KeyType>& groups) {//ordered and no-changing
    if(start) {
      preSentinel->setNext(start);
      while(AOHeapNode<KeyType> * next = getNextMinNormalizeTree(waitingIndex, groups)) {
        start->setBro(next);
        start = next;
      }
    }
    ElementPtr = 1;
  }
  
  void insertionSort() {
    for (int i = 1; i < ElementPtr; i++) {
      AOHeapNode<KeyType> * node = waitingList[i];
      int j = i - 1;
      while (j >= 0 && waitingList[j]->key > node->key) {
        waitingList[j + 1] = waitingList[j];
        j--;
      }
      waitingList[j + 1] = node;
    }
  }
  
  void linearConsolidate(int & minPtr) {
    minPtr = 0;
    ReplacePtr = 0;
    insertionSort();
    MinHeap<KeyType> groups;
    
    int waitingIndex = 0;
    AOHeapNode<KeyType> * bro = getNextMinNormalizeTree(waitingIndex, groups);
    assert(bro->getBro()==NULL);
    while(bro) {
//      counter++;
      AOHeapNode<KeyType> * tree = getNextMinNormalizeTree(waitingIndex, groups);
      bro->setBro(tree);
      bro = tree;
    }
    ElementPtr = 1;
  }
  
  void consolidate(int & minPtr) {
    return linearConsolidate(minPtr);
    
    minPtr = 0;
    ReplacePtr = 0;
    insertionSort();
    MinHeap<KeyType> groups;
    int waitingIndex = 0;
    AOHeapNode<KeyType> * preSentinel = getNextMinNormalizeTree(waitingIndex, groups);
    AOHeapNode<KeyType> * sentinel = preSentinel->getNext();
    AOHeapNode<KeyType> * start = getNextMinNormalizeTree(waitingIndex, groups);
    while(start) {
      if(!sentinel) {
        //get whole bro chain, no sentinel.d
        return noSentinel(preSentinel, start, waitingIndex, groups);
      }
      //find valid sentinel.
      while(sentinel->key < start->key) {
        preSentinel = sentinel;
        sentinel = sentinel->getNext();
        if(!sentinel) {
          return noSentinel(preSentinel, start, waitingIndex, groups);
        }
      }
      //find longest bro chain and insert.
      AOHeapNode<KeyType> *end = start;
      AOHeapNode<KeyType> *next = getNextMinNormalizeTree(waitingIndex, groups);
      while(next) {
        if(sentinel->key >= next->key) {
          end->setBro(next);
          end = next;
          next = getNextMinNormalizeTree(waitingIndex, groups);
          continue;
        }
        break;
      }
      batchInsert(sentinel, start, end);
      //prepare for next loop.
      start = next;
      preSentinel = sentinel;
      sentinel = sentinel->getNext();
    }
    ElementPtr = 1;
  }
  
public:
  AOListWeaver() {
    ElementPtr = 0;
    ReplacePtr = 0;
    
    bigcounter = 0;
  }
  ~AOListWeaver() {
  }
  
  void printInfo() {
//    printf("\n H = %d", bigcounter);
//    long sum = 0;
//    std::sort(counterList.begin(), counterList.end());
//    for (int i=0; i<counterList.size(); i++) {
////      printf("= %d, ", counterList[i]);
//      sum+=counterList[i];
//    }
//    printf(", avg= %f", (float)sum/(counterList.size()));
//    counterList.clear();
  }

  bool empty() {
    return ElementPtr==0;
  }
  
  void push(AOHeapNode<KeyType> * node, int& minPtr) {
    waitingList[ElementPtr] = node;
    if(minPtr == -1) {
      minPtr = 0;
    } else if(waitingList[minPtr]->key > node->key) {
      minPtr = ElementPtr;
    }
    ElementPtr += 1;
    if(ElementPtr == ConsolidateListSize) {
      consolidate(minPtr);
    }
  }
  
  void findMin(int& minPtr) {//输出
    if(ElementPtr==0) {
      minPtr = -1;
      return;
    }
    minPtr = 0;
    int next = 1;
    while(next < ElementPtr) {
      if(waitingList[minPtr]->key > waitingList[next]->key) {
        minPtr = next;
      }
      next += 1;
    }
//    if(ReplacePtr < minPtr) {
//      std::swap(waitingList[minPtr], waitingList[ReplacePtr]);
//      minPtr = ReplacePtr;
//      ReplacePtr+=1;
//    }
  }
  
  void udpateMinPtr(AOHeapNode<KeyType>* minNode, int& minPtr) {
    minPtr = 0;
    while(waitingList[minPtr] != minNode) {
      minPtr += 1;
    }
  }
  
  AOHeapNode<KeyType> * getMin(const int& minPtr) {
    if(minPtr==-1) return NULL;
    return waitingList[minPtr];
  }
  
  AOHeapNode<KeyType> * updateMin(int& minPtr) {
    AOHeapNode<KeyType> * oldMin = waitingList[minPtr];
    AOHeapNode<KeyType> * bro = oldMin->getBro();
    if(bro) {
      oldMin->removeBro();
    }
    AOHeapNode<KeyType> * next = oldMin->getNext();
    if(next) {
      oldMin->removeNext();
    }
    if(next && bro) {
      if(bro->key > next->key) std::swap(bro, next);
      waitingList[minPtr] = bro;
      waitingList[ElementPtr++] = next;
      if(ElementPtr == ConsolidateListSize) {
        consolidate(minPtr);
        return oldMin;
      }
    }else if (next) {
      waitingList[minPtr] = next;
    } else if(bro) {
      waitingList[minPtr] = bro;
    } else {
      ElementPtr-=1;
      waitingList[minPtr] = waitingList[ElementPtr];
    }
    findMin(minPtr);
    return oldMin;
  }
  
};

template <typename KeyType>
class AOHeap {
private:
  AOListWeaver<KeyType> trees;
  int MinPtr;
  int ElementNum;
  int BIgElementNum;
  
  void batchInsert(AOHeapNode<KeyType> * sentinel,
                       AOHeapNode<KeyType> * start,
                       AOHeapNode<KeyType> * end) {
    AOHeapNode<KeyType> * root = sentinel->getPre();
    end->setBro(sentinel);
    root->setNext(start);
  }

AOHeapNode<KeyType> * pickoutSubTree(AOHeapNode<KeyType>* x) {//返回标准子树
//  AOHeapNode<KeyType> * pre = x->getPre();
//  AOHeapNode<KeyType>* bro = x->getBro();
//  if(pre->getNext() == x) {
//    x->removePre();
//    x->removeBro();
//    pre->setNext(bro);
//    return x;
//  }
  x->removePre();
//  x->removeBro();
//  pre->setBro(bro);
  return x;
}
  
public:
  
  AOHeap() {
    MinPtr = -1;
    
    
    ElementNum = 0;
    BIgElementNum = 0;
  }
  ~AOHeap() {
  }
  
  AOHeapNode<KeyType>* top() {
    return trees.getMin(MinPtr);
  }
  
  bool empty() {
    return trees.empty();
  }
  
  struct AOHeapNode<KeyType> * insert(KeyType& key, int& nid) {
//    operation++;

    struct AOHeapNode<KeyType> * node = new AOHeapNode<KeyType>();
    node->key = key;
    node->nid = nid;
    trees.push(node, MinPtr);
    
//    ElementNum++;
    if(ElementNum>BIgElementNum) BIgElementNum = ElementNum;
    return node;
  }
  
  void pop() {
//    operation++;

//    ElementNum--;
    auto minTree = trees.updateMin(MinPtr);
    free(minTree);
  }
  
  void decrease(AOHeapNode<KeyType>* node) {
//    operation++;

    if (node == trees.getMin(MinPtr)) {
      return;
    }
    if(node->getPre()) {//新树
      if(node->getPre()->key > node->key) {
        pickoutSubTree(node);
        trees.push(node, MinPtr);
      }
    } else  {//原树
      if(trees.getMin(MinPtr)->key > node->key) {
//        trees.findMin(MinPtr);
        trees.udpateMinPtr(node, MinPtr);
      }
    }
  }
  
  void increase(AOHeapNode<KeyType>* node) {
//    operation++;

    if(trees.getMin(MinPtr) == node) {
      trees.findMin(MinPtr);
    }
    AOHeapNode<KeyType> * next = node->getNext();
    AOHeapNode<KeyType> * bro = node->getBro();
    if(bro && node->key > bro->key) {
      node->removeBro();
      trees.push(bro, MinPtr);
    }
    if(next && node->key > next->key) {
      node->removeNext();
      trees.push(next, MinPtr);
    }
  }
  
  void printInfo() {
//    trees.printInfo();
    printf(" %f,", (float)counter / operation);
//    counter = 0;
//    operation = 0;
    
  }
  void toModify() {
    
  }
};

#endif /* AOHeap_h */
