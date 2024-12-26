//
//  AOPTestWrapper.h
//  Adjacent ordered heap
//
//  Created by 嘟嘟 on 2024/12/26.
//

#ifndef AOPTestWrapper_h
#define AOPTestWrapper_h

#include "AOPTester.h"
#include "AOPFile.h"

/*
 4 single test
 */
template <typename KeyType, typename DataType>
void insertTest (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  
  std::string name = getOpFlags(AOPOpTypeIns);
  std::ofstream file = openAOPFile(AOPOpTypeIns, name);
  printTimes(file, AOPOpTypeIns, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void increaseTest (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  pairing.AOPTester::increaseAll();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  fibonacci.AOPTester::increaseAll();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  dary.AOPTester::increaseAll();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  binomial.AOPTester::increaseAll();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  skew.AOPTester::increaseAll();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  AOPacent.AOPTester::increaseAll();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  binary.AOPTester::increaseAll();
  
  std::string name = getOpFlags(AOPOpTypeInc);
  std::ofstream file = openAOPFile(AOPOpTypeInc, name);
  printTimes(file, AOPOpTypeInc, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void decreaseTest (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  pairing.AOPTester::decreaseAll();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  fibonacci.AOPTester::decreaseAll();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  dary.AOPTester::decreaseAll();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  binomial.AOPTester::decreaseAll();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  skew.AOPTester::decreaseAll();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  AOPacent.AOPTester::decreaseAll();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  binary.AOPTester::decreaseAll();
  
  std::string name = getOpFlags(AOPOpTypeDec);
  std::ofstream file = openAOPFile(AOPOpTypeDec, name);
  printTimes(file, AOPOpTypeDec, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void popTest (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  pairing.popAll();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  fibonacci.popAll();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  dary.popAll();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  binomial.AOPTester::popAll();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  skew.AOPTester::popAll();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  AOPacent.AOPTester::popAll();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  binary.AOPTester::popAll();
  
  std::string name = getOpFlags(AOPOpTypePop);
  std::ofstream file = openAOPFile(AOPOpTypePop, name);
  printTimes(file, AOPOpTypePop, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

/*
 3 someone Intertwined with Pop test
 */
template <typename KeyType, typename DataType>
void insertIntertwinedPop (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.insertIntertwinedPop();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.insertIntertwinedPop();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.insertIntertwinedPop();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.insertIntertwinedPop();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.insertIntertwinedPop();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.insertIntertwinedPop();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.insertIntertwinedPop();
  
  std::string name = getOpFlags(AOPOpTypeIns|AOPOpTypePop);
  std::ofstream file = openAOPFile(AOPOpTypeIns|AOPOpTypePop, name);
  printTimes(file, AOPOpTypeIns|AOPOpTypePop, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void decreaseIntertwinedPop (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  pairing.decreaseIntertwinedPop();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  fibonacci.decreaseIntertwinedPop();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  dary.decreaseIntertwinedPop();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  binomial.decreaseIntertwinedPop();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  skew.decreaseIntertwinedPop();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  AOPacent.decreaseIntertwinedPop();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  binary.decreaseIntertwinedPop();
  
  std::string name = getOpFlags(AOPOpTypeDec|AOPOpTypePop);
  std::ofstream file = openAOPFile(AOPOpTypeDec|AOPOpTypePop, name);
  printTimes(file, AOPOpTypeDec|AOPOpTypePop, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void increaseIntertwinedPop (AOPHeapKeyType& keytype) {
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  AOPacent.increaseIntertwinedPop();
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  pairing.increaseIntertwinedPop();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  fibonacci.increaseIntertwinedPop();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  dary.increaseIntertwinedPop();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  binomial.increaseIntertwinedPop();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  skew.increaseIntertwinedPop();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  binary.increaseIntertwinedPop();
  
  std::string name = getOpFlags(AOPOpTypeInc|AOPOpTypePop);
  std::ofstream file = openAOPFile(AOPOpTypeInc|AOPOpTypePop, name);
  printTimes(file, AOPOpTypeInc|AOPOpTypePop, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

/*
 some Intertwined with increase and pop
 */
template <typename KeyType, typename DataType>
void decIntertwinedWithIncAndPop (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.AOPTester::insertAll();
  pairing.decIntertwinedWithIncAndPop();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.AOPTester::insertAll();
  fibonacci.decIntertwinedWithIncAndPop();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.AOPTester::insertAll();
  dary.decIntertwinedWithIncAndPop();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.AOPTester::insertAll();
  binomial.decIntertwinedWithIncAndPop();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.AOPTester::insertAll();
  skew.decIntertwinedWithIncAndPop();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.AOPTester::insertAll();
  AOPacent.decIntertwinedWithIncAndPop();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.AOPTester::insertAll();
  binary.decIntertwinedWithIncAndPop();
  
  unsigned char ops = AOPOpTypePop | AOPOpTypeInc | AOPOpTypeDec;
  std::string name = getOpFlags(ops);
  std::ofstream file = openAOPFile(ops, name);
  printTimes(file, ops, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void insertIntertwinedWithIncAndPop (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.insertIntertwinedWithIncAndPop();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.insertIntertwinedWithIncAndPop();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.insertIntertwinedWithIncAndPop();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.insertIntertwinedWithIncAndPop();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.insertIntertwinedWithIncAndPop();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.insertIntertwinedWithIncAndPop();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binary.insertIntertwinedWithIncAndPop();

  unsigned char ops = AOPOpTypePop | AOPOpTypeInc | AOPOpTypeIns;
  std::string name = getOpFlags(ops);
  std::ofstream file = openAOPFile(ops, name);
  printTimes(file, ops, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

template <typename KeyType, typename DataType>
void insertAndDecIntertwinedWithIncAndPop (AOPHeapKeyType& keytype) {
  PairingTester<KeyType, DataType> pairing(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  pairing.insertAndDecIntertwinedWithIncAndPop();
  FibonacciTester<KeyType, DataType> fibonacci(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  fibonacci.insertAndDecIntertwinedWithIncAndPop();
  DAryTester<KeyType, DataType> dary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  dary.insertAndDecIntertwinedWithIncAndPop();
  BinomialTester<KeyType, DataType> binomial(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  binomial.insertAndDecIntertwinedWithIncAndPop();
  SkewTester<KeyType, DataType> skew(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  skew.insertAndDecIntertwinedWithIncAndPop();
  AOHeapTester<KeyType, DataType> AOPacent(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
  AOPacent.insertAndDecIntertwinedWithIncAndPop();
  BinaryHeapTester<KeyType, DataType> binary(AOPNodeNum, AOPKeyChangeNum, keytype, AOPRandSeed, AOPBandWidth);
//  binary.insertAndDecIntertwinedWithIncAndPop();
  
  unsigned char ops = AOPOpTypePop | AOPOpTypeDec | AOPOpTypeInc | AOPOpTypeIns;
  std::string name = getOpFlags(ops);
  std::ofstream file = openAOPFile(ops, name);
  printTimes(file, ops, pairing, fibonacci, dary, binomial, skew, AOPacent, binary);
  closeAOPFile(file);
}

static const long int AOPNodeNumUpper = 1000000;//100
static const long int AOPNodeStep = 10;

static const long int AOPKeyChangeNumUpper = 40;//0 1 10 20 40
static const long int AOPKeyChangeStep = 2;

template <typename KeyType, typename DataType>
void overallTestIns (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    insertTest<KeyType, DataType>(keytype);
  }
}

template <typename KeyType, typename DataType>
void overallTestInc (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    for(AOPKeyChangeNum=1; AOPKeyChangeNum<=AOPKeyChangeNumUpper;) {
      increaseTest<KeyType, DataType>(keytype);
      if(AOPKeyChangeNum<10) {
        AOPKeyChangeNum = 10;
      } else {
        AOPKeyChangeNum *= AOPKeyChangeStep;
      }
    }
  }
}

template <typename KeyType, typename DataType>
void overallTestDec (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    for(AOPKeyChangeNum=1; AOPKeyChangeNum<=AOPKeyChangeNumUpper;) {
      decreaseTest<KeyType, DataType>(keytype);
      if(AOPKeyChangeNum<10) {
        AOPKeyChangeNum = 10;
      } else {
        AOPKeyChangeNum *= AOPKeyChangeStep;
      }
    }
  }
}

template <typename KeyType, typename DataType>
void overallTestPop (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    popTest<KeyType, DataType>(keytype);
  }
}

/*
 Intertwined
 */

template <typename KeyType, typename DataType>
void overallIncPop (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    for(AOPKeyChangeNum=1; AOPKeyChangeNum<=AOPKeyChangeNumUpper;) {
      increaseIntertwinedPop<KeyType, DataType>(keytype);
      if(AOPKeyChangeNum<10) {
        AOPKeyChangeNum = 10;
      } else {
        AOPKeyChangeNum *= 2;
      }
    }
  }
}

template <typename KeyType, typename DataType>
void overallDecPop (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    for(AOPKeyChangeNum=1; AOPKeyChangeNum<=AOPKeyChangeNumUpper;) {
      decreaseIntertwinedPop<KeyType, DataType>(keytype);
      if(AOPKeyChangeNum<10) {
        AOPKeyChangeNum = 10;
      } else {
        AOPKeyChangeNum *= AOPKeyChangeStep;
      }
    }
  }
}

template <typename KeyType, typename DataType>
void overallTestAll (AOPHeapKeyType& keytype) {
  for(AOPNodeNum=100; AOPNodeNum<=AOPNodeNumUpper; AOPNodeNum*=AOPNodeStep) {
    for(AOPKeyChangeNum=1; AOPKeyChangeNum<=AOPKeyChangeNumUpper;) {
      insertAndDecIntertwinedWithIncAndPop<KeyType, DataType>(keytype);
      if(AOPKeyChangeNum<10) {
        AOPKeyChangeNum = 10;
      } else {
        AOPKeyChangeNum *= AOPKeyChangeStep;
      }
    }
  }
}

#endif /* AOPTestWrapper_h */
