#pragma once

#include "Operator.h"

namespace ft
{
	class OperatorsProvider
	{
	public:

		OperatorsProvider();
		~OperatorsProvider();
		
		inline const OperatorNOT*	Not() const			{ return &m_oNot; }
		inline const OperatorAND*	And() const			{ return &m_oAnd; }
		inline const OperatorOR*	Or() const			{ return &m_oOr; }
		inline const OperatorXOR*	Xor() const			{ return &m_oXor; }

	private:

		OperatorNOT	m_oNot;
		OperatorAND	m_oAnd;
		OperatorOR	m_oOr;
		OperatorXOR	m_oXor;
	};
}
