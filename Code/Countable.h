#pragma once

#include "Core.h"
#include "Types.h"

namespace ft
{
	// Classe de référence dénombrable

	class Countable
	{
	protected:

		Countable() : m_iReferencesCount(0)								{}
		virtual ~Countable()											{}

	public:

		Countable(const Countable&) : m_iReferencesCount(0)				{}

		inline virtual Countable&	operator = (const Countable&)		{ m_iReferencesCount = 0; return *this; }

		inline virtual void			AddReference()						{ ++m_iReferencesCount; }
		inline virtual void			Release()							{ --m_iReferencesCount; FT_ASSERT((int)m_iReferencesCount >= 0); }
		inline uint32				GetReferenceCount() const			{ return m_iReferencesCount; }

	private:

		uint32 m_iReferencesCount;
	};
}
