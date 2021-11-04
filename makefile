
Lab2: Client.o MFQS.o RTS.o
	g++ -o Lab2 Client.o MFQS.o RTS.o Process.o
	
Client.o: Client.cpp MFQS.h RTS.h
	g++ -c Client.cpp 

MFQS.o: MFQS.cpp MFQS.h
	g++ -c MFQS.cpp
	
RTS.o: RTS.cpp RTS.h
	g++ -c RTS.cpp
	
Process.o: Process.cpp Process.h
	g++ -c Process.cpp
	
clean:
	rm *.o Lab2





