#pragma once

#include "Output.h"

namespace ft
{
	// fw
	class Atom;
	class Proposition;
	class Rule;
	class OperatorNOT;
	class OperatorAND;
	class OperatorOR;
	class OperatorXOR;
	class OperatorIMP;
	class OperatorBIMP;

	std::ostream&	operator << (std::ostream& oOs, const Atom& oAtom);
	std::ostream&	operator << (std::ostream& oOs, const Proposition& oProposition);
	std::ostream&	operator << (std::ostream& oOs, const Rule& oRule);
	std::ostream&	operator << (std::ostream& oOs, const OperatorNOT& oOp);
	std::ostream&	operator << (std::ostream& oOs, const OperatorAND& oOp);
	std::ostream&	operator << (std::ostream& oOs, const OperatorOR& oOp);
	std::ostream&	operator << (std::ostream& oOs, const OperatorXOR& oOp);
	std::ostream&	operator << (std::ostream& oOs, const OperatorIMP& oOp);
	std::ostream&	operator << (std::ostream& oOs, const OperatorBIMP& oOp);
}
