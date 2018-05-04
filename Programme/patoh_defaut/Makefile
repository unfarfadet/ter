COPTIONS = -std=c++11 -O3 -Wall -Wextra
LOPTIONS = -L. -lpatoh -lm 

ter : ter.o gestionnaire.o
	g++ -o ter ter.o gestionnaire.o $(LOPTIONS)

ter.o : ter.cc
	g++ $(COPTIONS) -c ter.cc

gestionnaire.o : gestionnaire.cc gestionnaire.h
	g++ $(COPTIONS) -Wall -c gestionnaire.cc

clean:
	rm -rf *.o
	rm -rf ter	
