accelMake: example.cpp sca3300.cpp sca3300-tools.cpp
	g++ -c sca3300.cpp
	g++ -c sca3300-tools.cpp
	g++ -c example.cpp
	g++ -c main.cpp
	#g++ -o test sca3300.o sca3300-tools.o example.o -DARMA_DONT_USE_WRAPPER -lopenblas -llapack
	g++ -o test sca3300.o sca3300-tools.o example.o
	g++ -o main sca3300.o sca3300-tools.o main.o
