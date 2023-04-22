#pragma once

#include "FactorHelpers.h"
#include "UnitTests.h"

class FactorTests : public UnitTester
{
public:
	virtual void RunTests() override;

private:
	void TestCondition(UInt32 n, UInt32 factorSum);

	static const UInt32 ValidFactors[];
	static const UInt32 InitialTestValues[];
	static const UInt32 BigTestValue = 859963392;
	static char TestNameBuffer[255];
};