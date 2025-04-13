//
//  MixSeqsGenerator.h
//  Test
//
//  Created by 嘟嘟 on 2025/3/8.
//

#ifndef MixSeqsGenerator_h
#define MixSeqsGenerator_h

#include <utility>
#include <tuple>
#include <iostream>
#include <string_view>
#include <vector>
#include <unordered_map>
 
#include "CommonDef.h"
#include "AOPTester.h"

/*
 混合序列生成器，包括：插入，删除，键值增加，键值降低。
 参数：1 节点数量 numOfNodeInBaseSize，以BaseSize 表示；
      2 预插入节点数量 numOfNodePreInsert [0-0.8]（在键值变更和删除最小之前连续插入多少个节点）。
      3 mixScale, 需要混合生成的插入、键值增加、键值降低、删除序列是 节点数量 的多少倍,0，1，2，3...。
      4 ratioOfDelete2InDc，键值增加与键值降低之和，是删除最小事件比例的多少倍，0，1，2，3...。
        如果随机生成的节点已在堆中，则根据ratio决定执行哪个操作【键值增加，键值降低，删除】。
 */

class MixSeqsGenerator {
  std::vector<int> seqsKey;
  std::vector<int> seqsID;
  std::vector<int> seqsType;
  std::ofstream ofile;
  
public:
  MixSeqsGenerator() {
    srand(static_cast<unsigned int>(time(0)));
  }
  
  void generate(struct GenSeqsParas paras) {
    
    for (int size=1; size<=SeqSize1000000; size*=10) {
      
      int NumberOfNode = size * paras.numOfNodeInBaseSize;
      
      startWrite("mix", size, paras);
      seqsKey.clear();
      seqsID.clear();
      seqsType.clear();
      auto heap = new GenHeapTester<int>();
      
//      int numOfNodePreInsert = 0.5*NumberOfNode;
//      while (numOfNodePreInsert >0) {
//        if(seqsID.size()>BatchSize) write();
//        int nodeid = randInRange(NumberOfNode-1);
//        if(!heap->elementExist(nodeid)) {
//          //0 pop; 1 insert; 2 increase dlt; 3 decrease dlt.
//          int dltkey = randInRange(NumberOfNode-1)+1;
//          heap->insert(dltkey, nodeid);
//          seqsKey.push_back(dltkey);
//          seqsID.push_back(nodeid);
//          seqsType.push_back(1);
//          numOfNodePreInsert--;
//        }
//      }
//      write();
      
      for (int index = 0; index < NumberOfNode*paras.mixScale; index++) {
        if(seqsID.size()>BatchSize) write();
        int dltkey = randInRange(NumberOfNode-1)+1;
        int nodeid = randInRange(NumberOfNode-1);
        if (heap->elementExist(nodeid)) {
          int type = randInRange(paras.DeleteUnderInDc);
          if(type>=1) {
            type = type %  2;
            type += 2;
          }
          switch (type) {
            case 0: {
              while (true) {
                auto node = heap->top();
                nodeid = node->nid;
                int key = node->key;
//                  assert(heap->elementExist(nodeid));
                heap->popOne();
//                  assert(!heap->elementExist(nodeid));
                seqsKey.push_back(0);
                seqsID.push_back(nodeid);
                seqsType.push_back(0);
                auto nnode = heap->top();
                if(nnode==NULL || nnode->key != key) break;
              }
              break;
            }
            case 2:
//                assert(heap->elementExist(nodeid));
              heap->increase(nodeid, dltkey);
//                assert(heap->elementExist(nodeid));
              seqsKey.push_back(dltkey);
              seqsID.push_back(nodeid);
              seqsType.push_back(2);
              break;
            case 3:
//                assert(heap->elementExist(nodeid));
              heap->decrease(nodeid, dltkey);
//                assert(heap->elementExist(nodeid));
              seqsKey.push_back(dltkey);
              seqsID.push_back(nodeid);
              seqsType.push_back(3);
              break;
            default:{
              break;
            }
          }
        } else {
//            assert(!heap->elementExist(nodeid));
          heap->insert(dltkey, nodeid);
//            assert(heap->elementExist(nodeid));
          seqsKey.push_back(dltkey);
          seqsID.push_back(nodeid);
          seqsType.push_back(1);
        }
      }
      write();
      while(!heap->empty()) {
        auto node = heap->top();
        int nodeid = node->nid;
//          assert(heap->elementExist(nodeid));
        heap->popOne();
//          assert(!heap->elementExist(nodeid));
        seqsKey.push_back(0);
        seqsID.push_back(nodeid);
        seqsType.push_back(0);
        if(seqsID.size()>BatchSize) write();
      }
      write();
      finishWrite();
    }
  }
  
  void startWrite(std::string name, int size, struct GenSeqsParas paras) {
    std::string root = getProjectRoot();
    std::string base = root.append("/gen_seq/");
    std::string append = ".edges";
    std::string path = base;
    path.append(name).append("_").append(std::to_string(paras.numOfNodeInBaseSize)).append("_").append(std::to_string((int)(paras.mixScale))).append("_").append(std::to_string((int)(paras.DeleteUnderInDc))).append("_").append(std::to_string(size)).append(append);
    ofile.open(path.c_str(), std::ios::out | std::ios::trunc);
    if (!ofile.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        assert(0);
    }
  }
  
  void write() {
    
    for (int index=0; index < seqsKey.size(); index++) {
      ofile <<seqsID[index]  <<"," <<seqsKey[index]   <<"," <<seqsType[index] <<"," <<std::endl;
    }
    ofile.flush();
    
    seqsKey.clear();
    seqsID.clear();
    seqsType.clear();
  }
  
  void finishWrite() {
    ofile.close();
  }
  
};


#endif /* MixSeqsGenerator_h */


