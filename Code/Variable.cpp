
#include "Variable.h"

namespace ft
{
	Variable::Variable()
		: m_bState(false)
		, m_cId(0)
	{
	}

	Variable::Variable(bool bState, char cId)
		: m_bState(bState)
		, m_cId(cId)
	{
	}

	Variable::Variable(const Variable& oVar)
		: m_bState(oVar.m_bState)
		, m_cId(oVar.m_cId)
	{
	}

	Variable::~Variable()
	{
	}
}
