
#include "Variable.h"

namespace ft
{
	Variable::Variable()
		: m_iId(0)
		, m_eState(E_UNDEF)
	{
	}

	Variable::Variable(Id iId, EState eState)
		: m_iId(iId)
		, m_eState(eState)
	{
	}

	Variable::Variable(const Variable& oVar)
		: m_iId(oVar.m_iId)
		, m_eState(oVar.m_eState)
	{
	}

	Variable::~Variable()
	{
	}
}
