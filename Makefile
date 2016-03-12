OBJS = src/CException.o src/CPoint.o src/CObstacle.o src/CSnake.o src/CSingle.o src/CMultPC.o src/CMultNet.o src/CSocket.o src/CSocketServer.o src/CSocketClient.o src/CNibbles.o src/main.o
CC = g++
FLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb
LL = -lncurses

all: compile

compile: Nibbles

Nibbles: $(OBJS)
	cd src/;\
	g++ -Wall -pedantic -ggdb -o Nibbles main.cpp CNibbles.cpp CMultNet.cpp CSocketServer.cpp CSocketClient.cpp CSocket.cpp CMultPC.cpp CSingle.cpp CSnake.cpp CObstacle.cpp CPoint.cpp CException.cpp -lncurses;
	mkdir -p bin
	mv *.o src/
	$(CC) $(FLAGS) $(OBJS) -o bin/Nibbles $(LL)

run:
	matallui/Nibbles

src/CException.o: src/CException.h src/CException.cpp
	$(CC) $(FLAGS) -c src/CException.cpp $(LL)
	
src/CPoint.o: src/CPoint.h src/CPoint.cpp
	$(CC) $(FLAGS) -c src/CPoint.cpp $(LL)
	
src/CObstacle.o: src/CPoint.h src/CObstacle.h src/CObstacle.cpp
	$(CC) $(FLAGS) -c src/CObstacle.cpp $(LL)
	
src/CSnake.o: src/CPoint.h src/CObstacle.h src/CSnake.h src/CSnake.cpp
	$(CC) $(FLAGS) -c src/CSnake.cpp $(LL)
	
src/CSingle.o: src/CPoint.h src/CObstacle.h src/CSnake.h src/CSingle.h src/CSingle.cpp
	$(CC) $(FLAGS) -c src/CSingle.cpp $(LL)
	
src/CMultPC.o: src/CPoint.h src/CObstacle.h src/CSnake.h src/CSingle.h src/CMultPC.h src/CMultPC.cpp
	$(CC) $(FLAGS) -c src/CMultPC.cpp $(LL)
	
src/CMultNet.o: src/CPoint.h src/CObstacle.h src/CSnake.h src/CSingle.h src/CMultPC.h src/CMultNet.h src/CMultNet.cpp
	$(CC) $(FLAGS) -c src/CMultNet.cpp $(LL)
	
src/CSocket.o: src/CSocket.h src/CSocket.cpp
	$(CC) $(FLAGS) -c src/CSocket.cpp $(LL)
	
src/CSocketClient.o: src/CSocket.h src/CSocketClient.h src/CSocketClient.cpp
	$(CC) $(FLAGS) -c src/CSocketClient.cpp $(LL)
	
src/CSocketServer.o: src/CSocket.h src/CSocketServer.h src/CSocketServer.cpp
	$(CC) $(FLAGS) -c src/CSocketServer.cpp $(LL)
	
src/CNibbles.o: src/CPoint.h src/CObstacle.h src/CSnake.h src/CSingle.h src/CMultPC.h src/CMultNet.h src/CSocket.h src/CSocketClient.h src/CSocketServer.h src/CNibbles.h src/CNibbles.cpp
	$(CC) $(FLAGS) -c src/CNibbles.cpp $(LL)
	
src/main.o: src/CPoint.h src/CObstacle.h src/CSnake.h src/CSingle.h src/CMultPC.h src/CMultNet.h src/CSocket.h src/CSocketClient.h src/CSocketServer.h src/CNibbles.h src/main.cpp
	$(CC) $(FLAGS) -c src/main.cpp $(LL)

clean:
	-rm -rf src/*.o src/Nibbles* bin doc
	
