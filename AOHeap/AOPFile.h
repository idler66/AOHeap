//
//  AOPFile.h
//  Adjacent ordered heap
//
//  Created by 嘟嘟 on 2024/12/25.
//

#ifndef AOPFile_h
#define AOPFile_h

#include <time.h>
#include <cstdlib>
#include <fstream>
#include <streambuf>
#include <cmath>
#include <thread>
#include <stdlib.h>

enum AOPStatType : unsigned char {
  AOPStatNodeAvg = 1,
  AOPStatOPsAvg = 2
};

static const AOPStatType StatType = AOPStatNodeAvg;

enum AOPOpType : unsigned char {
  AOPOpTypeIns = 1,//insert into the heap
  AOPOpTypeInc = 2,       //increase key
  AOPOpTypeDec = 4,       //decrease key
  AOPOpTypePop = 8        //pop some element
};

std::string getOpFlags(unsigned char ops) {
  std::string name = "";
  if (ops&AOPOpTypeIns) {
    name +="Ins";
  }
  if (ops&AOPOpTypeInc) {
    name +="Inc";
  }
  if (ops&AOPOpTypeDec) {
    name +="dec";
  }
  if (ops&AOPOpTypePop) {
    name +="pop";
  }
  return name;
}

void closeAOPFile(std::ofstream& file) {
  file.close();
}

std::ofstream openAOPFile(unsigned char ops, std::string name) {
  std::ofstream file;
  std::string base = "/Users/unknown100name/Desktop/Test4/gen/";
  std::string ostr = ".csv";
  std::string path = base + name + ostr;
  if(!std::filesystem::exists(path)) {
    file.open(path.c_str(), std::ios::out | std::ios::trunc);
    file<< "method" << ","    //pairing, fibonacci, dary, binomial, skew, AOPacent, binary
        << "nodes" << ","     //AOPNodeNum
        << "changes" << ","     //AOPKeyChangeNum
        << "batchins" << ","   //AOPBatchInsSize
        << "ops" << ","     //operation name, insert, key increase, key decrease or pop
        << "clocks" << ","
        << "cpn" << ","
        << "date" << ","     //record insertion time
        << std::endl;
  } else {
    file.open(path.c_str(), std::ios::out | std::ios::app);
  }
  return file;
}

void writeAOP(std::string methodname, std::ofstream& file,
              unsigned char ops, clock_t clocks) {
  std::string opsName = getOpFlags(ops);
  int counter = 0;
  if (StatType == AOPStatNodeAvg) {
    counter = 1;
  } else {
    if (ops&AOPOpTypeIns) {
      counter++;
    }
    if (ops&AOPOpTypeInc) {
      counter+=AOPKeyChangeNum;
    }
    if (ops&AOPOpTypeDec) {
      counter+=AOPKeyChangeNum;
    }
    if (ops&AOPOpTypePop) {
      counter++;
    }
  }
  file<< methodname << ","
      << AOPNodeNum << "," << AOPKeyChangeNum << ","
      << AOPBatchInsSize << ","
      << opsName << "," << clocks << ","
      << clocks/((float)AOPNodeNum*counter) << "," << clock() << "," << std::endl;
}

template <typename KeyType>
void printHeapResult(std::ofstream& file, unsigned char ops,
                     AOPTester<KeyType>& tester, std::string method) {
  clock_t clock = 0;
  if (ops&AOPOpTypeIns) {
    clock += tester.insertTime;
  }
  if (ops&AOPOpTypeInc) {
    clock += tester.increaseTime;
  }
  if (ops&AOPOpTypeDec) {
    clock += tester.decreaseTime;
  }
  if (ops&AOPOpTypePop) {
    clock += tester.popTime;
  }
  writeAOP(method, file, ops, clock);
}

template <typename KeyType>
void printTimes(std::ofstream& file, unsigned char ops,
               PairingTester<KeyType>& pairing,
                FibonacciTester<KeyType>& fibonacci,
               DAryTester<KeyType>& dary,
                BinomialTester<KeyType>& binomial,
               SkewTester<KeyType>& skew,
                AOHeapTester<KeyType>& AOPacent,
               BinaryHeapTester<KeyType>& binary) {
  printHeapResult<KeyType>(file, ops, pairing, "pairing");
  printHeapResult<KeyType>(file, ops, fibonacci, "fibonacci");
  printHeapResult<KeyType>(file, ops, dary, "dary");
  printHeapResult<KeyType>(file, ops, binomial, "binomial");
  printHeapResult<KeyType>(file, ops, skew, "skew");
  printHeapResult<KeyType>(file, ops, AOPacent, "AOPacent");
  printHeapResult<KeyType>(file, ops, binary, "binary");
}

#endif /* AOPFile_h */
