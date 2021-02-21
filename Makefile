#  Copyright (c) Feb 18, 2021 Steven Varga, Toronto,ON Canada

CXXFLAGS =  -std=c++17 -O3  -I./include 
LIBS = -lprofiler -lhdf5  -lz -ldl -lm 

test: rndt 
	./rndt --graphviz -o tree.gv -d 70 -g 46
	dot -Tsvg tree.gv -o tree.svg
	xviewer -w  tree.svg
	#./rndt --hdf5 -o tree.h5 -d 70 -g 46

%.o : %.cpp 
	$(CXX) -o $@  $(CPPFLAGS) $(CXXFLAGS) -c $^

rndt: rndt.o
	$(CXX) $^ $(LIBS) -o $@	

clean:
	@$(RM) *.o *.h5 $(apps) rndt *.gv *.svg

dist-clean: clean 
.PHONY: test

