#pragma once

#include "IRuleElement.h"

namespace ft
{
	class Variable : public IRuleElement
	{
	public:

		typedef	char	Id;

		Variable();
		Variable(bool bState, Id iId);
		Variable(const Variable& oVar);
		virtual ~Variable();

		// implémentation interface IRuleElement
		virtual EType	GetType() const					{ return E_VARIABLE; }

		bool	GetState() const						{ return m_bState; }
		Id		GetId() const							{ return m_iId; }

		void	SetState(bool bState)					{ m_bState = bState; }
		void	SetId(Id iId)							{ m_iId = iId; }

	private:

		bool	m_bState;
		Id		m_iId;
	};
}
