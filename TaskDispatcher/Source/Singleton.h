#pragma once
#include <boost\noncopyable.hpp>
#include <WinBase.h>
#include <boost/detail/interlocked.hpp>
#include "Typedefs.h"

namespace mtd
{
	template<typename T>
	class CreateWithNew
		: private boost::noncopyable
	{
	public:
		static T* Create()
		{
			return new T();
		}
		static void Destroy(T* ptr)
		{
			delete ptr;
		}
	private:
		CreateWithNew();
		~CreateWithNew();
	};

	template<typename T>
	class CreateStaticVariable
		: private boost::noncopyable
	{
	public:
		static T* Create()
		{
			static T t;
			return &t;
		}
		static void Destroy(T*)
		{
		}
	private:
		CreateStaticVariable();
		~CreateStaticVariable();
	};

	template<typename T>
	class SingleThreaded
		: private boost::noncopyable
	{
	public:
		typedef T Type;

		static bool Initialize(T*& ptr, T* (*Func)())
		{
			ptr = Func();
			return true;
		}
	private:
		SingleThreaded();
		~SingleThreaded();
	};

	template<typename T>
	class MultiThreadedLockFree
		: private boost::noncopyable
	{
	public:
		typedef volatile T Type;

		static bool Initialize(volatile T*& ptr, T* (*Func)())
		{
			T* temp = Func();
			if(InterlockedCompareExchangePointer(reinterpret_cast<volatile PVOID*>(const_cast<T**>(&ptr)), temp, nullptr))
			{
				delete temp;
				return false;
			}
			return true;
		}
	private:
		MultiThreadedLockFree();
		~MultiThreadedLockFree();
	};

	template<typename T>
	class MultiThreadedMutexLock
		: private boost::noncopyable
	{
	public:
		typedef volatile T Type;

		static bool Initialize(volatile T*& ptr, T* (*Func)())
		{
			boost::unique_lock<boost::mutex> lock(s_mutex);
			if(!ptr)
			{
				T* temp = Func();
				ptr = temp;
				return true;
			}
			return false;
		}
	private:
		MultiThreadedMutexLock();
		~MultiThreadedMutexLock();
	private:
		static boost::mutex s_mutex;
	};

	template<typename T>
	boost::mutex MultiThreadedMutexLock<T>::s_mutex;


	template<typename T
			, template<typename> class CreateModel = CreateWithNew
			, template<typename> class ThreadingModel = SingleThreaded>
	class SingletonHolder
		: private boost::noncopyable
	{
	public:
		static T& Instance()
		{
			if(!m_inst)
			{
				if(ThreadingModel<T>::Initialize(m_inst, CreateModel<T>::Create));
				{
					atexit(Destroy);
				}	
			}
			return const_cast<T&>(*m_inst);
		}
	private:
		static void Destroy()
		{
			CreateModel<T>::Destroy(const_cast<T*>(m_inst));
		}

		SingletonHolder();
		~SingletonHolder();
	private:
		typedef typename ThreadingModel<T>::Type InstanceType;
		static InstanceType* m_inst;
	};

	template<typename T
			, template<typename> class CreateModel
			, template<typename> class ThreadingModel>
	typename SingletonHolder<T, CreateModel, ThreadingModel>::InstanceType* SingletonHolder<T, CreateModel, ThreadingModel>::m_inst = nullptr;




}