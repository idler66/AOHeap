//
//  AOPSPSeqTester.h
//  Test
//
//  Created by 嘟嘟 on 2025/2/27.
//

#ifndef AOPSPSeqTester_h
#define AOPSPSeqTester_h

#include "AOPFileReader.h"
#include "AOPTester.h"

#include <cmath>
#include <unordered_map>
#include <sys/resource.h>
#include <iostream>
#include <mach/mach.h>

#include "CommonDef.h"
#include "ItemsBatchReader.h"


void statMemory(int index) {
//  task_basic_info_data_t taskInfo;
//  mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
//  kern_return_t kernReturn = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&taskInfo, &infoCount);
//  if (kernReturn != KERN_SUCCESS) {
//      std::cerr << "Failed to get task info: " << kernReturn << std::endl;
//      assert(0);
//  }
//  assert(taskInfo.resident_size<=11222111000);
//  SharedHeapManager().statMemory(taskInfo.resident_size, index);//（单位：字节）
}

std::unique_ptr<AOPTester<int>> createDAry() {
    return std::make_unique<DAryTester<int>>();
}
std::unique_ptr<AOPTester<int>> createPairing() {
    return std::make_unique<PairingTester<int>>();
}
std::unique_ptr<AOPTester<int>> createAOH() {
    return std::make_unique<AOHeapTester<int>>();
}
std::unique_ptr<AOPTester<int>> createBinomial() {
    return std::make_unique<BinomialTester<int>>();
}
std::unique_ptr<AOPTester<int>> createFibonacci() {
    return std::make_unique<FibonacciTester<int>>();
}
std::unique_ptr<AOPTester<int>> createSkew() {
    return std::make_unique<SkewTester<int>>();
}
using HeapCreator = std::unique_ptr<AOPTester<int>>(*)();

void testAvgHeaps(AOPClockStat& stat,
                  ItemsBatchReader& reader,
                  std::vector<std::string>& heapNameList,
                  clock_t clocks[7]) {
 
  std::vector<HeapCreator> heapVectors;
  
  heapVectors.push_back(createDAry);
  heapVectors.push_back(createAOH);
  heapVectors.push_back(createFibonacci);
  heapVectors.push_back(createBinomial);
  heapVectors.push_back(createSkew);
  heapVectors.push_back(createPairing);
  
  for (int algIndex=0; algIndex < heapVectors.size(); algIndex++) {
    auto heap = heapVectors[algIndex]();
    heapNameList.push_back(heap->name());
    reader.start();
    std::vector<HeapItem> heapItems;

    while(reader.readHeapSeqs(heapItems)) {
      heap->startStatTime();
      for(int index=0; index<heapItems.size(); index++){
        struct HeapItem& item = heapItems[index];
        switch (item.type) {
          case 0: {
//            heap->startStatTime();
            heap->popOne();
//            heap->finishStatTime(clocks[algIndex]);
            break;
          }
          case 1:{
//            heap->startStatTime();
            heap->insert(item.key, item.nodeid);
//            heap->finishStatTime(clocks[algIndex]);
            break;
          }
          case 3: {
//            heap->startStatTime();
            heap->decrease(item.nodeid, item.key);
//            heap->finishStatTime(clocks[algIndex]);
            break;
          }
          case 2: {
//            heap->startStatTime();
            heap->increase(item.nodeid, item.key);
//            heap->finishStatTime(clocks[algIndex]);
            break;
          }
          default:
            assert(0);
            break;
        }
      }
      heap->finishStatTime(clocks[algIndex]);
      heapItems.clear();
    }
    
    heap->startStatTime();
    while(!heap->empty()) {
      heap->popOne();
    }
    heap->finishStatTime(clocks[algIndex]);
    
    HeapMarkType heaptype = heap->getHeapMarkType();
    if(PerMethodStatClocks)
      clocks[algIndex] += SharedHeapManager().getHeapClockSummary(heaptype);
    
    printf(" alg= %s, time= %lld \n", heap->name().c_str(), clocks[algIndex]);

  }
  heapVectors.clear();
}

void saveMethodClocks(std::string db) {
  HeapMethodClockStat methClock;
  WorstExecutionClocksManager& clocksManager = SharedHeapManager();
  for(int mark=0; mark<4; mark++) {
    methClock.write(db, clocksManager.getMethodName((MethodMarkType)mark), "avg",
                    clocksManager.getAvgs((MethodMarkType)mark, HeapSkewType),
                    clocksManager.getAvgs((MethodMarkType)mark, HeapBinomialType),
                    clocksManager.getAvgs((MethodMarkType)mark, HeapFibonacciType),
                    clocksManager.getAvgs((MethodMarkType)mark, HeapAOHType),
                    clocksManager.getAvgs((MethodMarkType)mark, HeapDAryType),
                    clocksManager.getAvgs((MethodMarkType)mark, HeapPairingType));
    methClock.write(db, clocksManager.getMethodName((MethodMarkType)mark), "wst",
                    clocksManager.getWorst((MethodMarkType)mark, HeapSkewType),
                    clocksManager.getWorst((MethodMarkType)mark, HeapBinomialType),
                    clocksManager.getWorst((MethodMarkType)mark, HeapFibonacciType),
                    clocksManager.getWorst((MethodMarkType)mark, HeapAOHType),
                    clocksManager.getWorst((MethodMarkType)mark, HeapDAryType),
                    clocksManager.getWorst((MethodMarkType)mark, HeapPairingType));
  }
  clocksManager.clear();
}

void checkMin(struct HeapItem& item,
              std::unique_ptr<AOPTester<int>>& heap,
              std::unique_ptr<AOPTester<int>>& heap2) {
//  auto heap = heapVectors[0]();
//  auto heap2 = heapVectors[0+1]();
  int lnid = 0;
  int lkey = 0;
  int lnid2 = 0;
  int lkey2 = 0;
  heap->checkDataItem(lnid, lkey);
  heap2->checkDataItem(lnid2, lkey2);//lnid2  int  78726,lkey2  int  -85480    -85480
  assert(lkey==lkey2);
}

void intCheckHeaps(AOPClockStat& stat,
                  ItemsBatchReader& reader,
                  std::vector<std::string>& heapNameList,
                  clock_t clocks[6]) {
  std::vector<HeapCreator> heapVectors;
  heapVectors.push_back(createAOH);
  
  heapVectors.push_back(createDAry);
//  heapVectors.push_back(createPairing);
  
//  heapVectors.push_back(createFibonacci);  //crash
//  heapVectors.push_back(createBinomial);
//  heapVectors.push_back(createSkew);
  
  for (int algIndex=0; algIndex < heapVectors.size()-1; algIndex++) {
    auto heap = heapVectors[algIndex]();
    auto heap2 = heapVectors[algIndex+1]();
    heapNameList.push_back(heap->name());
    
    reader.start();
    heap->startStatTime();
    std::vector<HeapItem> heapItems;
    int prekey=0;
    while(reader.readHeapSeqs(heapItems)) {
      for(int index=0; index<heapItems.size(); index++){
        struct HeapItem& item = heapItems[index];
        
        if(item.nodeid==78726) {//key  int  10989 ;+key  int  89070;-key  int  96655;-key  int  88884
          printf("dary key ");
        }
        
        if(item.nodeid==90482 && item.key==0 && item.type==0) {
          printf("dary key ");
        }
        
        switch (item.type) {
          case 0: {
            if(item.nodeid==78726) {
              printf("dary key ");
            }
            if(!heap2->empty()) checkMin(item, heap, heap2);
            heap->popOne();
            heap2->popOne();
            if(!heap2->empty()) checkMin(item, heap, heap2);
            break;
          }
          case 1:{
            heap->insert(item.key, item.nodeid);
            heap2->insert(item.key, item.nodeid);
            checkMin(item, heap, heap2);
            break;
          }
          case 3: {
            heap->decrease(item.nodeid, item.key);
            heap2->decrease(item.nodeid, item.key);
            checkMin(item, heap, heap2);
            break;
          }
          case 2: {
            heap->increase(item.nodeid, item.key);
            heap2->increase(item.nodeid, item.key);
            checkMin(item, heap, heap2);
            break;
          }
          default:
            assert(0);
            break;
        }
      }
      heapItems.clear();
    }
    while(!heap->empty()) {
      heap->popOne();
      heap2->popOne();
    }
    heap->finishStatTime(clocks[algIndex]);
    HeapMarkType heaptype = heap->getHeapMarkType();
    if(PerMethodStatClocks)
      clocks[algIndex] += SharedHeapManager().getHeapClockSummary(heaptype);
  }
  heapVectors.clear();
}

/*
 最短路径采样
 */
void testDataSet(AOPClockStat& writer,
                 std::string& baseName,
                 std::string dbName,
                 std::string& appendName) {
  
  clock_t clocks[6] = {0, 0, 0, 0, 0, 0};
  std::vector<std::string> heapNameList;
 
  for (int dbIndex=0; dbIndex<AvgBase; dbIndex++) {
    std::vector<struct HeapItem> heapItems;
    std::string path = baseName;
    path.append(dbName).append("_").append(std::to_string(dbIndex)).append(appendName);
//    readHeapSeqs(path, heapItems, HeapItemPopulator, maxNodeID);
    ItemsBatchReader reader(path);
    testAvgHeaps(writer, reader, heapNameList, clocks);
  }
  
  for (int i=0; i < 6; i++) {
//    writer.write(dbName, heapNameList[i], 0, clocks[i]/(SeqBaseSize*AvgBase));
  }
}

std::vector<std::string> getDBName() {
  std::vector<std::string> nameList;
  nameList.push_back("bio-CE-GN");
  nameList.push_back("bio-CE-CX");
  nameList.push_back("bio-DM-CX");
  
  nameList.push_back("bio-human-gene1");
  nameList.push_back("bio-mouse-gene");
  nameList.push_back("bio-WormNet-v3");

  nameList.push_back("datagen-7_5-fb");
  nameList.push_back("datagen-7_6-fb");
  nameList.push_back("datagen-7_7-zf");

  nameList.push_back("USA-road-d.BAY.gr");
  nameList.push_back("USA-road-d.CTR.gr");
  nameList.push_back("USA-road-d.W.gr");
  nameList.push_back("USA-road-d.USA.gr");
  //awk '{$1=""; sub(/^[ \t]+/, ""); print}' USA-road-d.USA.gr.edges > output.edges
  //删除第一列
  return nameList;
}

std::string getPath(std::string name) {
  std::string root = getProjectRoot();
  std::string base = root.append(name);
  return base;
}

void shortestPathSeqTest() {
  std::string baseName = getPath("/heap_seq/");
  std::string appendName = ".csv";
  AOPClockStat writer(AOPClockStatSS);
  std::vector<std::string> nameList = getDBName();
  for (int i=0; i< nameList.size(); i++) {
    std::string dbName = nameList[i];
    testDataSet(writer, baseName, dbName, appendName);
  }
}
/*
 有序样本
 */
void test4AsdDsd(GenSeqsParas paras) {
  std::string appendName = ".edges";
  AOPClockStat writer(AOPClockStatGen);
  std::string baseName = getPath("/gen_seq/");
  
  std::vector<std::string> nameList;
  nameList.push_back("gen_asc");
  nameList.push_back("gen_desc");
  
  WorstExecutionClocksManager& clocksManager = SharedHeapManager();
  clocksManager.clear();
  
  for (int index=0; index<nameList.size(); index++) {
    std::string dbName = nameList[index];
    clock_t clocks[7][6] = {0};
    std::vector<std::string> heapNameList;
    for (int dbIndex=0; dbIndex<5; dbIndex++) {
      std::string dbNameEXT = dbName;
      dbNameEXT.append("_").append(std::to_string((int)std::pow(10, dbIndex)));
      std::string path = baseName;
      path.append(dbNameEXT).append(appendName);
      ItemsBatchReader reader(path);
      heapNameList.clear();
      testAvgHeaps(writer, reader, heapNameList, clocks[dbIndex]);
    }
    for(int indexAlg=0; indexAlg < heapNameList.size(); indexAlg++) {
      writer.writeGen(paras.numOfNodeInBaseSize,
                      paras.mixScale, paras.DeleteUnderInDc,
                      dbName, heapNameList[indexAlg],
                      (int)clocks[0][indexAlg],
                      (int)clocks[1][indexAlg],
                      (int)clocks[2][indexAlg],
                      (int)clocks[3][indexAlg],
                      (int)clocks[4][indexAlg],
                      (int)clocks[5][indexAlg],
                      (int)clocks[6][indexAlg]);
    }
    saveMethodClocks(dbName);
  }
}

/*
 随机样本
 */
void test4Random(GenSeqsParas paras) {
  std::string appendName = ".edges";
  AOPClockStat writer(AOPClockStatGen);
  std::string genBaseName = getPath("/gen_seq/");
  
  std::vector<std::string> dbNameList;
  dbNameList.push_back("gen_rand_1");
  dbNameList.push_back("gen_rand_10");
  dbNameList.push_back("gen_rand_100");
  dbNameList.push_back("gen_rand_1000");
  dbNameList.push_back("gen_rand_10000");
  dbNameList.push_back("gen_rand_100000");
  
  WorstExecutionClocksManager& clocksManager = SharedHeapManager();
  clocksManager.clear();

  clock_t clocks[7][6] = {0};
  std::vector<std::string> heapNameList;
  
  for(int dbIndex=0; dbIndex<dbNameList.size(); dbIndex++) {
    std::string dbName = dbNameList[dbIndex];
    heapNameList.clear();
    std::vector<struct HeapItem> heapItems;
    std::string dbNameEXT = dbName;
    std::string path = genBaseName;
    path.append(dbNameEXT).append(appendName);
    ItemsBatchReader reader(path);
    testAvgHeaps(writer, reader, heapNameList, clocks[dbIndex]);
  }
  
  for(int indexAlg=0; indexAlg < heapNameList.size(); indexAlg++) {
    //每1万记录，消耗的时钟周期
    writer.writeGen(paras.numOfNodeInBaseSize,
                    paras.mixScale, paras.DeleteUnderInDc,
                    "gen_rand", heapNameList[indexAlg],
                    (int)clocks[0][indexAlg],
                    (int)clocks[1][indexAlg],
                    (int)clocks[2][indexAlg],
                    (int)clocks[3][indexAlg],
                    (int)clocks[4][indexAlg],
                    (int)clocks[5][indexAlg],
                    (int)clocks[6][indexAlg]);
  }
  saveMethodClocks("gen_rand");
}

/*
 混合序列
 */
void testMixSeq(std::string name, GenSeqsParas paras) {//mix or mix2
  AOPClockStat writer(AOPClockStatGen);
  
  std::string appendName = ".edges";
  std::string genBaseName = getPath("/gen_seq/"); 
  
  std::vector<std::string> dbNameList;
  for (int size=1; size<=SeqSize1000000; size*=10) {
    std::string mix = name;
    mix.append("_").append(std::to_string(paras.numOfNodeInBaseSize)).append("_").append(std::to_string((int)(paras.mixScale))).append("_").append(std::to_string((int)(paras.DeleteUnderInDc))).append("_").append(std::to_string(size));
    dbNameList.push_back(mix);
  }
  
  WorstExecutionClocksManager& clocksManager = SharedHeapManager();
  clocksManager.clear();
  
  clock_t clocks[7][6] = {0};
  std::vector<std::string> heapNameList;
  for(int dbIndex=0; dbIndex<dbNameList.size(); dbIndex++) {
    std::string dbName = dbNameList[dbIndex];
    heapNameList.clear();
    std::vector<struct HeapItem> heapItems;
    std::string dbNameEXT = dbName;
    std::string path = genBaseName;
    path.append(dbNameEXT).append(appendName);
    ItemsBatchReader reader(path);
    testAvgHeaps(writer, reader, heapNameList, clocks[dbIndex]);
    /*intCheckHeaps*/(writer, reader, heapNameList, clocks[dbIndex]);
  }
  
  for(int indexAlg=0; indexAlg < heapNameList.size(); indexAlg++) {
    //每1万记录，消耗的时钟周期
    writer.writeGen(paras.numOfNodeInBaseSize,
                    paras.mixScale,
                    paras.DeleteUnderInDc,
                    name, heapNameList[indexAlg],
                    (int)clocks[0][indexAlg],
                    (int)clocks[1][indexAlg],
                    (int)clocks[2][indexAlg], 
                    (int)clocks[3][indexAlg],
                    (int)clocks[4][indexAlg],
                    (int)clocks[5][indexAlg],
                    (int)clocks[6][indexAlg]);
  }
  saveMethodClocks(name);
}


/*
 统计 数据库 信息
 */

void statDBInfo4DB(AOPHeapStat& stat, std::string baseName,
                   std::string dbName, std::string appendName) {
  int Items = 0, changes = 0, eleInHeap = 0;
  AOPMaxStat maxStat;
  
  for(int index = 0; index < AvgBase; index++) {
    std::vector<struct HeapItem> ItemsList;
    std::string path = baseName;
    path.append(dbName).append("_").append(std::to_string(index)).append(appendName);
//    readHeapSeqs(path, ItemsList, HeapItemPopulator);
    ItemsBatchReader reader(path);
    auto heap = AOHeapTester<int>();
    while (reader.readHeapSeqs(ItemsList)) {
      Items += ItemsList.size();
      for(int index=0; index < ItemsList.size(); index++){
        struct HeapItem& item = ItemsList[index];
        switch (item.type) {
          case 0://new
            heap.popOne();
            eleInHeap--;
            break;
          case 1://new
            heap.insert(item.key, item.nodeid);
            eleInHeap++;
            maxStat.update(eleInHeap);
            break;
          case 3://de
            changes = changes + 1;
            heap.decrease(item.nodeid, item.key);
            break;
          case 2://in
            changes = changes + 1;
            heap.increase(item.nodeid, item.key);
            break;
          default:
            assert(0);
            break;
        }
      }
      ItemsList.clear();
    }
    maxStat.push();
  }
  stat.write(dbName, Items/AvgBase, changes/AvgBase, maxStat.get()/AvgBase, AvgBase);
}

void statDBInfo() {
  std::string baseName = getPath("/heap_seq/");
  std::string appendName = ".csv";
  AOPHeapStat stater;
  std::vector<std::string> nameList = getDBName();
  for (int i=0; i< nameList.size(); i++) {
    std::string dbname = nameList[i];
    statDBInfo4DB(stater, baseName, dbname, appendName);
  }
}


#endif /* AOPSPSeqTester_h */


