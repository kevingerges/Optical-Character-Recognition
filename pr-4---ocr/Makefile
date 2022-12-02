CXX=g++
CXXFLAGS=-g -Wall -std=c++11 -I /usr/include/gtest/
#DEFS=-DUSE_SOL_CLASSNAME
GTEST_LD_FLAGS=-lgtest -lgtest_main -lpthread
TARGETS=ocr
all: $(TARGETS)

ocr: bmplib.o numimg.o digitblob.o bigint.o ocr.cpp 
	$(CXX) $(CXXFLAGS) $(DEFS) $^ -o $@ 

bmplib.o: bmplib.cpp bmplib.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

numimg.o: numimg.cpp numimg.h digitblob.h
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@

digitblob.o: digitblob.cpp digitblob.h
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@


bigint.o: bigint.cpp bigint.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -c $< -o $@


clean:
	rm -rf *.o $(TARGETS)