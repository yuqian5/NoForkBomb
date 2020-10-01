CC = g++ -std=c++11 -O

all : build

build :
	$(CC) -o fbs fbs.cpp
	$(CC) -o fb mini_fork_bomb.cpp

clean :
	rm fbs fb