all: deauth-attack

deauth-attack: main.o proto_structures.o
	g++ main.o proto_structures.o -o deauth-attack

main.o: main.cpp proto_structures.h
	g++ main.cpp -c -o main.o

proto_structures.o: proto_structures.cpp proto_structures.h
	g++ proto_structures.cpp -c -o proto_structures.o

clean:
	rm -f *.o
	rm -f deauth-attack
