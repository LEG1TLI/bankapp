CXX = g++
CXXFLAGS = -std=c++14 -I./Crow/include -pthread
LDFLAGS = -pthread 

test_server: test_server.cpp
	$(CXX) $(CXXFLAGS) -o test_server test_server.cpp $(LDFLAGS)

clean:
	rm -f test_server test_server.exe