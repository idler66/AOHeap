//  AOHeap.h
//  Adjacent ordered heap
//  Created by 嘟嘟 on 2024/12/25.

#ifndef AOHeapMin_h
#define AOHeapMin_h

#include <cmath>
#include <algorithm>

//insert and delete control


template <typename KeyType>
struct AOHeapNodeMin {
private:
public:
  struct AOHeapNodeMin *bro, *pre, *next;
  KeyType key;
  int nid;
  AOHeapNodeMin() {
    next = NULL;
    bro = NULL;
    pre = NULL;
  }
  void setPre(struct AOHeapNodeMin *n) {
    pre = n;
    if(pre) assert(key >= pre->key);
  }
  struct AOHeapNodeMin * getPre() {
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
  
  void setWaiting() {
    nid |= (1 << (sizeof(int) * 8 - 1));
  }
  void clearWaiting() {
    nid &= ~(1 << (sizeof(int) * 8 - 1));
  }
  bool isWaiting() {
    return (nid & (1 << (sizeof(int) * 8 - 1))) != 0;
  }
  
  void moveBro2Next() {
    next = bro;
    bro = NULL;
  }
  void setNext(struct AOHeapNodeMin *n) {
    next = n;
    if(next) {
      next->setPre(this);
    }
  }
  struct AOHeapNodeMin* getNext() {
    return next;
  }
  void removeNext() {
    if(next) {
      next->setPre(NULL);
      next = NULL;
    }
  }
  void setBro(struct AOHeapNodeMin *n) {
    bro = n;
    if(bro) {
      bro->setPre(this);
    }
  }
  struct AOHeapNodeMin * getBro() {
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
class MinHeapMin {
private:
  std::vector<AOHeapNodeMin<KeyType> *> heap;
  static bool compare(const AOHeapNodeMin<KeyType> * a,
                      const AOHeapNodeMin<KeyType> * b) {
    return a->key > b->key;
  }
public:
  MinHeapMin() {}
  void push(AOHeapNodeMin<KeyType> * node) {
    node->setWaiting();
    heap.push_back(node);
    std::push_heap(heap.begin(), heap.end(), compare);
  }
  void pop() {
    heap.front()->clearWaiting();
    std::pop_heap(heap.begin(), heap.end(), compare);
    heap.pop_back();
  }
  AOHeapNodeMin<KeyType> * top() {
    return heap.front();
  }
  bool topIsWaiting() {
    auto top = heap.front();
    return top->isWaiting();
  }
  bool empty() {
    return heap.empty();
  }
  int size() {
    return heap.size();
  }
};

static const int ConsolidateListSizeMin = 64+32;
template <typename KeyType>
class AOListWeaverMin {
private:
  AOHeapNodeMin<KeyType> * waitingList[ConsolidateListSizeMin];
  int ElementPtr;
  int MinPtrInList;
  MinHeapMin<KeyType> groups;
  
  void batchInsert(AOHeapNodeMin<KeyType> * sentinel,
                       AOHeapNodeMin<KeyType> * start,
                       AOHeapNodeMin<KeyType> * end) {
    AOHeapNodeMin<KeyType> * root = sentinel->getPre();
    end->setBro(sentinel);
    root->setNext(start);
  }
  
  void normalizeTree(AOHeapNodeMin<KeyType> * tree) {
    if (!groups.empty()) {
      assert(tree!=groups.top());
    }
    if(tree->getBro()) {
      if(AOHeapNodeMin<KeyType> * next = tree->getNext()) {
        tree->removeNext();
        groups.push(next);
        assert(next->key >= tree->key);
      }
      tree->moveBro2Next();
    }
  }
  
  void insertionSort() {
    for (int i = 1; i < ConsolidateListSizeMin; i++) {
      AOHeapNodeMin<KeyType> * node = waitingList[i];
      int j = i - 1;
      while (j >= 0 && waitingList[j]->key > node->key) {
        waitingList[j + 1] = waitingList[j];
        j--;
      }
      waitingList[j + 1] = node;
    }
  }
  AOHeapNodeMin<KeyType> * getNextMinTreeInList() {
    AOHeapNodeMin<KeyType> * one = NULL;
    if(MinPtrInList < ConsolidateListSizeMin) one = waitingList[MinPtrInList];
    if(one) {
      MinPtrInList++;
      normalizeTree(one);
      return one;
    }
    return NULL;
  }
  void quickNoSentinel(AOHeapNodeMin<KeyType> * preSentinel,
                  AOHeapNodeMin<KeyType> * start) {
    if(start) {
      preSentinel->setNext(start);
      while(AOHeapNodeMin<KeyType> * next = getNextMinTreeInList()) {
        start->setBro(next);
        start = next;
      }
    }
  }
  void quickConsolidate() {//合并list中的树,并把部分抛入heap中
    MinPtrInList = 0;
    ElementPtr = 1;
    insertionSort();
    AOHeapNodeMin<KeyType> * preSentinel = getNextMinTreeInList();
    AOHeapNodeMin<KeyType> * sentinel = preSentinel->getNext();
    AOHeapNodeMin<KeyType> * start = getNextMinTreeInList();
    while(start) {
      if(!sentinel) {
        quickNoSentinel(preSentinel, start);
        MinPtrInList = 0;
        return;
      }
      //find valid sentinel.
      while(sentinel->key < start->key) {
        preSentinel = sentinel;
        sentinel = sentinel->getNext();
        if(!sentinel) {
          quickNoSentinel(preSentinel, start);
          MinPtrInList = 0;
          return;
        }
      }
      //find longest bro chain and insert.
      AOHeapNodeMin<KeyType> *end = start;
      AOHeapNodeMin<KeyType> *next = getNextMinTreeInList();
      while(next) {
        if(sentinel->key >= next->key) {
          end->setBro(next);
          end = next;
          next = getNextMinTreeInList();
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
    MinPtrInList = 0;
  }
  
public:
  AOListWeaverMin() {
    ElementPtr = 0;
    MinPtrInList = -1;
  }
  bool empty() {
    return ElementPtr==0;
  }
  
  void push(AOHeapNodeMin<KeyType> * node) {
    waitingList[ElementPtr] = node;
    if(MinPtrInList == -1) {
      MinPtrInList = 0;
    } else if(waitingList[MinPtrInList]->key > node->key) {
      MinPtrInList = ElementPtr;
    }
    ElementPtr += 1;
    if(ElementPtr == ConsolidateListSizeMin) {
      quickConsolidate();
    }
  }
  
  void findMinInList() { //找到list中最小的
    if(ElementPtr==0) {
      MinPtrInList = -1;
      return;
    }
    MinPtrInList = 0;
    int next = 1;
    while(next < ElementPtr) {
      if(waitingList[MinPtrInList]->key > waitingList[next]->key) {
        MinPtrInList = next;
      }
      next += 1;
    }
  }
  
  void udpateMinPtr(AOHeapNodeMin<KeyType>* minNode) {
    //明确minNode最小，且在list中，快速定位最小者
    assert(MinPtrInList < ElementPtr);
    MinPtrInList = 0;
    while(waitingList[MinPtrInList] != minNode) {
      MinPtrInList += 1;
    }
  }
  
  void moveMinToList() {//如果heap中更小，移动到list中
    if(!groups.empty()) {
      auto top = groups.top();
      if(MinPtrInList == -1 || top->key < waitingList[MinPtrInList]->key) {
        groups.pop();
        push(top);
      }
    }
  }
  void move2List(AOHeapNodeMin<KeyType>* node) {
    AOHeapNodeMin<KeyType>* head = NULL;
    AOHeapNodeMin<KeyType>* holder = NULL;
    while(!groups.empty()) {
      auto top = groups.top();
      groups.pop();
      if(head!=NULL) {
        holder->setBro(top);
        holder = top;
      } else {
        head = top;
        holder = top;
      }
//      if(top == node) break;
      normalizeTree(top);
    }
    push(head);
  }
  
  AOHeapNodeMin<KeyType> * getMin() {
    if(MinPtrInList==-1) return NULL;
    assert(MinPtrInList < ElementPtr);
    return waitingList[MinPtrInList];
  }
  
  AOHeapNodeMin<KeyType> * updateMin() {
    assert(-1 < MinPtrInList);
    assert(MinPtrInList < ElementPtr);
    AOHeapNodeMin<KeyType> * oldMin = waitingList[MinPtrInList];
    AOHeapNodeMin<KeyType> * bro = oldMin->getBro();
    if(bro) {
      oldMin->removeBro();
    }
    AOHeapNodeMin<KeyType> * next = oldMin->getNext();
    if(next) {
      oldMin->removeNext();
    }
    if(next && bro) {
      if(bro->key > next->key) std::swap(bro, next);
      waitingList[MinPtrInList] = bro;
      waitingList[ElementPtr++] = next;
      if(ElementPtr == ConsolidateListSizeMin) {
        quickConsolidate();
        moveMinToList();
        return oldMin;
      }
    }else if (next) {
      waitingList[MinPtrInList] = next;
    } else if(bro) {
      waitingList[MinPtrInList] = bro;
    } else {
      ElementPtr-=1;
      waitingList[MinPtrInList] = waitingList[ElementPtr];
    }
    findMinInList();
    moveMinToList();
    return oldMin;
  }
  
};

template <typename KeyType>
class AOHeapMin {
private:
  AOListWeaverMin<KeyType> trees;
  
  void batchInsert(AOHeapNodeMin<KeyType> * sentinel,
                       AOHeapNodeMin<KeyType> * start,
                       AOHeapNodeMin<KeyType> * end) {
    AOHeapNodeMin<KeyType> * root = sentinel->getPre();
    end->setBro(sentinel);
    root->setNext(start);
  }

AOHeapNodeMin<KeyType> * pickoutSubTree(AOHeapNodeMin<KeyType>* x) {//返回标准子树
  AOHeapNodeMin<KeyType> * pre = x->getPre();
  AOHeapNodeMin<KeyType>* bro = x->getBro();
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
  
  AOHeapMin() {
  }
  ~AOHeapMin() {
  }
  
  AOHeapNodeMin<KeyType>* top() {
    return trees.getMin();
  }
  
  bool empty() {
    return trees.empty();
  }
  
  struct AOHeapNodeMin<KeyType> * insert(KeyType& key, int& nid) {
    /*
     插入逻辑：列表->满了就批量合并->子树群部分到minheap
     */
    struct AOHeapNodeMin<KeyType> * node = new AOHeapNodeMin<KeyType>();
    node->key = key;
    node->nid = nid;
    trees.push(node);
    return node;
  }
  
  void pop() { //删除最小，剩余insert，更新最小到列表中
    auto minTree = trees.updateMin();
    free(minTree);
  }
  
  void move2List(AOHeapNodeMin<KeyType>* node) {
    trees.move2List(node);
  }
  
  void decrease(AOHeapNodeMin<KeyType>* node) {
    if (node == trees.getMin()) {
      return;
    }
    if(node->getPre()) {//新树
      if(node->getPre()->key > node->key) {
        pickoutSubTree(node);
        trees.push(node);
      }
    } else  {//原树
      if(trees.getMin()->key > node->key) {
        trees.udpateMinPtr(node);
      }
    }
  }
  
  void increase(AOHeapNodeMin<KeyType>* node) {
    if(trees.getMin() == node) {
      trees.findMinInList();
    }
    AOHeapNodeMin<KeyType> * next = node->getNext();
    AOHeapNodeMin<KeyType> * bro = node->getBro();
    if(bro && node->key > bro->key) {
      node->removeBro();
      trees.push(bro);
    }
    if(next && node->key > next->key) {
      node->removeNext();
      trees.push(next);
    }
  }
  
};

#endif /* AOHeap_h */

