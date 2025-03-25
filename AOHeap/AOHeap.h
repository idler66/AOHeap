//  AOHeap.h
//  Adjacent ordered heap
//  Created by 嘟嘟 on 2024/12/25.

#ifndef AOHeap_h
#define AOHeap_h

#include <cmath>
#include <algorithm>

static const int AOHReshapeCount = 32;
static const int AOHSubGroupSize = 64;
static const bool AOHStat = true;

template <typename KeyType>
struct AOHeapNode {
private:
public:
  struct AOHeapNode *bro, *pre, *next;
  KeyType key;
  int nid;
  AOHeapNode() {
    next = NULL;
    bro = NULL;
    pre = NULL;
  }
  void setPre(struct AOHeapNode *n) {
    if(n==this) {
      printf("sdfsf");
    }
    pre = n;
    if(bro && n && bro ==n) {
      printf("sdfsf");
    }
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
  
  void swapBroAndNext() {
    std::swap(bro, next);
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
    if(next==this) {
      printf("sdfsf");
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
    if(bro == this) {
      printf("sdfsf");
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
    heap.push_back(node);
    std::push_heap(heap.begin(), heap.end(), compare);
  }
  void pop() {
    std::pop_heap(heap.begin(), heap.end(), compare);
    heap.pop_back();
  }
  AOHeapNode<KeyType> * top() {
    return heap.front();
  }
  bool empty() {
    return heap.empty();
  }
};

static const int ConsolidateListSize = 64;
template <typename KeyType>
class AOListWeaver {
private:
  AOHeapNode<KeyType> * waitingList[ConsolidateListSize];
  int ElementPtr;
  
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
  
  void reshape(AOHeapNode<KeyType>* root) {
    AOHeapNode<KeyType> * visitor = root;
    while(visitor) {
      visitor->swapBroAndNext();
      visitor = visitor->getBro();
    }
  }
  
  void checkNoDouble(int index) {
    for(int ptr=0; ptr<ElementPtr; ptr++) {
      if(ptr!=index)
        assert(waitingList[ptr]!=waitingList[index]);
    }
    for(int ptr=0; ptr<ElementPtr; ptr++) {
      assert(waitingList[ptr]->getPre()==NULL);
    }
    
  }
  
  void consolidate(int & minPtr) {
    minPtr = 0;
    assert(ElementPtr == ConsolidateListSize);
    //ordering the array
    insertionSort();
    MinHeap<KeyType> groups;
    int waitingIndex = 0;
    AOHeapNode<KeyType> * preSentinel = getNextMinNormalizeTree(waitingIndex, groups);
    AOHeapNode<KeyType> * sentinel = preSentinel->getNext();
    AOHeapNode<KeyType> * start = getNextMinNormalizeTree(waitingIndex, groups);
    while(start) {
      if(!sentinel) {
        //get whole bro chain, no sentinel.
        return noSentinel(preSentinel, start, waitingIndex, groups);
      }
      //find valid sentinel.
      while(sentinel->key < start->key) {
        preSentinel = sentinel;
        sentinel = sentinel->getNext();
        if(!sentinel) return noSentinel(preSentinel, start, waitingIndex, groups);
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
    checkNoDouble(ElementPtr);
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
  }
  
  AOHeapNode<KeyType> * getMin(const int& minPtr) {
    if(minPtr==-1) return NULL;
    return waitingList[minPtr];
  }
  
  AOHeapNode<KeyType> * updateMin(int& minPtr) {
    AOHeapNode<KeyType> * oldMin = waitingList[minPtr];
    AOHeapNode<KeyType> * bro = oldMin->getBro();
    if(bro) oldMin->removeBro();
    AOHeapNode<KeyType> * next = oldMin->getNext();
    if(next) oldMin->removeNext();
    if(next && bro) {
      if(bro->key > next->key) std::swap(bro, next);
      waitingList[minPtr] = bro;
      checkNoDouble(minPtr);

      waitingList[ElementPtr++] = next;
      if(ElementPtr == ConsolidateListSize) {
        consolidate(minPtr);
        return oldMin;
      }
    }else if (next) {
      waitingList[minPtr] = next;
      checkNoDouble(minPtr);
    } else if(bro) {
      waitingList[minPtr] = bro;
      checkNoDouble(minPtr);
    } else {
      ElementPtr-=1;
      waitingList[minPtr] = waitingList[ElementPtr];
      checkNoDouble(minPtr);
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
  
  void batchInsert(AOHeapNode<KeyType> * sentinel,
                       AOHeapNode<KeyType> * start,
                       AOHeapNode<KeyType> * end) {
    AOHeapNode<KeyType> * root = sentinel->getPre();
    end->setBro(sentinel);
    root->setNext(start);
  }

AOHeapNode<KeyType> * pickoutSubTree(AOHeapNode<KeyType>* x) {//返回标准子树
  AOHeapNode<KeyType> * pre = x->getPre();
  AOHeapNode<KeyType>* bro = x->getBro();
  if(pre->getNext() == x) {
    x->removePre();
    x->removeBro();
    pre->setNext(bro);
    return x;
  }
  x->removePre();
  x->removeBro();
  pre->setBro(bro);
  return x;
}
  
public:
  
  AOHeap() {
    MinPtr = -1;
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
    struct AOHeapNode<KeyType> * node = new AOHeapNode<KeyType>();
    node->key = key;
    node->nid = nid;

    int checkMinPtr;

    trees.findMin(checkMinPtr);
    assert(checkMinPtr==MinPtr ||
           trees.getMin(checkMinPtr)->key==trees.getMin(MinPtr)->key);
    
    trees.push(node, MinPtr);
    
    trees.findMin(checkMinPtr);
    
    assert(checkMinPtr==MinPtr ||
           trees.getMin(checkMinPtr)->key==trees.getMin(MinPtr)->key);
    return node;
  }
  
  void pop() {
    int checkMinPtr;
    trees.findMin(checkMinPtr);
    if(checkMinPtr==28)
    assert(checkMinPtr==MinPtr);
    
    auto minTree = trees.updateMin(MinPtr);
    free(minTree);
    
    int checkMinPtr2;

    trees.findMin(checkMinPtr2);
    assert(checkMinPtr2==MinPtr);
  }
  
  void decrease(AOHeapNode<KeyType>* node) {
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
        trees.findMin(MinPtr);
      }
    }
  }
  
  void increase(AOHeapNode<KeyType>* node) {
    //remove smaller key if needed
    if(trees.getMin(MinPtr) == node) trees.findMin(MinPtr);
    
    AOHeapNode<KeyType>* group1 = NULL;
    if(node->getBro() && node->getNext()) {
      if(node->getBro()->key > node->getNext()->key) {//next is small!
        if(node->getNext()->key < node->key) {
          group1 = node->getNext();
          node->removeNext();
        }
      } else {//bro is small!
        if(node->getBro()->key < node->key) {
          group1 = node->getBro();
          node->removeBro();
        }
      }
    }
    if(group1) {
      trees.push(group1, MinPtr);
    }
    
    //remove bigger key if needed
    AOHeapNode<KeyType>* group2 = NULL;
    AOHeapNode<KeyType>* single = NULL;
    if(!node->getBro() && node->getNext()) {
      node->swapBroAndNext();
    }
    if(node->getBro() && node->key > node->getBro()->key) {
      if(node->getPre()) {//非根
        single = pickoutSubTree(node);
        trees.push(single, MinPtr);
      } else {//根
        group2 = node->getBro();
        node->removeBro();
        trees.push(group2, MinPtr);
      }
    }
    
    int checkMinPtr;
    trees.findMin(checkMinPtr);
    assert(checkMinPtr==MinPtr ||
           trees.getMin(checkMinPtr)->key==trees.getMin(MinPtr)->key);
  }
  
};

#endif /* AOHeap_h */
