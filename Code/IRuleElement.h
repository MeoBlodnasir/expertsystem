#pragma once

namespace ft
{
	//////////////////////////////
	// Interface				//
	//////////////////////////////

	// Element de base de la RuleStack
	// les classes Operator et Variable en héritent

	class IRuleElement
	{
	public:

		enum EType
		{
			E_NONE = -1,
			E_OPERATOR = 0,
			E_VARIABLE
		};

		IRuleElement()			{}
		virtual ~IRuleElement()	{}

		virtual EType	GetType() const = 0;
	};
}
