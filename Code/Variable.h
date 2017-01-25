#pragma once

#include "IRuleElement.h"

namespace ft
{
	// fw
	class VariablesManager;

	class Variable : public IRuleElement
	{
	public:

		typedef	char	Id;

		Variable();
		Variable(Id iId, bool bState, bool bLock);
		Variable(const Variable& oVar);
		virtual ~Variable();

		// implémentation interface IRuleElement
		virtual EType	GetType() const					{ return E_VARIABLE; }

		inline Id		GetId() const					{ return m_iId; }
		inline bool	GetState() const				{ return m_bState; }
		inline bool	IsLocked() const				{ return m_bLock; }

	private:
		friend class VariablesManager;

		Id		m_iId;
		bool	m_bState;
		bool	m_bLock;
	};
}
