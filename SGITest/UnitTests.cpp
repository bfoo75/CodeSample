#include "UnitTests.h"

#include <iostream>

void UnitTester::TestCondition(bool bSuccess, const char* testName)
{
	std::cout << testName << ": " << ResultLabel[bSuccess] << "\r\n";
}