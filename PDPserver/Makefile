
ACPserver: MbedJSONValue.o ACPServer.o acpd_pdp.o acpd_prp.o acpd_pip.o acpd_reqres.o
	c++ MbedJSONValue.o ACPServer.o acpd_pdp.o acpd_prp.o acpd_pip.o acpd_reqres.o -o ACPserver -lssl -lcrypto -lpthread -lsqlite3 

ACPServer.o: ACPServer.cpp
	c++ -c ACPServer.cpp -lssl -lcrypto -pthread -lsqlite3

MbedJSONValue.o: MbedJSONValue.cpp
	c++ -c MbedJSONValue.cpp 

acpd_pdp.o: /home/pi/temp01/PDPserver/PDPserverFiles/PDP/acpd_pdp.cpp
	c++ -c /home/pi/temp01/PDPserver/PDPserverFiles/PDP/acpd_pdp.cpp

acpd_prp.o: /home/pi/temp01/PDPserver/PDPserverFiles/PRP/acpd_prp.cpp
	c++ -c /home/pi/temp01/PDPserver/PDPserverFiles/PRP/acpd_prp.cpp

acpd_pip.o: /home/pi/temp01/PDPserver/PDPserverFiles/PIP/acpd_pip.cpp
	c++ -c /home/pi/temp01/PDPserver/PDPserverFiles/PIP/acpd_pip.cpp

acpd_reqres.o: /home/pi/temp01/PDPserver/PDPserverFiles/reqres/acpd_reqres.cpp
	c++ -c /home/pi/temp01/PDPserver/PDPserverFiles/reqres/acpd_reqres.cpp

clear:
	rm -f MbedJSONValue.o ACPServer.o acpd_pdp.o acpd_prp.o acpd_pip.o ACPserver
