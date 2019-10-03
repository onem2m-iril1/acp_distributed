#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

#include <iostream>
#include <string>
//#include <string.h> // for c string / arrays
#include <list> // http://www.cplusplus.com/reference/list/list/
//#include <cmath>

#include "sqlite3.h"
#include "acpd_prp.h"

#include "../reqres/acpd_reqres.h"
#include "../PDP/acpd_pdp.h"

using namespace std;

namespace acpd_prp
{
	Resource2ACPTable resource2ACPTableObj;
	bool acpMatchFlag;
	string globalOperation;
	list<acpd_reqres::AttributeValuePair> getACPfromPRP(list<acpd_reqres::AttributeValuePair> listObj1)
	{
		cout << "In PRP - getACPfromPRP 1" << endl;
//		initializePRP();
		list<acpd_reqres::AttributeValuePair> listObj2;
		bool checkFlag = checkRequestACPfromDB(listObj1);
		if (checkFlag)
			return listObj1;
		return listObj2;
	}
	
	int stringToInt(string str)
	{
		short result = 0, strLength = str.length() - 1;
		for (int i = 0; i <= strLength; i++)
		{
			result += ((str[i] - '0') * pow(10.0, strLength - i));
		}
		return result;
	}

	bool checkOp(string op, string chACP) // op is the request message attribute, chACP is the ACP rule
	{
		//	op couldn't be lesser than 1..!
		int opi, chACPi;
		chACPi = stringToInt(chACP);
		opi = stringToInt(op);
		int mask = pow(2.0, opi - 1);
	//	cout << "op = " << opi << "chACP = " << chACPi << "mask = " << mask << endl;
		if (chACPi & mask)
			return 1;
		return 0;
	}

	static int callback1(void *NotUsed, int argc, char **argv, char **azColName) {
	   int i;
/*	   cout << "callback function" << endl;
	   cout << "callback function" << endl;
	   cout << "callback function" << endl;
	   cout << "callback function" << endl;
	   */
	   for(i = 0; i<argc; i++) {
//		   cout << argv[i];
		   if (azColName[i][0] == 'o' && azColName[i][1] == 'p')
		   {
			   //cout << "gotOP" << endl;
			   if (checkOp(globalOperation,argv[i]))
			   {
					//cout << "matched.!";
					acpMatchFlag = 1;
				}
		   }
		  //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	   }
	   //printf("\n");
	   return 0;
	}

	bool checkRequestACPfromDB(list<acpd_reqres::AttributeValuePair> listObj1)
	{
	   sqlite3 *db;
	   char *zErrMsg = 0;
	   int rc;
	   
	   /* Open database */
	   rc = sqlite3_open("test.db", &db);
	   
	   printf("1\n");
	   if( rc ) {
		  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		  return(0);
	   } else {
		  fprintf(stdout, "Opened database successfully\n");
	   }
		list<acpd_reqres::AttributeValuePair>::iterator reqMsgIterator = listObj1.begin();
		reqMsgIterator->value;

	   string s = "abc";
	   // assuming the request has exactly 4 attributes/values and they are exactly in to-from-op-ty order.
	   string to = reqMsgIterator->value;
	   reqMsgIterator++;
	   string from = reqMsgIterator->value;
	   reqMsgIterator++;
	   string op = reqMsgIterator->value;
	   reqMsgIterator++;
	   string type = reqMsgIterator->value;
	   globalOperation = op;
	   
	   cout << "r to = ." << to << "." << endl;
	   cout << "r fr = ." << from << "." << endl;
	   cout << "r op = ." << op << "." << endl;
	   cout << "r ty = ." << type << "." << endl;
	   /* Create SQL statement */   
	   s = "SELECT ResID, fr, op, ty FROM resource INNER JOIN ACRt ON resource.ACPID = ACRt.ACPID WHERE " \
	   "resource.ResID = '" + to + "' AND " \
	   "ACRt.fr = '" + from + "' AND " \
	   "ACRt.ty = " + type + ";"; // ORDER BY ROWID ASC LIMIT 1
	//	   "ACRt.op = " + op +	" AND " \

	/*
	   s = "INSERT INTO resource (ID,ResID,ACPID) VALUES ('13','//myMAF.provider.org/-/','4'); \
	   INSERT INTO ACRt (ACRID,ACPID,fr,op,ty) VALUES ('7','4','0 2 481 1 100 3030 10011','1','3');";
//	*/
		//s = "SELECT ResID, fr, op, ty FROM resource INNER JOIN ACRt ON resource.ACPID = ACRt.ACPID;";
	   char sql[s.length()+1];
	   strcpy(sql, s.c_str());
	   acpMatchFlag = 0; // initialize

	   /* Execute SQL statement */
	   rc = sqlite3_exec(db, sql, callback1, 0, &zErrMsg);
	   
	   if( rc != SQLITE_OK ){
		  fprintf(stderr, "SQL error: %s\n", zErrMsg);
		  sqlite3_free(zErrMsg);
	   } else {
		  fprintf(stdout, "All executed successfully-\n");
	   }	   
	   sqlite3_close(db);
	   if (acpMatchFlag)
	   {
		   return 1;
	   }
		return 0;
	}

	void initializePRP()
	{
		cout << "In PRP - initializePRP" << endl;
		generateACP();
		cout << "In PRP - initializePRP - ACP generated" << endl;
	}

	bool checkRequestACP(list<acpd_reqres::AttributeValuePair>& requestObj)
		// ACR class contains attribute-value pairs which are just like the request message
		// This would be a generic/scalable function
	{
		//  cout << requestObj.ACRiList.back().attribute << endl;
		list<ACP*>::iterator ACPIt; // A single ACP pointer List having a list of ACP pointed by it
		list<ACR>::iterator ACRIt; // traverse list of rules in a single ACP
		list<acpd_reqres::AttributeValuePair>::iterator attributeValuePairIt; // traverse attributes in a single rule
		list<acpd_reqres::AttributeValuePair>::iterator requestAttributeValuePairIt; // traverse attributes in a single rule
		unsigned int ACPCounter;
		unsigned int ACRCounter;
		unsigned int attributeValuePairCounter;
		bool checkerFlag = 0;
		// traverse the list of ACP
		for (ACPCounter = 0, ACPIt = resource2ACPTableObj.ACPList.begin();
			ACPIt != resource2ACPTableObj.ACPList.end(); ACPCounter++, ACPIt++)
		{
			// traverse the list of ACR within the single ACP
			for (ACRCounter = 0, ACRIt = (*ACPIt)->ACRList.begin();
				ACRIt != (*ACPIt)->ACRList.end(); ACRCounter++, ACRIt++)
			{
				checkerFlag = 1;
				//      cout << "ACR " << ACRCounter << endl;
				for (attributeValuePairCounter = 0, attributeValuePairIt = ACRIt->ACRiList.begin(),
					requestAttributeValuePairIt = requestObj.begin();
					attributeValuePairIt != ACRIt->ACRiList.end();
					attributeValuePairCounter++, attributeValuePairIt++, requestAttributeValuePairIt++)
				{
					// yet this loop assumes that only four values are to be iterated which are in a strict pattern according to request.
					if (requestAttributeValuePairIt == requestObj.end() || /* request does't have enough parameter to match the ACR */
						attributeValuePairIt->attribute != requestAttributeValuePairIt->attribute || /* Attributes doesn't match (assumption of fixed values fail) */
						attributeValuePairIt->value != requestAttributeValuePairIt->value) /* values of attributes doesn't match    */
					{
						checkerFlag = 0;
					//	/*
						cout << attributeValuePairIt->attribute << "\treq " << requestAttributeValuePairIt->attribute << "\tval " <<
												attributeValuePairIt->value << "\treq " << requestAttributeValuePairIt->value << endl;
						//	*/
					}
				}
				if (checkerFlag == 1) // whole ACR has been successfully matched.!
				{
					//              cout << "matched.!" << endl;
					return 1;
				}
			}
		}
		return 0;
	}

	void generateACP()
	{
		acpd_reqres::AttributeValuePair* attributeValuePairObj;
		ACR* ACRObj;
		ACP* ACPObjPtr;
		// first ACR in first ACP
		attributeValuePairObj = new acpd_reqres::AttributeValuePair;
		ACRObj = new ACR;
		attributeValuePairObj->setAttributeValuePair("to", "//myMAF.provider.org/-/");
		ACRObj->addACRi(*attributeValuePairObj);
//		attributeValuePairObj->setAttributeValuePair("fr", "1.1");
		attributeValuePairObj->setAttributeValuePair("fr", "0 2 481 1 100 3030 10011");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("op", "1");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("ty", "3");
		ACRObj->addACRi(*attributeValuePairObj);

		ACPObjPtr = new ACP;
		ACPObjPtr->addACR(*ACRObj);
		// dont add acp yet
	//    resource2ACPTableObj.addACP(*ACPObjPtr); // a new pointer to the ACP object pointed by ACPObjPtr is added in resourceObj.
										//  printACP(*(resourceObj.ACPList.front()));
										// now two pointers are pointing to a single object. So, ACPObjPtr can point to new ACP object
		// second ACR in first ACP
		attributeValuePairObj = new acpd_reqres::AttributeValuePair;
		ACRObj = new ACR;
		attributeValuePairObj->setAttributeValuePair("to", "1.1.12");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("From", "2.2.12");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("op", "11");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("ty", "22");
		ACRObj->addACRi(*attributeValuePairObj);

		//    ACPObjPtr = new ACP;  // add to ACP already created ago
		ACPObjPtr->addACR(*ACRObj);
		resource2ACPTableObj.addACP(*ACPObjPtr);

		// first ACR in second ACP
		attributeValuePairObj = new acpd_reqres::AttributeValuePair;
		ACRObj = new ACR;
		attributeValuePairObj->setAttributeValuePair("to", "1.1.22");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("From", "2.2.22");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("op", "21");
		ACRObj->addACRi(*attributeValuePairObj);
		attributeValuePairObj->setAttributeValuePair("ty", "32");
		ACRObj->addACRi(*attributeValuePairObj);

		ACPObjPtr = new ACP;
		ACPObjPtr->addACR(*ACRObj);
		resource2ACPTableObj.addACP(*ACPObjPtr);


		// delete ptrs
		attributeValuePairObj = 0;
		delete attributeValuePairObj;
		ACRObj = 0;
		delete ACRObj;
		ACPObjPtr = 0;
		delete ACPObjPtr;
		cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
	}

	//
	//  ACR -=-=-=-=-=-=-=-=-
	//

	ACR::ACR()
	{
		//  cout << "ACR Object created." << endl;
	}

	ACR::~ACR()
	{
		//  cout << "ACR Object deleted." << endl;
	}

	void ACR::addACRi(acpd_reqres::AttributeValuePair attributeValuePairObj)
	{
		ACRiList.push_back(attributeValuePairObj);
	}

	//
	//  ACP -=-=-=-=-=-=-=-=-
	//

	ACP::ACP()
	{
		//  cout << "ACP Object created." << endl;
	}

	ACP::~ACP()
	{
		cout << "ACP Object deleted." << endl;
	}

	void ACP::addACR(ACR ACRObj)
	{
		ACRList.push_back(ACRObj);
	}

	//
	//  resource    -=-=-=-=-=-=-=-=-
	//

	Resource2ACPTable::Resource2ACPTable()
	{
		//  cout << "resource Object created." << endl;
	}

	Resource2ACPTable::~Resource2ACPTable()
	{
		//  cout << "resource Object deleted." << endl;
	}

	void Resource2ACPTable::addACP(ACP& ACPObj1)
	{
		ACP** ACPObjDPtr = new ACP*;
		*ACPObjDPtr = &ACPObj1;
		ACPList.push_back(*ACPObjDPtr);
		ACPObjDPtr = 0;
	}

}
