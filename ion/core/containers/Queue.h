///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
		T m_items[SIZE];
		u32 m_producerIdx;
		u32 m_consumerIdx;
	};

	template <typename T, int SIZE> Queue<T, SIZE>::Queue()
	{
		m_producerIdx = 0;
		m_consumerIdx = 0;
	}

	template <typename T, int SIZE> Queue<T, SIZE>::~Queue()
	{

	}

	template <typename T, int SIZE> void Queue<T, SIZE>::Push(T& item)
	{
		int index = m_producerIdx % SIZE;
		m_items[index] = item;
		thread::atomic::Increment(m_producerIdx);
	}

	template <typename T, int SIZE> T Queue<T, SIZE>::Pop()
	{
		int index = m_consumerIdx % SIZE;
		T item = m_items[index];
		thread::atomic::Increment(m_consumerIdx);

		return item;
	}

	template <typename T, int SIZE> bool Queue<T, SIZE>::IsEmpty() const
	{
		return m_consumerIdx == m_producerIdx;
	}

	template <typename T, int SIZE> bool Queue<T, SIZE>::IsFull() const
	{
		return (m_producerIdx - m_consumerIdx) == SIZE;
	}
}