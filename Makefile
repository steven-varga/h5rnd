#  Copyright (c) Feb 18, 2021 Steven Varga, Toronto,ON Canada

CXXFLAGS =  -std=c++17 -O3  -I./include 
LIBS = -lhdf5  -lz -ldl -lm 

test: h5rnd
	#./h5rnd --graphviz -o tree.gv -d 30 -g 11
	#dot -Tsvg tree.gv -o tree.svg
	#xviewer -wf  tree.svg&
	./h5rnd --graphviz --hdf5 -o tree.h5 -d 13 -g 7
	dot -Tsvg tree.gv -o tree.svg
	xviewer -wf  tree.svg&
%.o : %.cpp 
	$(CXX) -o $@  $(CPPFLAGS) $(CXXFLAGS) -c $^

h5rnd: h5rnd.o
	$(CXX) $^ $(LIBS) -o $@	

clean:
	@$(RM) *.o *.h5 $(apps) h5rnd *.gv *.svg

dist-clean: clean 
.PHONY: test

