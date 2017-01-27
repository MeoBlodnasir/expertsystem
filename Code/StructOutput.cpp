
#include "StructOutput.h"

#include "Atom.h"
#include "Proposition.h"
#include "Rule.h"
#include "LogicOperator.h"

namespace ft
{
	std::ostream&	operator << (std::ostream& oOs, const Atom& oAtom)
	{
		oOs << oAtom.GetId();
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const Proposition& oProposition)
	{
		for (std::vector<ILogicElement*>::const_iterator itElem = oProposition.m_oElements.begin(), itEnd = oProposition.m_oElements.end(); itElem != itEnd; ++itElem)
		{
			if ((*itElem)->GetType() == ILogicElement::E_ATOM)
				oOs << *dynamic_cast<const Atom*>(*itElem);
			else if ((*itElem)->GetType() == ILogicElement::E_PROPOSITION)
				oOs << *dynamic_cast<const Proposition*>(*itElem);
			else if ((*itElem)->GetType() == ILogicElement::E_OPERATOR)
			{
				const OperatorNOT*	pNot	= nullptr;
				const OperatorAND*	pAnd	= nullptr;
				const OperatorOR*	pOr		= nullptr;
				const OperatorXOR*	pXor	= nullptr;
				const OperatorIMP*	pImp	= nullptr;
				const OperatorBIMP*	pBimp	= nullptr;

				if ((pNot = dynamic_cast<const OperatorNOT*>(*itElem)))
					oOs << *pNot;
				else if ((pAnd = dynamic_cast<const OperatorAND*>(*itElem)))
					oOs << *pAnd;
				else if ((pOr = dynamic_cast<const OperatorOR*>(*itElem)))
					oOs << *pOr;
				else if ((pXor = dynamic_cast<const OperatorXOR*>(*itElem)))
					oOs << *pXor;
				else if ((pImp = dynamic_cast<const OperatorIMP*>(*itElem)))
					oOs << *pImp;
				else if ((pBimp = dynamic_cast<const OperatorBIMP*>(*itElem)))
					oOs << *pBimp;
			}
		}
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const Rule& oRule)
	{
		oOs << oRule.m_oAntecedent << ' ';

		if (oRule.m_bIsBidirectionnal)
			oOs << OperatorBIMP();
		else
			oOs << OperatorIMP();

		oOs << ' ' << oRule.m_oConsequent;
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const OperatorNOT& oOp)
	{
		oOs << '!';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const OperatorAND& oOp)
	{
		oOs << '+';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const OperatorOR& oOp)
	{
		oOs << '|';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const OperatorXOR& oOp)
	{
		oOs << '^';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const OperatorIMP& oOp)
	{
		oOs << "=>";
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const OperatorBIMP& oOp)
	{
		oOs << "<=>";
		return oOs;
	}
}
