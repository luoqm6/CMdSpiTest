objects = main.o CMdSpi.o ./thostmduserapi.so ./thosttraderapi.so

CTP:${objects}
	g++ -o CTP ${objects}

main.o:main.cpp
	g++ -c main.cpp

CMdSpi.o:CMdSpi.cpp CMdSpi.h
	g++ -c CMdSpi.cpp

clean:
	rm CTP main.o CMdSpi.o