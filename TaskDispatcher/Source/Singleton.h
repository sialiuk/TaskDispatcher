#pragma once
#include <boost\noncopyable.hpp>
#include <WinBase.h>
#include "Typedefs.h"

namespace mtd
{
	template<typename T>
	class Singleton
		: private boost::noncopyable
	{
	public:
		static T& Instance()
		{
			static T t;
			return t;
		}

	private:
		Singleton();
		~Singleton() { }
	};


	template<typename T>
	class SingletonLock
		: private boost::noncopyable
	{
	public:
		static T& Instance()
		{
			if (!m_ptr)
			{
				Lock lock(m_mutex);
				if(!m_ptr)
				{
					m_ptr.reset(new T());
				}
			}
			return *m_ptr;
		}

	private:
		SingletonLock() {}

		~SingletonLock() {}
	private:
		static std::unique_ptr<T> m_ptr;
		static Mutex m_mutex;
	};

	template<typename T>
	std::unique_ptr<T> SingletonLock<T>::m_ptr;
	
	template<typename T>
	Mutex SingletonLock<T>::m_mutex;

	
template<typename T>
class SingletonHandler
	: private boost::noncopyable
{
public:
	static T& Instance()
	{
		if(!m_ptr)
		{
			static T tObj;
			if(InterlockedCompareExchangePointer(reinterpret_cast<volatile PVOID*>(&m_ptr), &tObj, nullptr))
			{
				tObj.~T();
			}
		}
		return *m_ptr;
	}
private:
	SingletonHandler() {}
	~SingletonHandler() {}
private:
	static T* m_ptr;
};

template<typename T>
T* SingletonHandler<T>::m_ptr = nullptr;


}