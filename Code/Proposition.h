#pragma once

#include "IProposition.h"
#include "StrongPointer.h"

#include <vector>

namespace ft
{
	// fw
	class Atom;

	class Proposition : public IProposition
	{
	public:

		Proposition();
		Proposition(const Proposition& oProposition);
		Proposition(const Atom& oAtom);
		virtual ~Proposition() override;

		// Implémentation de l'interface ILogicElement
		virtual	std::string		GetDesc() const override;
		virtual ILogicElement*	Duplicate() const override;

		// Implémentation de l'interface IProposition
		virtual bool	Evaluate(const VariablesManager& pVariablesManager) const override;
		virtual bool	CheckValidity() const override;
		virtual void	GetAtomsId(AtomIdSet* pIdSet) const override;

		void			AddElement(const ILogicElement& oElement);
		void			ReplaceAtom(ILogicElement::AtomId iId, const IProposition& oNew);

		bool			XorPresent() const;
		bool			AndPresent() const;
		bool			OrPresent() const;
		bool			NotPresent() const;

	private:

		std::vector< SPtr<ILogicElement> >	m_oElements;
	};
}
