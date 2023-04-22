#pragma once

#include <cstdlib>
#include <iostream>

#define RING_BUFFER_SIZE_RATIO 3

typedef unsigned int UInt32;

namespace FactorHelpers
{
	//WG - This function breaks down the factor sum into it's factorables...  just for fun.
	//It could be improved by changing the mod to use bit masks, since we don't actually
	//need to know the remainder... but I only use it once to show the results of the 1500
	//test case.
	template<size_t TFactorsLength>
	UInt32 ShowFactors(UInt32 value, const UInt32(&factorableNumbers)[TFactorsLength])
	{
		if (value == 1)
		{
			return 1;
		}

		size_t factorableNumberLength = std::size(factorableNumbers);
		for (UInt32 factorIndex = 0; factorIndex < factorableNumberLength; factorIndex++)
		{
			UInt32 factor = factorableNumbers[factorIndex];
			if (value == factor)
			{
				std::cout << value << "\r\n";
				return value;
			}
			else if (value % factor == 0)
			{
				std::cout << factor << '*';
				return ShowFactors(value / factor, factorableNumbers);
			}
		}

		std::cout << "\nNot factorable!\n";
		return 0;
	}

	//WG - A Ring buffer implementation - allocates an array, then loops around to reuse the freed
	//elements.
	template<typename TDataType>
	struct RingBuffer
	{
	public:
		RingBuffer(size_t BufferSize) : 
			Size(BufferSize)
		{
			Buffer = new TDataType[Size];
		}

		UInt32 GetTail()
		{
			return Tail;
		}

		void Push(TDataType PushedValue)
		{
			Buffer[Head] = PushedValue;
			UInt32 nextIndex = GetNextIndex(Head);
			if (nextIndex == Tail)
			{
				throw std::out_of_range("Ring buffer head met tail! The buffer allocation is too small for it's usage!");
			}
			Head = nextIndex;
		}

		void Pop()
		{
			if (Tail != Head)
			{
				Tail = GetNextIndex(Tail);
			}
			else
			{
				throw std::out_of_range("Ring buffer head met tail! The buffer allocation is too small for it's usage!");
			}
		}

		UInt32 GetNextIndex(UInt32 index)
		{
			return (index == Size) ? 0 : ++index;
		}

		TDataType operator[](UInt32 index) const
		{
			bool IsUnwrappedAndInRange = (Head > Tail) && (index < Head && index >= Tail);
			bool IsWrappedAndInRange = (Head < Tail) && (index < Head || index >= Tail);
			if (!IsUnwrappedAndInRange && !IsWrappedAndInRange)
			{
				throw std::out_of_range("Ring buffer index out of range!");
			}

			return Buffer[index];
		}

	private:
		const size_t Size;
		UInt32 Head = 0;
		UInt32 Tail = 0;
		TDataType* Buffer;
	};

	struct FactorInfo
	{
	public:
		UInt32 NextMultiple;
		UInt32 Factor;
		UInt32 BufferIndex;

		FactorInfo() :
			NextMultiple(1),
			Factor(0),
			BufferIndex(0)
		{ }
	};

	//WG - My original successful implementation used queues to store the list of previous
	//factors, which lived in the FactorInfo.  I swapped to the ring buffer method after
	//polishing the algorithm.  The buffer overhead dropped by about half.

	template<size_t TFactorsLength>
	UInt32 GetNthFactorSum(UInt32 n, const UInt32(&factorableNumbers)[TFactorsLength])
	{
		UInt32 maxFactor = 1;
		FactorInfo factorInfos[TFactorsLength];
		for (UInt32 i = 0; i < TFactorsLength; i++)
		{
			UInt32 factor = factorableNumbers[i];
			factorInfos[i].Factor = factor;
			if (factor > maxFactor)
			{
				maxFactor = factor;
			}
		}

		//WG - This buffer size isn't exact, but it's pretty close.  I based it off observations from the buffers sizes
		//as I was debugging.
		size_t bufferSize = (n / maxFactor) + (maxFactor << 1U);
		RingBuffer<UInt32> buffer(bufferSize);

		UInt32 currentN = 0;
		UInt32 lowestFactorSum = 0;
		while (currentN < n)
		{
			lowestFactorSum = std::numeric_limits<UInt32>::max();
			for (auto& currentFactorInfo : factorInfos)
			{
				UInt32& nextFactor = currentFactorInfo.NextMultiple;
				if (nextFactor < lowestFactorSum)
				{
					lowestFactorSum = nextFactor;
				}
			}

			buffer.Push(lowestFactorSum);

			bool tailUsed = false;
			for (auto& currentFactorInfo : factorInfos)
			{
				UInt32& nextFactor = currentFactorInfo.NextMultiple;
				UInt32& bufferIndex = currentFactorInfo.BufferIndex;
				if (nextFactor == lowestFactorSum)
				{
					nextFactor = buffer[currentFactorInfo.BufferIndex] * currentFactorInfo.Factor;
					bufferIndex = buffer.GetNextIndex(bufferIndex);
				}
				tailUsed |= (bufferIndex == buffer.GetTail());
			}

			if (!tailUsed)
			{
				buffer.Pop();
			}

			currentN++;

			//WG - Uncomment to see the sequence do it's calculations 1 factor at a time.
			//This will however skew your benchmarking about 100x.
			//std::cout << currentN << ": " << lowestFactorSum << "\r\n";
		}
		return lowestFactorSum;
	}
}