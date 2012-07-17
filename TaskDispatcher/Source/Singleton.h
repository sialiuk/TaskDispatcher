#pragma once
#include <boost\noncopyable.hpp>

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
class SingletonWithPointer
	: private boost::noncopyable
{
public:
	static T& Instance()
	{
		if (!m_ptr)
		{
			m_ptr.reset(new T());
		}
		return *m_ptr;
	}

private:
	SingletonWithPointer()
	{
	}

	~SingletonWithPointer()
	{
	}
private:
	static std::unique_ptr<T> m_ptr;
};

template<typename T>
std::unique_ptr<T> SingletonWithPointer<T>::m_ptr;