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

		enum EState
		{
			E_UNDEF = -1,
			E_FALSE = 0,
			E_TRUE = 1
		};

		Variable();
		Variable(Id iId, EState eState);
		Variable(const Variable& oVar);
		virtual ~Variable();

		// implémentation interface IRuleElement
		virtual EType	GetType() const					{ return E_VARIABLE; }

		inline Id		GetId() const					{ return m_iId; }
		inline EState	GetState() const				{ return m_eState; }

	private:
		friend class VariablesManager;

		Id		m_iId;
		EState	m_eState;
	};
}
