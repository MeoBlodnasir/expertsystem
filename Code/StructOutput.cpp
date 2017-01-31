
#include "StructOutput.h"

#include "Atom.h"
#include "Proposition.h"
#include "Rule.h"
#include "LogicOperator.h"

#include <string>

namespace ft
{
	std::ostream&	operator << (std::ostream& oOs, const Rule& oRule)
	{
		oOs << "L:  " << oRule.sLitteralExpression << "     ";

		oOs << "P:  " << oRule.m_oAntecedent.GetDesc() << ' ';
		if (oRule.m_bIsBidirectionnal)
			oOs << OperatorBIMP().GetDesc();
		else
			oOs << OperatorIMP().GetDesc();
		oOs << ' ' << oRule.m_oConsequent.GetDesc();

		return oOs;
	}
}
