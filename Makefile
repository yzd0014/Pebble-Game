output: main.o global_data.o gamePlay.o agent.o
	g++ main.o global_data.o gamePlay.o agent.o -o pebbleGame

main.o: main.cpp
	g++ -c main.cpp

global_data.o: global_data.cpp
	g++ -c global_data.cpp

gamePlay.o: gamePlay.cpp
	g++ -c gamePlay.cpp

agent.o: agent.cpp
	g++ -c agent.cpp

clean:
	rm *.o