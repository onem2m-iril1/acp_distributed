
#include <iostream>
#include <string>
#include <list>

#include "acpd_reqres.h"

using namespace std;

namespace acpd_reqres
{
	//
	//  AttributeValuePair   -=-=-=-=-=-=-=-=-
	//

	AttributeValuePair::AttributeValuePair()
	{
		//  cout << "ACRk Object created." << endl;
	}

	AttributeValuePair::~AttributeValuePair()
	{
		//  cout << "AttributeValuePair Object deleted." << endl;
	}

	bool AttributeValuePair::setAttributeValuePair(string attri, string val)
	{
		attribute = attri;
		value = val;
		return 1;
	}

	bool AttributeValuePair::setAttributeValuePair(string attri, int val)
	{
		string strVal = intToString(val);
		setAttributeValuePair(attri, strVal);
		return 1;
	}

	string intToString(int x)
	{
		string str;
		if (x < 10)
		{
			str = char(x) + '0';
			return str;
		}
		str = char(x % 10) + '0';
		return intToString(x / 10) + str;
	}

}