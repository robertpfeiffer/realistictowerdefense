#include <convert.h>

using namespace rapidxml;

#ifndef WIN32
	#include <strings.h>
	#include <string.h>
	#define _strcmpi strcasecmp
#endif

std::string Convert::toString(int i)
{
	std::stringstream out;
	out << i;
	return out.str();
}

bool Convert::attrToBool(xml_attribute<>* attr, bool defaultValue)
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	if (strcmp(str, "0") == 0) return false;
	if (strcmp(str, "1") == 0) return true;

	if (_strcmpi(str, "false") == 0) return false;
	if (_strcmpi(str, "true") == 0) return true;

	return defaultValue;
}

long Convert::attrToLong(xml_attribute<>* attr, long defaultValue)
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	long val = atol(str);
	if (val == 0)
	{
		if (strcmp(str, "0") != 0) return defaultValue;
	}
	return val;	
}

double Convert::attrToDouble(xml_attribute<>* attr, double defaultValue)
{
	if (attr == NULL) return defaultValue;
	char* str = attr->value();

	double val = atof(str);
	if (val == 0.0)
	{
		if (strcmp(str, "0.0") != 0) return defaultValue;
	}
	return val;	
}
