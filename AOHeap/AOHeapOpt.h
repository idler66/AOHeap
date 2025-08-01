//  AOHeap.h
//  Adjacent ordered heap
//  Created by 嘟嘟 on 2024/12/25.

#ifndef AOHeapOpt_h
#define AOHeapOpt_h

#include <cmath>
#include <algorithm>
#include <stdlib.h>
#include "AOHeap.h"

template <typename KeyType>
class AOListWeaverOpt {
private:
  AOHeapNode<KeyType> * waitingList[ConsolidateListSize];
  int ElementPtr;
  MinHeap<KeyType> groups;

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
    if(tree->getBro()) {
      if(AOHeapNode<KeyType> * next = tree->getNext()) {
        tree->removeNext();
        groups.push(next);
      }
      tree->moveBro2Next();
    }
  }
  
  AOHeapNode<KeyType> * getNextMinNormalizeTree(int& waitingIndex,
                                                int MaxBinaryHeapSize) {//ordered and no-changing
//    if( groups.size() < MaxBinaryHeapSize) return NULL;//stop when array empty
//    if(waitingIndex >= ElementPtr && groups.size() < 32) return NULL;//stop when array empty

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
  
  void insertionSort() {
    std::sort(waitingList, waitingList + ElementPtr,
                    [](const AOHeapNode<KeyType>* a, const AOHeapNode<KeyType>* b) {
                        return a->key < b->key;
                    });
//    for (int i = 1; i < ElementPtr; i++) {
//      AOHeapNode<KeyType> * node = waitingList[i];
//      int j = i - 1;
//      while (j >= 0 && waitingList[j]->key > node->key) {
//        waitingList[j + 1] = waitingList[j];
//        j--;
//      }
//      waitingList[j + 1] = node;
//    }
  }
  
  void linearConsolidate(int & minPtr) {
//    findMin(minPtr);
//    if(minPtr!=0) {
//      std::swap(waitingList[0], waitingList[minPtr]);
//      minPtr = 0;
//    }
//    for(int index=1; index <ElementPtr; index++) {
////      if(index == minPtr) {
////        waitingList[0] = waitingList[index];
////        continue;
////      }
//      auto node = waitingList[index];
//      groups.push(node);
//    }
//    ElementPtr = 1;
    insertionSort();
    int waitingIndex = 0;
    minPtr = 0;
    
    int heapSize = groups.size();
    int MaxBinaryHeap = 32;
    AOHeapNode<KeyType> * bro = getNextMinNormalizeTree(waitingIndex, MaxBinaryHeap);
    AOHeapNode<KeyType> * root = bro;
    while(bro) {
      AOHeapNode<KeyType> * tree = getNextMinNormalizeTree(waitingIndex, MaxBinaryHeap);
      bro->setBro(tree);
      bro = tree;
    }
    ElementPtr = 1;
//    if(root) push(root, minPtr);
    waitingList[0] = root;
  }
  
  void consolidate(int & minPtr) {
    return linearConsolidate(minPtr);
  }
  
  AOHeapNode<KeyType> * getNextMinToNode() {
    if(!groups.empty()) {
      return groups.top();
    }
    return NULL;
  }
  AOHeapNode<KeyType>* increConsolidate(AOHeapNode<KeyType>* sentinelNode) {
    AOHeapNode<KeyType> * root = getNextMinToNode();
    AOHeapNode<KeyType> * bro = root;
    while(bro) {
      groups.pop();
      if(bro == sentinelNode) break;
      normalizeTree(bro, groups);
      AOHeapNode<KeyType> * tree = getNextMinToNode();
      bro->setBro(tree);
      bro = tree;
    }
    return root;
  }
  
public:
  AOListWeaverOpt() {
    ElementPtr = 0;
    bigcounter = 0;
  }
  ~AOListWeaverOpt() {
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
  }
  
  void udpateMinPtr(AOHeapNode<KeyType>* minNode, int& minPtr) {
    minPtr = 0;
    while(waitingList[minPtr] != minNode) {
      minPtr += 1;
//      const std::string dd = "sdf";
//      if(minPtr > ElementPtr) throw std::runtime_error(dd);
    }
  }
  
  void checkMin(int minPtr) {
    int nmin;
    findMin(nmin);
    assert(minPtr == nmin);
  }
  
  AOHeapNode<KeyType> * getMin(const int& minPtr) {
    if(minPtr==-1) return NULL;
    return waitingList[minPtr];
  }
  
  AOHeapNode<KeyType> * updateMin(int& minPtr) {
//    findMin(minPtr);
    AOHeapNode<KeyType> * oldMin = waitingList[minPtr];
    AOHeapNode<KeyType> * bro = oldMin->getBro();
    if(bro != NULL) {
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
  
  AOHeapNode<KeyType>* toModify(AOHeapNode<KeyType>* node) {
    if(node->inBinaryHeap) {
      AOHeapNode<KeyType>* root = increConsolidate(node);
      return root;
    }
    return NULL;
  }
  
  AOHeapNode<KeyType>* didModified(AOHeapNode<KeyType>* oldMin) {
    if(oldMin==NULL) {
      if(!groups.empty()) {
        AOHeapNode<KeyType> * minInBH = groups.top();
        groups.pop();
//        normalizeTree(minInBH, groups);
        return minInBH;
      }
      return NULL;
    }
    if(!groups.empty()) {
      AOHeapNode<KeyType> * minInBH = groups.top();
      if(minInBH->key < oldMin->key) {
        groups.pop();
//        normalizeTree(minInBH, groups);
        return minInBH;
      }
    }
    return NULL;
  }
  
};

template <typename KeyType>
class AOHeapOpt {
private:
  AOListWeaverOpt<KeyType> trees;
  int MinPtr;
  int ElementNum;
    
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
  
  AOHeapOpt() {
    MinPtr = -1;
    ElementNum = 0;
  }
  ~AOHeapOpt() {
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
    return node;
  }
  
  void pop() {
//    operation++;
    auto minTree = trees.updateMin(MinPtr);
    free(minTree);
  }
  
  void toModify(AOHeapNode<KeyType>* node) {
    auto rslt = trees.toModify(node);
    if(rslt) {
      trees.push(rslt, MinPtr);
    }
  }
  
  void didModified() {
    auto node = trees.didModified(top());
    if(node) {
      trees.push(node, MinPtr);
    }
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
        trees.udpateMinPtr(node, MinPtr);
//        trees.findMin(MinPtr);
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
  
};

#endif /* AOHeap_h */
