#pragma once

#include <chrono>

using namespace std::chrono;

class UnitTester
{
public:
	virtual void RunTests() = 0;

protected:
	void TestCondition(bool bSuccess, const char* testName);

private:
	const char* ResultLabel[2] =
	{
		"Fail",
		"Pass"
	};
};