#pragma once

#include <iostream>
#include <string>
#include <list> // http://www.cplusplus.com/reference/list/list/

#include "../reqres/acpd_reqres.h"
#include "../PDP/acpd_pdp.h"

using namespace std;

namespace acpd_prp
{
	list<acpd_reqres::AttributeValuePair> getACPfromPRP(list<acpd_reqres::AttributeValuePair> listObj);
	int stringToInt(string str);
	bool checkOp(string op, string chACP); // op is the request message attribute, chACP is the ACP rule
	static int callback1(void *NotUsed, int argc, char **argv, char **azColName);
	bool checkRequestACPfromDB(list<acpd_reqres::AttributeValuePair> listObj1);
	void initializePRP();
	void generateACP();
	bool checkRequestACP(list<acpd_reqres::AttributeValuePair>& requestObj);
	class ACR   // its "acr(k)" i.e. k'th rule; It is one single rule, containing multiple 
//attributes (accessControlOrigiator, accessControlOperation, accessControlContext)
	{
	public:
		ACR();
		~ACR();
		void addACRi(acpd_reqres::AttributeValuePair);
		list<acpd_reqres::AttributeValuePair> ACRiList;
	};

	class ACP // A single ACP contains // One set of multiple rules. at least one 
	//self-priviliges and one priviliges rule 
	{
	public:
		ACP();
		~ACP();
		void addACR(ACR);
		list<ACR> ACRList;
	};

	class Resource2ACPTable // first column is only one resource (CSE/AE -ID),
	// second contains multiple ACP objects
	{
	public:
		Resource2ACPTable();
		~Resource2ACPTable();
		string resourceID;
		list<ACP*> ACPList; // a list of pointers to multiple ACP objects
		void addACP(ACP&);
	};
	extern Resource2ACPTable resource2ACPTableObj;
}
