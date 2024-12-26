//
//  AOHeap.h
//  Adjacent ordered heap
//
//  Created by 嘟嘟 on 2024/12/25.
//

#ifndef AOHeap_h
#define AOHeap_h

static const long int AOHReshapeCount = 32;
static const long int AOHSubGroupSize = 64;
static const bool AOHAllocDynamic = true;
static const bool AOHStat = true;

enum AOHNodePosType : unsigned char {
  AOHNodePosInNone = 1,
  AOHNodePosInMainTree = 2,
  AOHNodePosInWaitingList = 4,
};

template <typename KeyType, typename DataType>
struct AOHeapNode {
private:
public:
  struct AOHeapNode *bro, *pre, *next;
  AOHNodePosType nodePos;
  KeyType key;
  DataType data;
  long int nid;
  
  AOHeapNode() {
    init();
  }
  void init() {
    next = NULL;
    bro = NULL;
    pre = NULL;
    nodePos = AOHNodePosInNone;
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

template <typename KeyType, typename DataType>
class AOHeap {
private:
  AOHeapNode<KeyType, DataType> * mainTree;
  AOHeapNode<KeyType, DataType> * waitingList;
  AOHeapNode<KeyType, DataType>* NodeList;
  long int nodeNum;
  long int mergeHeight;
  long int mergeNum;

  void reshape(AOHeapNode<KeyType, DataType>* root) {
    AOHeapNode<KeyType, DataType> * visitor = root;
    while(visitor) {
      visitor->swapBroAndNext();
      visitor = visitor->getBro();
    }
  }
  
  void batchInsert(AOHeapNode<KeyType, DataType> * sentinel,
                       AOHeapNode<KeyType, DataType> * start,
                       AOHeapNode<KeyType, DataType> * end) {
    AOHeapNode<KeyType, DataType> * root = sentinel->getPre();
//    sentinel->removePre();
    end->setBro(sentinel);
    root->setNext(start);
  }
  
  void appendToLeft(AOHeapNode<KeyType, DataType> * x,
                  AOHeapNode<KeyType, DataType> * pre,
                  AOHeapNode<KeyType, DataType> * after) {
    if(after) {
      after->removePre();
      x->setBro(after);
    }
    pre->setNext(x);
  }
  
  void setHeader(AOHeapNode<KeyType, DataType>* h) {
    h->nodePos = AOHNodePosInMainTree;
    mainTree = h;
  }
  
  AOHeapNode<KeyType, DataType>* meld(AOHeapNode<KeyType, DataType>* first,
                                      AOHeapNode<KeyType, DataType>* second) {
    if(second) second->nodePos = AOHNodePosInMainTree;
    if(!first) {
      return second;
    }
    if(second->key < first->key) {
      std::swap(first, second);
    }
    long height = 1;
    AOHeapNode<KeyType, DataType>* mhd = first;
    while(mhd->getNext() && mhd->getNext()->key < second->key) {
      height++;
      mhd = mhd->getNext();
    }
    mergeHeight+=height;
    mergeNum++;
    if(second) appendToLeft(second, mhd, mhd->getNext());
    if(height > AOHReshapeCount) {
      reshape(first->getNext());
    }
    return first;
  }
  
  void push(AOHeapNode<KeyType, DataType>* h) {//merge with meld
    h->nodePos = AOHNodePosInMainTree;
    if(!mainTree) {
      mainTree = h;
      return;
    }
    if(h->key < mainTree->key) {
      std::swap(h, mainTree);
    }
    long height = 1;
    AOHeapNode<KeyType, DataType>* mhd = mainTree;
    while(mhd->getNext() && mhd->getNext()->key < h->key) {
      mhd = mhd->getNext();
      height++;
    }
    mergeHeight+=height;
    mergeNum++;
    appendToLeft(h, mhd, mhd->getNext());
    if(height > AOHReshapeCount) {
      reshape(mainTree->getNext());
    }
  }

  AOHeapNode<KeyType, DataType> * group2StandardTree(AOHeapNode<KeyType, DataType> * remainder) {
    if(!remainder) return NULL;
    AOHeapNode<KeyType, DataType> * stdtree = remainder;
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
    
    AOHeapNode<KeyType, DataType> * visitor = remainder;
    AOHeapNode<KeyType, DataType> * sentinel = stdtree->getNext();
    if(!sentinel) {
      stdtree->setNext(remainder);
      return stdtree;
    }
    AOHeapNode<KeyType, DataType> * end = NULL;
    while(visitor) {
      bool hasItems = false;
      while(visitor && visitor->key <= sentinel->key) {
        visitor->swapBroAndNext();
        end = visitor;
        visitor = visitor->getBro();
        hasItems = true;
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
  
AOHeapNode<KeyType, DataType>* _consolidate() {
  long int counter = 1;
  AOHeapNode<KeyType, DataType>* group = NULL;
  AOHeapNode<KeyType, DataType>* sum = NULL;
  while (waitingList!=NULL) {
    auto local = waitingList;
    waitingList = waitingList->getBro();
    local->bro = NULL;
    local->pre = NULL;
    if(counter%AOHSubGroupSize==0) {
      counter = 1;
      group = meld(group, local);
      sum = meld(sum, group);
      group = NULL;
    } else {
      group = meld(group, local);
      counter += 1;
    }
  }
  if(group) sum = meld(sum, group);
  if(!sum) return NULL;
  return sum;
}

void _insert(AOHeapNode<KeyType, DataType> *h) {
  if(!mainTree) {
    setHeader(h);
    return;
  }
  if(h->key <= top()->key) {
    push(h);
    return;
  }
  h->nodePos = AOHNodePosInWaitingList;
  if (waitingList) {
    h->bro = waitingList;
    waitingList->pre = h;
  }
  waitingList = h;
}

void pickOut(AOHeapNode<KeyType, DataType> *mainNode) {
  if (mainNode == mainTree) {
    mainTree = NULL;
    return ;
  }
  if(mainNode->nodePos == AOHNodePosInMainTree) {
    _pickOutFromMainTree(mainNode);
  } else if(mainNode->nodePos == AOHNodePosInWaitingList) {
    _pickoutFromWaitingList(mainNode);
  }
}

AOHeapNode<KeyType, DataType>* _pickoutFromWaitingList(AOHeapNode<KeyType, DataType> *node) {
  if(node==waitingList) {
    waitingList = waitingList->bro;
    if(waitingList) waitingList->pre = NULL;
    node->bro = NULL;
  } else {
    auto pre = node->pre;
    auto bro = node->bro;
    pre->bro = bro;
    if(bro) bro->pre = pre;
  }
  node->bro = NULL;
  node->pre = NULL;
  return node;
}

AOHeapNode<KeyType, DataType> * _pickOutFromMainTree(AOHeapNode<KeyType, DataType>* x) {
  AOHeapNode<KeyType, DataType> * pre = x->getPre();
  AOHeapNode<KeyType, DataType>* bro = x->getBro();
  if(pre->getNext() == x) {
    x->removePre();
    x->removeBro();
    pre->setNext(bro);
    return x;
  }
//    if(pre->getBro() == x) {
    x->removePre();
    x->removeBro();
    pre->setBro(bro);
//    }
  return x;
}
  
public:
  long int prevalue;
  
  AOHeap(long int _sizeOfNode) {
    mergeNum = 0;
    mergeHeight = 0;
    prevalue = LONG_MIN;
    nodeNum = _sizeOfNode;
    if(!AOHAllocDynamic) {
      NodeList = (AOHeapNode<KeyType, DataType>*)malloc(sizeof(AOHeapNode<KeyType, DataType>)*nodeNum);
      memset(NodeList, 0, sizeof(AOHeapNode<KeyType, DataType>)*nodeNum);
    }
    mainTree = NULL;
    waitingList = NULL;
  }
  ~AOHeap() {
    if(!AOHAllocDynamic) {
      free(NodeList);
    }
  }
  
  AOHeapNode<KeyType, DataType>* top() {
    return mainTree;
  }
  bool empty() {
    return mainTree == NULL;
  }
  
  struct AOHeapNode<KeyType, DataType> * insert(KeyType& key, long int& nid) {
    if(prevalue > key) prevalue = key;
    struct AOHeapNode<KeyType, DataType> * node;
    if(!AOHAllocDynamic) {
      node = (this->NodeList + nid);
    } else {
      node = new AOHeapNode<KeyType, DataType>();
    }
    node->key = key;
    node->nid = nid;
    _insert(node);
    return node;
  }
  
  void pop() {
    auto waitingTree = _consolidate();
    AOHeapNode<KeyType, DataType> * remainder = mainTree->getNext();
    mainTree->removeNext();
    mainTree->init();
    mainTree = NULL;
    
    auto leftTree = group2StandardTree(remainder);
    if(!leftTree && !waitingTree) {
      if(AOHAllocDynamic) {
        free(mainTree);
      }
      return;
    }
    if(!leftTree && waitingTree) {
      if(AOHAllocDynamic) {
        free(mainTree);
      }
      mainTree = waitingTree;
      mainTree->nodePos = AOHNodePosInMainTree;
      return;
    }
    if(leftTree && !waitingTree) {
      if(AOHAllocDynamic) {
        free(mainTree);
      }
      mainTree = leftTree;
      mainTree->nodePos = AOHNodePosInMainTree;
      return;
    }
    
    if(waitingTree->key < leftTree->key) {
      std::swap(waitingTree, leftTree);
    }
    mainTree = leftTree;
    mainTree->nodePos = AOHNodePosInMainTree;
    waitingList = waitingTree;
    waitingList->nodePos = AOHNodePosInWaitingList;
  }
  
  void increase(AOHeapNode<KeyType, DataType>* node) {
    AOHeapNode<KeyType, DataType>* tree = NULL;
    AOHeapNode<KeyType, DataType>* group = NULL;
    bool consolidate = false;
    if(node == mainTree) consolidate = true;
    if(node == mainTree || node->nodePos == AOHNodePosInWaitingList) {
      if(node->getNext() && node->key > node->getNext()->key) {
        group = node->getNext();
        node->removeNext();
      }
    } else {
      if (node->getBro() && node->key > node->getBro()->key) {
        pickOut(node);
        tree = node;
      } else if (node->getNext() && node->key > node->getNext()->key) {
        node->swapBroAndNext();//ordered
        pickOut(node);
        group = node;
      }
    }
    if(group) {
      auto nextTree = group2StandardTree(group);
      _insert(nextTree);
    } else if(tree) {
      if (tree->getNext() && tree->key > tree->getNext()->key) {
        group = tree->getNext();
        tree->removeNext();
        auto nextTree = group2StandardTree(group);
        _insert(nextTree);
      }
      _insert(tree);
    }
    if(consolidate) {
      auto waitingTree = _consolidate();
      if(waitingTree) {
        _insert(waitingTree);
      }
    }
  }
  
  void decrease(AOHeapNode<KeyType, DataType>* mainNode) {
    if(mainNode->key < prevalue) {
      prevalue = mainNode->key;
    }
    if (mainNode == mainTree) {
      return;
    }
    if(mainNode->nodePos==AOHNodePosInWaitingList) {
      if(mainTree->key > mainNode->key) {
        _pickoutFromWaitingList(mainNode);
        push(mainNode);
      }
      return;
    }
    if(mainNode->getPre()->key > mainNode->key) {
      _pickOutFromMainTree(mainNode);
      _insert(mainNode);
    }
  }
  
  float T() {
    if(mergeNum==0) return 0;
    return (float)mergeHeight/(float)mergeNum;
  }
};

#endif /* AOHeap_h */
