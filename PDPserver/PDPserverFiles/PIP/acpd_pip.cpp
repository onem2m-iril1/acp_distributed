
#include <iostream>
#include <string>
#include <list> // http://www.cplusplus.com/reference/list/list/

#include "acpd_pip.h"

#include "../reqres/acpd_reqres.h"
#include "../PIP/acpd_pip.h"
#include "../../MbedJSONValue.h"

using namespace std;

namespace acpd_pip
{
	list<acpd_reqres::AttributeValuePair> getRequesteeAttributes(string strPacket)
	{
		cout << "In pip - getRequesteeAttributes" << endl;
		// this function will extract the elements usefull for ACP check and return them.
			// parse the recieved registration buffer
		list<acpd_reqres::AttributeValuePair> attrList ,attrListEmpty;
		acpd_reqres::AttributeValuePair attrObj;
		string to, fr;
		int op, ty;
		MbedJSONValue MSG;
		parse(MSG, strPacket.c_str());
		// Find 'to' Value
		if(MSG.hasMember("to")) {
			to = MSG["to"].get<string>();
			std::cout << "to: " << to << endl;
		}
		else {
			printf("'to' not Specified\n");
			return attrListEmpty;
		}
		attrObj.setAttributeValuePair("to", to);
		attrList.push_back(attrObj);
		// Find 'fr' Value
		if(MSG.hasMember("fr")) {
			fr = MSG["fr"].get<string>();
			std::cout << "fr: " << fr << endl;
		}
		else {
			printf("'fr' not Specified\n");
			return attrListEmpty;
		}
		attrObj.setAttributeValuePair("fr", fr);
		attrList.push_back(attrObj);
		// Find 'op' Value
		if (MSG.hasMember("op")) {
			op = MSG["op"].get<int>();
			cout << "op: " << op << endl;
		}
		else {
			cout << "'op' not specified" << endl;
			return attrListEmpty;
		}
		attrObj.setAttributeValuePair("op",op);
		attrList.push_back(attrObj);
		// Find 'ty' Value
		if(MSG.hasMember("ty")) {
			ty = MSG["ty"].get<int>();
			std::cout << "ty: " << ty << endl;
		}
		else {
			printf("'ty' not Specified\n");
			return attrListEmpty;
		}
		attrObj.setAttributeValuePair("ty",ty);
		attrList.push_back(attrObj);
		return attrList; // yet returning what is received as request was simple
	}
}
