//
//  NaiveSeqsGenerator.h
//  Test
//
//  Created by 嘟嘟 on 2025/3/14.
//

#ifndef NaiveSeqsGenerator_h
#define NaiveSeqsGenerator_h


#include <utility>
#include <tuple>

#include <iostream>
#include <string_view>

#include <vector>
#include <unordered_map>

#include "CommonDef.h"


class NaiveSeqsGenerator {
  int numberOfNode;
  std::vector<int> seqs;
  std::ofstream ofile;
  
public:
  NaiveSeqsGenerator() {
    srand(static_cast<unsigned int>(time(0)));
  }
  
  void startWrite(std::string name, int size) {
    std::string root = getProjectRoot();
    std::string base = root.append("/naive_seq/");
    std::string append = ".edges";
    std::string path = base.append(name).append(append);
    ofile.open(path.c_str(), std::ios::out | std::ios::trunc);
    if (!ofile.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        assert(0);
    }
  }
  void write() {
    for (int index=0; index < seqs.size(); index++) {
      int& key = seqs[index];
      ofile <<index  <<","   <<key   <<","   <<1  <<"," <<std::endl;
    }
    seqs.clear();
  }
  void finishWrite() {
    ofile.close();
  }
  
  void generateRandomSeqs(SeqSize _randomSize, std::string name) {
    numberOfNode = _randomSize * SeqBaseSize;
    seqs.clear();
    int maxRange = numberOfNode-1>RAND_MAX?RAND_MAX:(int)(numberOfNode-1);
    for (int index = 0; index < numberOfNode; index++) {
      int key = randInRange(maxRange);
      seqs.push_back(key);
      if (seqs.size()>BatchSize) {
        write();
      }
    }
    write();
  }
  
  //降序序列
  void generateDescSeqs(SeqSize _randomSize, std::string name) {
    numberOfNode = _randomSize * SeqBaseSize;
    seqs.clear();
    for (int index = numberOfNode-1; index > -1 ; index--) {
      seqs.push_back(index);
      if (seqs.size()>BatchSize) {
        write();
      }
    }
    write();
  }
  
  //升序序列
  void generateAscSeqs(SeqSize _randomSize, std::string name) {
    numberOfNode = _randomSize * SeqBaseSize;
    seqs.clear();
    for (int index = 0; index < numberOfNode; index++) {
      seqs.push_back(index);
      if (seqs.size()>BatchSize) {
        write();
      }
    }
    write();
  }
    
  void generateAscDsc() {
    generateAscSeqs(SeqSize1, "gen_asc_1");
    generateAscSeqs(SeqSize10, "gen_asc_10");
    generateAscSeqs(SeqSize100, "gen_asc_100");
    generateAscSeqs(SeqSize1000, "gen_asc_1000");
    generateAscSeqs(SeqSize10000, "gen_asc_10000");
    generateAscSeqs(SeqSize100000, "gen_asc_100000");

    generateDescSeqs(SeqSize1, "gen_desc_1");
    generateDescSeqs(SeqSize10, "gen_desc_10");
    generateDescSeqs(SeqSize100, "gen_desc_100");
    generateDescSeqs(SeqSize1000, "gen_desc_1000");
    generateDescSeqs(SeqSize10000, "gen_desc_10000");
    generateDescSeqs(SeqSize100000, "gen_desc_100000");
  }
  
  void generateRand() {
    std::vector<std::string> randNameList;
    randNameList.push_back("gen_rand_1");
    randNameList.push_back("gen_rand_10");
    randNameList.push_back("gen_rand_100");
    randNameList.push_back("gen_rand_1000");
    randNameList.push_back("gen_rand_10000");
    randNameList.push_back("gen_rand_100000");
    std::vector<SeqSize> randGenSizes;
    randGenSizes.push_back(SeqSize1);
    randGenSizes.push_back(SeqSize10);
    randGenSizes.push_back(SeqSize100);
    randGenSizes.push_back(SeqSize1000);
    randGenSizes.push_back(SeqSize10000);
    randGenSizes.push_back(SeqSize100000);
    for (int scale=0; scale<randNameList.size(); scale++) {
      std::string name = randNameList[scale];
      generateRandomSeqs(randGenSizes[scale], name);
    }
  }
};

#endif /* NaiveSeqsGenerator_h */
