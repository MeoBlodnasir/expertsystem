
#include "Variable.h"

namespace ft
{
	Variable::Variable()
		: m_bState(false)
		, m_cId(0)
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
