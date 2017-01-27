#pragma once

namespace ft
{
	//////////////////////////////
	// Interface				//
	//////////////////////////////

	// Element de base d'une Proposition
	// les classes Operator et Proposition en héritent

	class ILogicElement
	{
	public:

		enum EType
		{
			E_NONE = -1,
			E_OPERATOR = 0,
			E_PROPOSITION,
			E_ATOM
		};

		typedef char	AtomId;

		ILogicElement()						{}
		ILogicElement(const ILogicElement&)	{}
		virtual ~ILogicElement()			{}

		virtual	EType			GetType() const = 0;
		virtual	ILogicElement*	Duplicate() const = 0;
		virtual	void				Print() const = 0;
	};
}
