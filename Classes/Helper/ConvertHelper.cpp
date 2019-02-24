#include "ConvertHelper.h"

int ConvertHelper::asInt(std::string number)
{
	int value = 0;
	for (int i = 0; i < number.length(); i++)
		value = 10 * value + (int)(number[i] - '0');

	return value;
}
