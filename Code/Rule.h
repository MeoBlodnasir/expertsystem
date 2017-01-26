#pragma once

#include "Proposition.h"

namespace ft
{
	// Une Rule est d�finie par: Ant�c�dent => Cons�quence

	class Rule
	{
	public:

		Rule();
		Rule(const Rule& oRule);
		~Rule();

		inline const Proposition&	GetConsequent() const		{ return m_oConsequent; }
		inline const Proposition&	GetAntecedent() const		{ return m_oAntecedent; }
		ILogicElement::AtomId		GetConsequentAtomId() const;

		void		AddAntecedentElement(const ILogicElement& oElement);
		void		AddConsequentElement(const ILogicElement& oElement);

	private:

		Proposition		m_oAntecedent;
		Proposition		m_oConsequent;
	};
}
