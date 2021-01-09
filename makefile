CFLAGS = -std=c++17 -Wfatal-errors -Wall -pedantic 
DFLAGS = -pthread

all :  main server joueur partie utils 
	g++ -o exec main.o partie.o joueur.o server.o utils.o $(DFLAGS)

main : main.cpp
	g++ -c -o main.o main.cpp $(CFLAGS)

server : serveur.cpp
	g++ -c -o server.o serveur.cpp $(CFLAGS)

joueur: joueur.cpp 
	g++ -c -o joueur.o joueur.cpp $(CFLAGS)

partie: partie.cpp 
	g++ -c -o partie.o partie.cpp $(CFLAGS)

utils: utils.cpp
	g++ -c -o utils.o utils.cpp $(CFLAGS)

