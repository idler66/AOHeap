//
//  main.cpp
//  Test
//
//  Created by reexpound on 2024/1/26.
//
//
 
#include <math.h>

#include <utility>
#include <tuple>
#include <iostream>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "AOPSPSeqTester.h"
#include "NaiveSeqsGenerator.h"
#include "MixSeqsGenerator.h"

#include <filesystem>

//#include <benchmark/benchmark.h>

void removeFiles() {
  std::string root = getProjectRoot();
  std::string dirPath = root.append("/gen_seq/");
  for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
    if (std::filesystem::is_regular_file(entry.status())) {
      std::filesystem::remove(entry.path());
    }
  }
}

int main() {
  MixSeqsGenerator mixGenerator;
  GenSeqsParas paras;
  paras.numOfNodeInBaseSize = 1;

  paras.mixScale = 1;//2亿+记录
  paras.DeleteUnderInDc = 500;
  removeFiles();
  mixGenerator.generate(paras);
  printf(" 2000-1-500 \n");
  testMixSeq("mix", paras);
  testMixSeq("mix", paras);
//  
  paras.mixScale = 2;//20亿+记录
  paras.DeleteUnderInDc = 500;
  mixGenerator.generate(paras);
  printf(" 2000-2-500  \n");
  testMixSeq("mix", paras);
  testMixSeq("mix", paras);
  
  paras.mixScale = 1;
  paras.DeleteUnderInDc = 2;
  mixGenerator.generate(paras);
  printf(" 2000-1-2  \n");
  testMixSeq("mix", paras);
  testMixSeq("mix", paras);
  
  paras.mixScale = 2;
  paras.DeleteUnderInDc = 2;
  mixGenerator.generate(paras);
  printf(" 2000-2-2  \n");
  testMixSeq("mix", paras);
  testMixSeq("mix", paras);
  
////  statDBInfo();
////  shortestPathSeqTest();
  ///
   return 0;
}
