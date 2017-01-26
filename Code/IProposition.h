#pragma once

#include "ILogicElement.h"

#include <unordered_set>

namespace ft
{
	// fw
	class VariablesManager;

	typedef	std::unordered_set<ILogicElement::AtomId>	AtomIdSet;

	//////////////////////////////
	// Interface				//
	//////////////////////////////

	// Element de base d'une Proposition
	// les classes Proposition et Atom en héritent

	class IProposition : public ILogicElement
	{
	public:

		IProposition() : ILogicElement()								{}
		IProposition(const IProposition& oProp) : ILogicElement(oProp)	{}
		virtual ~IProposition()											{}

		inline virtual EType	GetType() const override	{ return E_PROPOSITION; }

		virtual bool			Evaluate(const VariablesManager*) const = 0; // Ce serait bien que ça soit const le VariablesManager
		virtual bool			SelfAssert() const = 0;
		virtual void			GetAtomsId(AtomIdSet* pIdSet) const = 0;
	};
}
