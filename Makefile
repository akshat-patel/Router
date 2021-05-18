router.out : RouterRunner.o router.o
	g++ -Wall -g -o router.out RouterRunner.o router.o

RouterRunner.o : RouterRunner.cpp RouterRunner.h
	g++ -Wall -g -c RouterRunner.cpp

router.o : router.cpp router.h 
	g++ -Wall -g -c router.cpp

clean:
	rm router.out router.o RouterRunner.o
