#include "FactorTests.h"

#include <iterator>
#include <cstdlib>

char FactorTests::TestNameBuffer[255] = {};
const UInt32 FactorTests::InitialTestValues[] =
{
	1, 2, 3, 4, 5, 6, 8, 9, 10,	12,	15,	16,	18,	20,	24,	25,	27,	30
};

const UInt32 FactorTests::ValidFactors[] =
{
	2, 3, 5
};

//WG - Had another wild goose chase with this problem before settling on a solution I liked.
//Originally I started optimizing the algorithm for getting the factor list of an arbitrary number,
//but realized through testing that iterating to 859963392 wasn't really feasible, despite wanting
//to use a bunch of bit operator tricks for figuring out if numbers are divisible.  So I changed my
//strategy to find the next factor in the list by multiplying the available factors.  That led me
//to do a bunch of math to see if I could find a relationship between the growth of factors and n,
//but it didn't seem to be leading me anywhere except towards that I needed to be able to count primes.
//That was a good indication I was on the right track given the importance of that task in math.  I eventually figured
//out that the factors always had to use the lowest number in the sequence they had last been factored against
//and could fully understand the uniqueness of the problem eg: the further down the factor list you go, 
//the more memory is required to store the list of old factors that still need to be checked against.

//I also wasn't too sure what I could use for collections.  I know it said c++ on the test... but I opted to
//use c style arrays.  I'm a little rusty on best practices here since unreal and unity both have
//radically different policies in terms of array lengths & resizeable collections than C.

void FactorTests::RunTests()
{
	size_t TestValueLength = std::size(InitialTestValues);
	for (UInt32 i = 0; i < TestValueLength; i++)
	{
		//WG - N is not 0 based.
		UInt32 n = i + 1;
		TestCondition(FactorHelpers::GetNthFactorSum(n, ValidFactors), InitialTestValues[i]);
	}

	steady_clock::time_point start = high_resolution_clock::now();
	UInt32 factorSum = FactorHelpers::GetNthFactorSum(1500, ValidFactors);
	steady_clock::time_point end = high_resolution_clock::now();
	duration<double> elapsed = end - start;

	TestCondition(factorSum, BigTestValue);
	FactorHelpers::ShowFactors(factorSum, ValidFactors);

	std::cout << "BigTestValue Total time (s): " << elapsed.count() << "\r\n";
}

//WG - NOT THREAD SAFE since the buffer is being reused.
void FactorTests::TestCondition(UInt32 n, UInt32 factorSum)
{
	sprintf_s(TestNameBuffer, "FactorSum - n: %i\t sum: %i\t", n, factorSum);
	UnitTester::TestCondition(n == factorSum, TestNameBuffer);
}