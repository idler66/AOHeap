//
//  AOPFileReader.h
//  Test
//
//  Created by 嘟嘟 on 2025/2/27.
//

#ifndef AOPFileReader_h
#define AOPFileReader_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "CommonDef.h"

class AOPMaxStat {
  int max;
  int sum;
  int pushCount;
public:
  AOPMaxStat()  {
    max = 0;
    sum = 0;
    pushCount = 0;
  };
  void update(int _max) {
    if(_max > max) {
      max = _max;
    }
  }
  int get() {
    return sum / pushCount;
  }
  void push() {
    pushCount++;
    sum += max;
    max = 0;
  }
};

/*
 用于统计，一个数据库中记录 作用于堆时，堆的统计信息。
 */

enum AOPClockStatType : unsigned char {
  AOPClockStatGen = 1,//随机、升序、降序
  AOPClockStatSS = 2,//最短路径生成
};

class AOPClockStat {
  std::ofstream file;
  AOPClockStatType statType;
public:
  std::string getFileName() {
    std::string statName = "ss_clocks";
    if(statType == AOPClockStatGen) {
      statName = "gen_clocks";
    }
    return statName;
  }
  AOPClockStat(AOPClockStatType _statType) {
    statType = _statType;
    std::string root = getProjectRoot();
    std::string base = root.append("/results/");
    std::string append = ".csv";
    std::string path = base.append(getFileName()).append(append);
    file.open(path.c_str(), std::ios::out | std::ios::app);//trunc
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
//        assert(0);
    }
    file  << "type" << ","    << "alg"  << ","
          << "1"    << ","    << "10" << ","
          << "100"  << ","    << "1000" << ","
          << "10000"<< ","    << "100000"<< ","  << "1000000"<< ","
          << "base node size" << ","
          << "mix scale" << ","    << "incdec/del"  << ","
          << std::endl;
  }
  ~AOPClockStat() {
    file.close();
  }
  
  /*
   type:生产数据的方式 asc dsc rand
   */
  void writeGen(int baseNodeSize,
                int mixScale,   int incDecOnDel,
                std::string type,  std::string alg,
                clock_t one,        clock_t one10,
                clock_t one100,     clock_t one1000,
                clock_t one10000,   clock_t one100000, clock_t one1000000) {
    file  << type << ","          << alg << ","
          << one << ","           << one10 << ","
          << one100 << ","        << one1000 << ","
          << one10000 << ","      << one100000 << "," 
           << one1000000 << ","   << baseNodeSize << ","
          << mixScale << ","      << incDecOnDel << "," << std::endl;
  }
  
};

/*
 堆方法 最差 均值
 */
class HeapMethodClockStat {
  std::ofstream file;
public:
  HeapMethodClockStat() {
    std::string root = getProjectRoot();
    std::string base = root.append("/results/");
    std::string append = ".csv";
    std::string path = base.append("method").append(append);
    file.open(path.c_str(), std::ios::out | std::ios::app);//trunc
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
//        assert(0);
    }
    file  << "db" << "," << "method" << "," << "type" << ","
          << "Skew" << "," << "Binomial" << ","<< "Fibonacci" << ","
    << "AOH" << "," << "DAry" << "," << "Pairing" << ","
          << std::endl;
  }
  ~HeapMethodClockStat() {
    file.close();
  }
  //type worst or avg
  void write(std::string db, std::string method, std::string type,
             float Skew, float Binomial, float Fibonacci,
             float AOH, float DAry, float Pairing) {
    file  << db << "," << method << "," << type << ","
          << Skew << "," << Binomial << "," << Fibonacci << ","
    << AOH << ","  << DAry << "," << Pairing << ","
          << std::endl;
  }
};

/*
 用于统计，一个数据库中记录 作用于堆时，堆的统计信息。
 */
class AOPHeapStat {
  std::ofstream file;
public:
  AOPHeapStat() {
    std::string root = getProjectRoot();
    std::string base = root.append("/results/");
    std::string name = "dbstat";
    std::string append = ".csv";
    std::string path = base.append(name).append(append);
    bool exist = false;
    if(file.good()) {
      exist = true;
    }
    file.open(path.c_str(), std::ios::out | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
//        assert(0);
    }
    if(!exist) {
      file  << "db-name"      << ","    << "db-item-size" << ","
            << "change-num"   << ","    << "max-heap-size" << ","
            << "sampling-num"      << ","<< std::endl;
    }
  }
  ~AOPHeapStat() {
    file.close();
  }
  
  /*
   dbname 数据库名称；
   dbitemsize 数据库中记录数量；
   changenum 键值 增加、降低 数量；
   maxheapsize 当在这个数据库上 运行堆算法时，堆中元素最大数量
   */
  void write(std::string dbName,       int dbItemSize,
             int changeNum,  int maxHeapSize,
             int samplingNum) {
    file  << dbName << ","      << dbItemSize << ","
          << changeNum << ","   << maxHeapSize << ","
          << samplingNum << ","
          << std::endl;
  }
};

//template <typename AOPDataPopulator>
//void readHeapSeqs(const std::string& filePath,
//                  std::vector<HeapItem>& data,
//                  AOPDataPopulator populator) {
//    std::ifstream file(filePath);
//    std::string line;
//    if (!file.is_open()) {
//        std::cerr << "Error opening file: " << filePath << std::endl;
//        assert(0);
//    }
//  int counter = 0;
//    while (std::getline(file, line) && counter++ < 100000000) {
//      std::stringstream ss(line);
//      HeapItem item;
//      populator(item, line, ss);
//      data.push_back(item);
//    }
//    file.close();
//}

#endif /* AOPFileReader_h */
