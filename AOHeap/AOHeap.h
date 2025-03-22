//  AOHeap.h
//  Adjacent ordered heap
//  Created by 嘟嘟 on 2024/12/25.

#ifndef AOHeap_h
#define AOHeap_h

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
    init();
  }
  void init() {
    next = NULL;
    bro = NULL;
    pre = NULL;
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
  
  void swapBroAndNext() {
    std::swap(bro, next);
  }
  void moveBro2Next() {
    auto obro = bro;
    removeNext();
    removeBro();
    setNext(obro);
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
class AOList {
private:
  AOHeapNode<KeyType> * waitingList[ConsolidateListSize];
  //next blank space, [0,ConsolidateListSize-1]. ConsolidateListSize consolidate triggered!
  int ElementPtr;
  
private:
  void batchInsert(AOHeapNode<KeyType> * sentinel,
                       AOHeapNode<KeyType> * start,
                       AOHeapNode<KeyType> * end) {
    AOHeapNode<KeyType> * root = sentinel->getPre();
    end->setBro(sentinel);
    root->setNext(start);
  }
  
  void normalizeTree(AOHeapNode<KeyType> * tree,
                    MinHeap<KeyType>& groups) {
    if(tree->getBro()) {
      if(AOHeapNode<KeyType> * next = tree->getNext()) {
        tree->removeNext();
        groups.push(next);
      }
      tree->moveBro2Next();
    }
  }
  
  AOHeapNode<KeyType> * getNextMin(int& waitingIndex,
                                  MinHeap<KeyType>& groups) {
    AOHeapNode<KeyType> * one = NULL;
    if(waitingIndex < ElementPtr) one = waitingList[waitingIndex];
    AOHeapNode<KeyType> * two = NULL;
    if(!groups.empty()) {
      two = groups.top();
    }
    if(one && two) {
      if(one->key > two->key) {
        one = NULL;
      } else {
        two = NULL;
      }
    }
    if(one) {
      waitingIndex++;
      normalizeTree(one, groups);
      return one;
    }
    if(two) {
      groups.pop();
      normalizeTree(two, groups);
      return two;
    }
    return NULL;
  }
  
  void noSentinel(AOHeapNode<KeyType> * start,
                  int & waitingIndex,
                  AOHeapNode<KeyType> * preSentinel,
                  MinHeap<KeyType>& groups) {
    if(!start) return;;
    preSentinel->setNext(start);
    while(AOHeapNode<KeyType> * next = getNextMin(waitingIndex, groups)) {
      start->setBro(next);
      start = next;
    }
  }
  
  void consolidate() {
    std::sort(waitingList, waitingList+ElementPtr,
              [](const AOHeapNode<KeyType> & a, const AOHeapNode<KeyType> * b) { return b->key < b->key; });
    MinHeap<KeyType> groups;
    AOHeapNode<KeyType> * preSentinel = waitingList[0];//root & sentinel
    normalizeTree(preSentinel, groups);
    int waitingIndex = 1;
    AOHeapNode<KeyType> * sentinel = preSentinel->getNext();
    AOHeapNode<KeyType> * start = getNextMin(waitingIndex, groups);
    while(start) {
      if(!sentinel) { //get whole bro chain, no any sentinel
        return noSentinel(start, waitingIndex, preSentinel, groups);
      } else {//get longest bro chain
        while(sentinel->key < start->key) {//find valid sentinel
          preSentinel = sentinel;
          sentinel = sentinel->getNext();
          if(!sentinel) return noSentinel(start, waitingIndex, preSentinel, groups);
        }
        AOHeapNode<KeyType> *end = start;
         getNextMin(waitingIndex, groups);
        while(sentinel->key >= end->key) {
          AOHeapNode<KeyType> *pre = end;
          end = getNextMin(waitingIndex, groups);
          pre->setBro(end);
          pre = end;
        }
        batchInsert(sentinel, start, end->getPre());
        start = end;
      }
      preSentinel = sentinel;
      sentinel = sentinel->getNext();
    }
    ElementPtr = 1;
  }
  
public:
  AOList() {
    ElementPtr = 0;
  }
  bool empty() {
    return ElementPtr==0;
  }
  void push(AOHeapNode<KeyType> * node, int& ptr) {
    ptr = ElementPtr;
    waitingList[ElementPtr++] = node;
    if(ElementPtr == ConsolidateListSize) {
      consolidate();
    }
  }
  AOHeapNode<KeyType> * getMin() {
    if(ElementPtr > 1 ) consolidate();
    AOHeapNode<KeyType> * root = waitingList[0];
    return root;
  }
  void replace(AOHeapNode<KeyType> * node, int& ptr) {
    assert(0 <= ptr < ConsolidateListSize);
    waitingList[ptr] = node;
  }
};

template <typename KeyType>
class AOHeap {
private:
  AOHeapNode<KeyType> * mainTree;
  AOList<KeyType> waitingList;
  int MinPtr;

  void reshape(AOHeapNode<KeyType>* root) {
    AOHeapNode<KeyType> * visitor = root;
    while(visitor) {
      visitor->swapBroAndNext();
      visitor = visitor->getBro();
    }
  }
  
  void batchInsert(AOHeapNode<KeyType> * sentinel,
                       AOHeapNode<KeyType> * start,
                       AOHeapNode<KeyType> * end) {
    AOHeapNode<KeyType> * root = sentinel->getPre();
//    sentinel->removePre();
    end->setBro(sentinel);
    root->setNext(start);
  }
  
  void appendToLeft(AOHeapNode<KeyType> * x,
                  AOHeapNode<KeyType> * pre,
                  AOHeapNode<KeyType> * after) {
    if(after) {
      after->removePre();
      x->setBro(after);
    }
    pre->setNext(x);
  }

  AOHeapNode<KeyType> * group2StandardTree(AOHeapNode<KeyType> * remainder) {
    if(!remainder) return NULL;
    AOHeapNode<KeyType> * stdtree = remainder;
    if(stdtree->getBro()==NULL || stdtree->getNext()==NULL) {
      if(stdtree->getNext()==NULL) {
        stdtree->moveBro2Next();
      }
      return stdtree;
    } else {
      remainder = stdtree->getNext();
      stdtree->moveBro2Next();
      if(!remainder) {
        return stdtree;
      }
    }
    
    AOHeapNode<KeyType> * visitor = remainder;
    AOHeapNode<KeyType> * sentinel = stdtree->getNext();
    if(!sentinel) {
      stdtree->setNext(remainder);
      return stdtree;
    }
    AOHeapNode<KeyType> * end = NULL;
    while(visitor) {
      bool hasItems = false;
      if(visitor->key <= sentinel->key) hasItems = true;
      while(visitor && visitor->key <= sentinel->key) {
//        visitor->swapBroAndNext();
//        end = visitor;
//        visitor = visitor->getBro();
        end = visitor;
        visitor = visitor->getNext();
        end->swapBroAndNext();
      }
      if(hasItems) {
        batchInsert(sentinel, remainder, end);
        remainder = visitor;
//        end = NULL;
      }
      if(!sentinel->getNext()) {
        sentinel->setNext(remainder);
        break;
      } else {
        sentinel = sentinel->getNext();
      }
    }
    return stdtree;
  }

AOHeapNode<KeyType> * pickoutSubTree(AOHeapNode<KeyType>* x) {
  //返回标准子树
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
    mainTree = NULL;
  }
  ~AOHeap() {
  }
  
  AOHeapNode<KeyType>* top() {
    return mainTree;
  }
  bool empty() {
    return mainTree == NULL;
  }
  
  struct AOHeapNode<KeyType> * insert(KeyType& key, int& nid) {
    struct AOHeapNode<KeyType> * node = new AOHeapNode<KeyType>();
    node->key = key;
    node->nid = nid;
    int ptr;
    waitingList.push(node, ptr);
    if(!mainTree || node->key < mainTree->key) {
      mainTree = node;
      MinPtr = ptr;
    }
    return node;
  }
  
  void pop() {
    //有两种方式：1 独立；2 融合。
    //1 删除最小，用群替代根节点。
    //2 排序，合并，并更新主子树。
    AOHeapNode<KeyType> * remain = mainTree->getNext();
    mainTree->removeNext();
    free(mainTree);
    mainTree = NULL;
    waitingList.replace(remain, MinPtr);
    mainTree = waitingList.getMin();
    MinPtr = 0;
  }
  
  void increase(AOHeapNode<KeyType>* node) {
    //remove smaller key if needed
    AOHeapNode<KeyType>* group1 = NULL;
    if(node->getBro() && node->getNext()) {
      if(node->getBro()->key > node->getNext()->key) {
        if(node->getNext()->key < node->key) {
          group1 = node->getNext();
          node->removeNext();
        }
      } else {
        if(node->removeBro()->key < node->key) {
          group1 = node->getBro();
          node->removeBro();
        }
      }
    }
    if(group1) {
      auto tree = group2StandardTree(group1);
      waitingList.push(tree);
    }
    //remove bigger key if needed
    AOHeapNode<KeyType>* group2 = NULL;
    AOHeapNode<KeyType>* single = NULL;
    if(!node->getBro() && node->getNext()) {
      node->swapBroAndNext();
    }
    int ptr;
    if(node->getBro() && node->key > node->getBro()->key) {
      if(node->getPre()) {
        single = pickoutSubTree(node);
        waitingList.push(single, ptr);
      } else {
        group2 = node->getBro();
        node->removeBro();
        auto tree = group2StandardTree(group2);
        waitingList.push(tree, ptr);
      }
    }
    //更新根节点
    if(node == mainTree) {
      mainTree = waitingList.getMin(ptr);
      MinPtr = ptr;
    }
  }
  
  void decrease(AOHeapNode<KeyType>* node) {
    if (node == mainTree) {
      return;
    }
    int ptr;
    if(node->getPre()) {//非根节点
      if(node->getPre()->key > node->key) {
        pickoutSubTree(node);
        waitingList.push(node, ptr);
      }
    }
    if(mainTree->key > node->key) {
      mainTree = node;
      MinPtr = ptr;
    }
  }
  
};

#endif /* AOHeap_h */
