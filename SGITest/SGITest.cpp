#include "OrientedRectTests.h"
#include "FactorTests.h"

#include <iostream>

int main()
{
	UnitTester* TestFixtures[] =
	{
		new OrientedRectTests(),
		new FactorTests()
	};

	for (auto TestFixture : TestFixtures)
	{
		TestFixture->RunTests();
		delete TestFixture;
	}
}