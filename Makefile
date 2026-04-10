CXX = g++
CXXFLAGS = -stdc++17 -Waöö -Wextra -Iinclude -I./Crow/include -pthread
LDFLAGS = -pthread

SRC_CORE = \
	src/security_config.cpp \
	src/database.cpp \
	src/bank_account.cpp \
	src/bank_manager.cpp \
	src/bank_service.cpp

all: bank_cli bank_api

bank_cli: apps/cli_main.cpp $(SRC_CORE)
	$(CXX) $(CXXFLAGS) -o $@ apps/cli_main.cpp $(SRC_CORE) $(LDFLAGS)

bank_api : apps/api_main.cpp $(SRC_CORE)
	$(CXX) $(CXXFLAGS) -o $@ apps/api_main.cpp $(SRC_CORE) $(LDFLAGS)

clean:
	rm -f bank_cli bank_api 