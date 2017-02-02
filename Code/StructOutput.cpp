
#include "StructOutput.h"

#include "Rule.h"
#include "LogicOperator.h"

namespace ft
{
	std::ostream&	operator << (std::ostream& oOs, const Rule& oRule)
	{
		oOs << oRule.m_oAntecedent.GetDesc() << ' ';
		if (oRule.m_bIsBidirectionnal)
			oOs << OperatorBIMP().GetDesc();
		else
			oOs << OperatorIMP().GetDesc();
		oOs << ' ' << oRule.m_oConsequent.GetDesc();

		return oOs;
	}
}
