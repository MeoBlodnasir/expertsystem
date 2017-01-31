#pragma once

#include "Countable.h"

// Macro pratique
#if defined(SPtr)
#	undef SPtr
#endif
#define SPtr	ft::StrongPtr

namespace ft
{
	//FT_TODO("Deplacer la fonction Swap ailleurs")
	template <typename T>
	static inline void	Swap(T* const p1, T* const p2)
	{
		FT_ASSERT(p1 != nullptr);
		FT_ASSERT(p2 != nullptr);

		const T oTmp = *p1;
		*p1 = *p2;
		*p2 = oTmp;
	}

	// Référence dénombrable autodestructrice pour Strong Pointers

	class CountableSPtr : public Countable
	{
	public:
#ifdef __FT_DEBUG__
		#define FT_COUNTABLESPTR_TEST_VAL	0x600D1007
		virtual uint32	VerifCountableSPtr() const	{ return FT_COUNTABLESPTR_TEST_VAL; }
#endif
		virtual void	Release() override			{ Countable::Release(); if ((int)GetReferenceCount() <= 0) delete this; }
	};

	// Pointeur à référence dénombrable
	// Mémoire allouée avec "new" uniquement ____________________________________________________________________^

	template <class T>
	class StrongPtr
	{
	public:
		
		StrongPtr() : m_pData(nullptr)									{}
		StrongPtr(T* p) : m_pData(p)									{ AddReference_Internal((CountableSPtr*)(m_pData)); }
		StrongPtr(const StrongPtr<T>& xOther) : m_pData(xOther.m_pData)	{ AddReference_Internal((CountableSPtr*)(m_pData)); }
		template <class U>
		StrongPtr(const StrongPtr<U>& xOther) : m_pData(xOther.m_pData)	{ AddReference_Internal((CountableSPtr*)(m_pData)); }

		~StrongPtr()													{ Release_Internal((CountableSPtr*)(m_pData)); }

		StrongPtr& operator = (const StrongPtr<T>& xRhs)				{ StrongPtr<T> xTemp(xRhs); Swap(&m_pData, &xTemp.m_pData); return *this; }
		StrongPtr& operator = (T* pRhs)									{ StrongPtr<T> xTemp(pRhs); Swap(&m_pData, &xTemp.m_pData); return *this; }
		template <class U> 
		StrongPtr& operator = (const StrongPtr<U>& xRhs)				{ StrongPtr<T> xTemp(xRhs); Swap(&m_pData, &xTemp.m_pData); return *this; }

		inline		operator T* () const								{ return static_cast<T*>(m_pData); }
		inline T&	operator *  () const								{ FT_ASSERT(m_pData != nullptr); return *m_pData; }
		inline T*	operator -> () const								{ FT_ASSERT(m_pData != nullptr); return  m_pData; }
		bool		operator !  () const								{ return m_pData == nullptr; }

		inline T*	Get() const											{ return m_pData; }
		inline void	Release()											{ *this = nullptr; }

	private:

		T*	m_pData;

		static inline void	AddReference_Internal(CountableSPtr* p)
		{
			if (p != nullptr)
			{
				FT_ASSERT(p->VerifCountableSPtr() == FT_COUNTABLESPTR_TEST_VAL);
				p->AddReference();
			}
		}

		static inline void	Release_Internal(CountableSPtr* p)
		{
			if (p != nullptr)
			{
				FT_ASSERT(p->VerifCountableSPtr() == FT_COUNTABLESPTR_TEST_VAL);
				p->Release();
			}
		}
	};
}

template <class T, class U> inline bool operator == (const SPtr<T>& a, const SPtr<U>& b )	{ return a.Get() == b.Get(); }
template <class T, class U> inline bool operator != (const SPtr<T>& a, const SPtr<U>& b )	{ return a.Get() != b.Get(); }

template <class T, class U> inline bool operator == (const SPtr<T>& a, U* b )				{ return a.Get() == b; }
template <class T, class U> inline bool operator != (const SPtr<T>& a, U* b )				{ return a.Get() != b; }

template <class T, class U> inline bool operator == (T* a, const SPtr<U>& b )				{ return a == b.Get(); }
template <class T, class U> inline bool operator != (T* a, const SPtr<U>& b )				{ return a != b.Get(); }

template <class T>			inline bool operator <  (const SPtr<T>& a, const SPtr<T>& b)	{ return a.Get() < b.Get(); }