#pragma once

#include <iostream>
#include <string>
#include <list> // http://www.cplusplus.com/reference/list/list/

#include "acpd_pip.h"

#include "../reqres/acpd_reqres.h"
#include "../PIP/acpd_pip.h"

using namespace std;

namespace acpd_pip
{
	list<acpd_reqres::AttributeValuePair> getRequesteeAttributes(string strPacket);

}
