#pragma once

#include "IRuleElement.h"

namespace ft
{
	class Variable : public IRuleElement
	{
	public:

		Variable();
		Variable(bool bState, char cId);
		Variable(const Variable& oVar);
		virtual ~Variable();

		// implémentation interface IRuleElement
		virtual EType	GetType() const					{ return E_VARIABLE; }

		bool	GetState() const						{ return m_bState; }
		char	GetId() const							{ return m_cId; }

		void	SetState(bool bState)					{ m_bState = bState; }
		void	SetId(char cId)							{ m_cId = cId; }

	private:

		bool	m_bState;
		char	m_cId;
	};
}
