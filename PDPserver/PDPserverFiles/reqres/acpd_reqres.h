#pragma once

#include <iostream>
#include <string>
#include <list> // http://www.cplusplus.com/reference/list/list/

//extern class resource Resource2ACPTable;
using namespace std;

namespace acpd_reqres
{
	string intToString(int x);

	class AttributeValuePair
	{
	public:
		AttributeValuePair();
		~AttributeValuePair();
		string attribute;
		string value;
		// didnt make getter function as they may waste memory by returning the object.
		bool setAttributeValuePair(string, string);
		bool setAttributeValuePair(string, int);
	};

	class Acpd_reqres
	{
	public:
		string name;
	};

}
