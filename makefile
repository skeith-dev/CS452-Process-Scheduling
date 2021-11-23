
 
Lab2: Client.o MFQS.o RTS.o Process.o Statistics.o
	g++ -o Lab2 Client.o MFQS.o RTS.o Process.o Statistics.o
	
	
Client.o: Client.cpp Client.h MFQS.h RTS.h
	g++ -c Client.cpp 


MFQS.o: MFQS.cpp MFQS.h
	g++ -c MFQS.cpp
	
RTS.o: RTS.cpp RTS.h
	g++ -c RTS.cpp
	
Process.o: Process.cpp Process.h
	g++ -c Process.cpp
	
Statistics.o: Statistics.cpp Statistics.h
	g++ -c Statistics.cpp

clean:
	rm *.o Lab2


#-I/usr/informix/incl/c++


