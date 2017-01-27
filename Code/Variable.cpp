
#include "Variable.h"

namespace ft
{
	Variable::Variable()
		: m_iId(0)
		, m_bState(false)
	{
	}

	Variable::Variable(Id iId, bool bState /*= false*/)
		: m_iId(iId)
		, m_bState(bState)
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
