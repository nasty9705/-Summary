CXX= g++
CXXFLAGS= -g -std=c++11

BINARIES= generator matrix

all: generator matrix

generator: maingenerate.o generatematrix.o generatevector.o
	$(CXX) $(CXXFLAGS) -o $@ $^

matrix: main.o biginteger.o elements.o ratlib.o matrix.o vector.o errors.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) ${CXXFLAGS} -c $<

clean:
	rm -f $(BINARIES) *.o generator matrix
