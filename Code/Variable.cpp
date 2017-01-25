
#include "Variable.h"

namespace ft
{
	Variable::Variable()
		: m_iId(0)
		, m_bState(false)
		, m_bLock(false)
	{
	}

	Variable::Variable(Id iId, bool bState, bool bLock)
		: m_iId(iId)
		, m_bState(bState)
		, m_bLock(bLock)
	{
	}

	Variable::Variable(const Variable& oVar)
		: m_iId(oVar.m_iId)
		, m_bState(oVar.m_bState)
	{
	}

	Variable::~Variable()
	{
	}
}
