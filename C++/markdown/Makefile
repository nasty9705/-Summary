CXX= g++
CXXFLAGS= -g -std=c++11

all: main.o abstractText.o errors.o header.o list.o functions.o usualtext.o textViewer.o code.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) ${CXXFLAGS} -c $<

clean:
	rm -f $(BINARIES) *.o markdown
