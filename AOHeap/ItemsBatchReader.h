//
//  ItemsBatchReader.h
//  Test
//
//  Created by 嘟嘟 on 2025/3/10.
//

#ifndef ItemsBatchReader_h
#define ItemsBatchReader_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "CommonDef.h"

void HeapItemPopulator(HeapItem& item, std::string& line, std::stringstream& ss) {
  std::string cell;
  std::getline(ss, cell, ',');
  size_t pos;
  int nodeid = std::stoi(cell, &pos);
  item.nodeid = nodeid;
  std::getline(ss, cell, ',');
  int key = std::stoi(cell, &pos);
  item.key = key;
  std::getline(ss, cell, ',');
  int type = std::stoi(cell, &pos);
  item.type = type;
}

class ItemsBatchReader {
public:
  std::string filePath;
  std::ifstream infile;
  
  ItemsBatchReader(std::string path) {
    filePath = path;
  }
  ~ItemsBatchReader() {
    if (infile.is_open()) {
      infile.close();
    }
  }
  
  void start() {
    infile.open(filePath, std::ios::in);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
      std::cerr << "Error code: " << strerror(errno) << std::endl; // 输出错误信息

        assert(0);
    }
  }
  
  bool readHeapSeqs(std::vector<HeapItem>& heapItems) {
    assert(heapItems.size()==0);
    int index = 0;
    std::string line;
    while (std::getline(infile, line)) {
      std::stringstream ss(line);
      index++;
      HeapItem item;
      HeapItemPopulator(item, line, ss);
      heapItems.push_back(item);
      if(index == BatchSize) break;
    }
    if(index > 0) return true;
    infile.close();
    return false;
  }
  
};

#endif /* ItemsBatchReader_h */
