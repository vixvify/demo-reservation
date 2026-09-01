CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

SERVER_SOURCES = \
	src/server/server.cpp \
	src/server/worker.cpp \
	src/reservation/reservation.cpp \
	src/utils/logger.cpp \
	src/utils/delay.cpp

CLIENT_SOURCES = \
	src/client/client.cpp

all: server client

server:
	$(CXX) $(CXXFLAGS) $(SERVER_SOURCES) -o server

client:
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o client

clean:
	rm -f server client