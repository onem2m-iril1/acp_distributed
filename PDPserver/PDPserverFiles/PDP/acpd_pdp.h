#pragma once

#include <iostream>
#include <string>
#include <list> // http://www.cplusplus.com/reference/list/list/

#include "../reqres/acpd_reqres.h"

using namespace std;

void pdphello ();

namespace acpd_pdp
{
	bool getPEPrequest(string strPacket);
	bool matchACPandAttributes(list<acpd_reqres::AttributeValuePair> listObj1, list<acpd_reqres::AttributeValuePair> listObj2);
}
