
#include "Variable.h"

namespace ft
{
	Variable::Variable()
		: m_bState(false)
		, m_iId(0)
	{
	}

	Variable::Variable(bool bState, Id iId)
		: m_bState(bState)
		, m_iId(iId)
	{
	}

	Variable::Variable(const Variable& oVar)
		: m_bState(oVar.m_bState)
		, m_iId(oVar.m_iId)
	{
	}

	Variable::~Variable()
	{
	}
}
