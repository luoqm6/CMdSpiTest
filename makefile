objects = main.o CMdSpi.o ./thostmduserapi.so ./thosttraderapi.so

CTP:${objects}
	g++ -o CTP ${objects}

*.o:*.cpp
	g++ -c *.cpp

