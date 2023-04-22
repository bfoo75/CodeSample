#include "FactorHelpers.h"

#include <cstdlib>
#include <iostream>

namespace FactorHelpers
{
	bool IsEven(UInt32 value)
	{
		return value & 1;
	}
}