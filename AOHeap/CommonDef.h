//
//  CommonDef.h
//  Test
//
//  Created by 嘟嘟 on 2025/3/5.
//

#ifndef CommonDef_h
#define CommonDef_h

#define AvgBase 1  //重复频率 正式值为10, 用于随机生成 非混合序列

#define BatchSize 10000000

#define SeqBaseSize 3000  //最大3亿节点

enum SeqSize : unsigned int {
  SeqSize1 = 1,
  SeqSize10 = 10,
  SeqSize100 = 100,
  SeqSize1000 = 1000,
  SeqSize10000 = 10000,//万;
  SeqSize100000 = 100000,//10万;
  SeqSize1000000 = 1000000,//100万;
};

struct GenSeqsParas {
  int numOfNodeInBaseSize;
  int mixScale;  //后两个越大，其
  int DeleteUnderInDc;
};


struct HeapItem {
  int nodeid;
  int key;
  int type; //0 pop, 1 new, 2 increase key dlt, 3 decrease key dlt,
};
 
int randInRange(int max) {
  return  ((int)(abs(rand()) % (max + 1)));
}

enum MethodMarkType : unsigned int {
  MethodMarkIncreaseType = 0,
  MethodMarkDecreaseType = 1,
  MethodMarkPopType = 2,
  MethodMarkInsertType = 3,
};

enum HeapMarkType : unsigned int {
  HeapFibonacciType = 0,
  HeapPairingType = 4,
  HeapDAryType = 8,
  HeapSkewType = 12,
  HeapBinomialType = 16,
  HeapAOHType = 20, //28
};


class WorstExecutionClocks {
  clock_t WorstClocks;
  clock_t SumClocks;
  clock_t ExecutionTimes;
public:
  WorstExecutionClocks() {
    WorstClocks = 0;
    SumClocks = 0;
    ExecutionTimes = 0;
  }
  void update(int _WorstClocks) {
    if(_WorstClocks > WorstClocks) {
      WorstClocks = _WorstClocks;
    }
    ExecutionTimes++;
    SumClocks += _WorstClocks;
  }
  clock_t getWorst() {
    return WorstClocks;
  }
  float getAvg() {
    auto avg = (float)SumClocks/(ExecutionTimes>0?ExecutionTimes:1);
    return avg;
  }
  clock_t getSummary() {
    return SumClocks;
  }
};

class WorstExecutionClocksManager {
  std::vector<WorstExecutionClocks> ClocksList;
  long ru_maxrss[6];
public:
  WorstExecutionClocksManager() {
    for (int ctype = 0; ctype < 24; ctype++) {
      WorstExecutionClocks clocks;
      ClocksList.push_back(clocks);
    }
    memset(ru_maxrss, 0, 6);
  }
  ~WorstExecutionClocksManager() {
    
  }
  void update(clock_t _clocks, MethodMarkType methodtype, HeapMarkType heaptype) {
    auto& wexec = ClocksList[heaptype+methodtype];
    wexec.update(_clocks);
  }
  clock_t getWorst(MethodMarkType methodtype, HeapMarkType heaptype) {
    auto& wexec = ClocksList[heaptype+methodtype];
    return wexec.getWorst();
  }
  float getAvgs(MethodMarkType methodtype, HeapMarkType heaptype) {
    auto& wexec = ClocksList[heaptype+methodtype];
    return wexec.getAvg();
  }
  clock_t getHeapClockSummary(HeapMarkType heaptype) {
    return ClocksList[heaptype+MethodMarkPopType].getSummary() +
            ClocksList[heaptype+MethodMarkIncreaseType].getSummary() +
            ClocksList[heaptype+MethodMarkDecreaseType].getSummary() +
            ClocksList[heaptype+MethodMarkInsertType].getSummary();
  }
  void clear() {
    ClocksList.clear();
    for (int ctype = 0; ctype < 24; ctype++) {
      WorstExecutionClocks clocks;
      ClocksList.push_back(clocks);
    }
    memset(ru_maxrss, 0, 6);
  }
  
  std::string getMethodName(MethodMarkType method) {
    switch (method) {
      case MethodMarkIncreaseType:
        return "Incr";
        break;
      case MethodMarkDecreaseType:
        return "Decr";
        break;
      case MethodMarkPopType:
        return "pop";
        break;
      case MethodMarkInsertType:
        return "insert";
        break;
      default:
        break;
    }
    return "err";
  }
  
  void statMemory(long m, int index) {
    if(ru_maxrss[index] < m) ru_maxrss[index] = m;
  }
  
  long getMemory(int index) {
    return ru_maxrss[index];
  }
};

static WorstExecutionClocksManager sharedClocksHeapManager;
static WorstExecutionClocksManager& SharedHeapManager() {
  return sharedClocksHeapManager;
}

std::string getProjectRoot() {
  std::filesystem::path filePath = __FILE__;
  std::filesystem::path projectRoot = filePath.parent_path().parent_path(); // 根据项目结构调整
  return projectRoot.string();
}

#endif /* CommonDef_h */
