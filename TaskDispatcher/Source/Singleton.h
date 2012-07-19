#pragma once
#include <boost\noncopyable.hpp>
#include <WinBase.h>
#include <boost/detail/interlocked.hpp>
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
					T* temp = new T();
					m_ptr.reset(temp);
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
				T* tObj = new T(); 
				if(InterlockedCompareExchangePointer(reinterpret_cast<volatile PVOID*>(&m_ptr), tObj, nullptr))
				{
					delete tObj;
				}
				else
				{
					atexit(Destroy);
				}
			}
			return *m_ptr;
		}
	private:
		static void Destroy()
		{
			delete m_ptr;
		}
		SingletonHandler() {}
		~SingletonHandler() {}
	private:
		static T* m_ptr;
	};

	template<typename T>
	T* SingletonHandler<T>::m_ptr = nullptr;

}