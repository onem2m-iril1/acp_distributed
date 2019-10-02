
#include <iostream>
#include <string>
#include <list> // http://www.cplusplus.com/reference/list/list/

#include "acpd_pdp.h"

#include "../reqres/acpd_reqres.h"
#include "../PIP/acpd_pip.h"
#include "../PRP/acpd_prp.h"

using namespace std;

using namespace acpd_pdp;

void pdphello ()
{
	cout << "hello pdp" << endl;
}

namespace acpd_pdp
{
	bool getPEPrequest(string strPacket)
	{
		cout << "In pdp - getPEPrequest 1" << endl;
		list<acpd_reqres::AttributeValuePair> listObj2, listObj3;
		listObj2 = acpd_pip::getRequesteeAttributes(strPacket);	// get required attributes from PIP
		cout << "In pdp - getPEPrequest back from PIP" << endl;
		cout << listObj2.front().value << endl;
		listObj3 = acpd_prp::getACPfromPRP(listObj2);	// get matching policy rule from PRP
//		cout << listObj3.front().value << endl;
		cout << "In pdp - getPEPrequest back from PRP" << endl;
		if (matchACPandAttributes(listObj2, listObj3)) // match attributes and rule respectively to be sure
			return true;
		return false;
	}

	bool matchACPandAttributes(list<acpd_reqres::AttributeValuePair> listObj1, list<acpd_reqres::AttributeValuePair> listObj2)
	{
		cout << "In pdp - matchACPandAttributes 1" << endl;
		if (listObj2.empty())
			return false;
		return true;
	}
}



/*
bool getPEPrequest(string strPacket)
{
	cout << "In pdp - getPEPrequest" << endl;
/*	list<acpd_reqres::AttributeValuePair> listObj2, listObj3;
	listObj2 = acpd_pip::getRequesteeAttributes(strPacket);	// get required attributes from PIP
	cout << listObj2.front().value << endl;
	listObj3 = acpd_prp::getACPfromPRP(listObj2);	// get matching policy rule from PRP
//		cout << listObj3.front().value << endl;
	if (matchACPandAttributes(listObj2, listObj3)) // match attributes and rule respectively to be sure
		return true;
//		/
	return false;
}

bool matchACPandAttributes(list<acpd_reqres::AttributeValuePair> listObj1, list<acpd_reqres::AttributeValuePair> listObj2)
{
	if (listObj2.empty())
		return false;
	return true;
}
*/