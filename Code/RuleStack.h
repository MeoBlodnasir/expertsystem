#pragma once

#include <vector> // deque, queue, stack ?
#include "Variable.h"

namespace ft
{
	// fw
	class IRuleElement;

	//////////////////////////////
	// Classe abstraite			//
	//////////////////////////////

	class ARuleStack
	{
		public:

			ARuleStack();
			virtual ~ARuleStack();

			virtual bool	Evaluate() const = 0;
			virtual bool	SelfAssert() const = 0;

			void			AddElement(const IRuleElement* pElement);

		protected:

			std::vector<const IRuleElement*>	m_oElements;
	};

	//////////////////////////////
	// Classes instanciables	//
	//////////////////////////////

	class ConditionRuleStack : public ARuleStack
	{
		public:

			ConditionRuleStack();
			virtual ~ConditionRuleStack();

			virtual bool	Evaluate() const;
			virtual bool	SelfAssert() const;
	};

	class ResultRuleStack : public ARuleStack
	{
		public:

			ResultRuleStack();
			virtual ~ResultRuleStack();

			Variable::Id	GetTopVariableId() const;
			virtual bool	Evaluate() const;
			virtual bool	SelfAssert() const;
	};
}
