#pragma once

#include "Proposition.h"

namespace ft
{
	// Une Rule est définie par: Antécédent [<]=> Conséquence

	class Rule
	{
	public:

		Rule();
		Rule(const Rule& oRule);
		~Rule();

		inline const Proposition&	GetConsequent() const		{ return m_oConsequent; }
		inline const Proposition&	GetAntecedent() const		{ return m_oAntecedent; }
		ILogicElement::AtomId		GetConsequentFirstAtomId() const;

		void			SetAntecedentProposition(const Proposition& oProposition)	{ m_oAntecedent = oProposition; }
		void			SetConsequentProposition(const Proposition& oProposition)	{ m_oConsequent = oProposition; }

		void			AddAntecedentElement(const ILogicElement& oElement);
		void			AddConsequentElement(const ILogicElement& oElement);

		inline void		SetBidirectionnal(bool bValue)			{ m_bIsBidirectionnal = bValue; }
		inline bool		IsBidirectionnal() const				{ return m_bIsBidirectionnal; }

		bool			CheckComponentsValidity() const;
		bool			CheckUnacceptedConditions() const;
	private:

		Proposition		m_oAntecedent;
		Proposition		m_oConsequent;
		bool			m_bIsBidirectionnal;

		friend std::ostream& operator << (std::ostream& oOs, const Rule& oRule);
	};
}
