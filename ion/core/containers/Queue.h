///////////////////////////////////////////////////
// File:		Queue.h
// Date:		22nd December 2013
// Authors:		Matt Phillips
// Description:	Lock-free single producer, single consumer queue
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/thread/Atomic.h"

namespace ion
{
	template <typename T, int SIZE> class Queue
	{
	public:
		Queue();
		~Queue();

		void Push(T& item);
		T Pop();

		bool IsEmpty() const;
		bool IsFull() const;

	private:
		T mItems[SIZE];
		u32 mProducerIdx;
		u32 mConsumerIdx;
	};

	template <typename T, int SIZE> Queue<T, SIZE>::Queue()
	{
		mProducerIdx = 0;
		mConsumerIdx = 0;
	}

	template <typename T, int SIZE> Queue<T, SIZE>::~Queue()
	{

	}

	template <typename T, int SIZE> void Queue<T, SIZE>::Push(T& item)
	{
		int index = mProducerIdx % SIZE;
		mItems[index] = item;
		thread::atomic::Increment(mProducerIdx);
	}

	template <typename T, int SIZE> T Queue<T, SIZE>::Pop()
	{
		int index = mConsumerIdx % SIZE;
		T item = mItems[index];
		thread::atomic::Increment(mConsumerIdx);

		return item;
	}

	template <typename T, int SIZE> bool Queue<T, SIZE>::IsEmpty() const
	{
		return mConsumerIdx == mProducerIdx;
	}

	template <typename T, int SIZE> bool Queue<T, SIZE>::IsFull() const
	{
		return (mProducerIdx - mConsumerIdx) == SIZE;
	}
}