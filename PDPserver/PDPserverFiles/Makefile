
mafserver: MbedJSONValue.o mafServer.o acpd_pdp.o 
	c++ MbedJSONValue.o mafServer.o -I/home/pi/temp01/PDPserver/PDPserverFiles/PDP/ acpd_pdp.o -o mafserver -lssl -lcrypto -lpthread -lsqlite3 

mafServer.o: mafServer.cpp
	c++ -c mafServer.cpp -lssl -lcrypto -pthread -lsqlite3

MbedJSONValue.o: MbedJSONValue.cpp
	c++ -c MbedJSONValue.cpp 

acpd_pdp.o: /home/pi/temp01/PDPserver/PDPserverFiles/PDP/acpd_pdp.cpp
	c++ -I/home/pi/temp01/PDPserver/PDPserverFiles/PDP/ -c acpd_pdp.cpp

clear: 
	rm -f MbedJSONValue.o mafServer.o mafserver
