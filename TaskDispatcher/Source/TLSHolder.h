#pragma once
#include <stdexcept>
#include <Windows.h>

namespace mtd
{
	template<typename T>
	class TlsHolder
	{
	public:
		TlsHolder()
			: m_index(TlsAlloc())
		{
			if(m_index == TLS_OUT_OF_INDEXES)
			{
				throw std::runtime_error("Error TlsAlloc");
			}
		}

		~TlsHolder()
		{
			TlsFree(m_index);
		}

		BOOL SetValue(T* ptr)
		{
			return TlsSetValue(m_index, ptr);
		}

		T* GetValue()
		{
			return static_cast<T*>(TlsGetValue(m_index));
		}

	private:
		TlsHolder(const TlsHolder&);
		TlsHolder& operator=(const TlsHolder&);
	private:
		DWORD m_index;
	};
}
