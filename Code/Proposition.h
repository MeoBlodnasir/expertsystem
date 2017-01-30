#pragma once

#include "IProposition.h"
#include "StrongPointer.h"

#include <vector>

namespace ft
{
	class Proposition : public IProposition
	{
	public:

		Proposition();
		Proposition(const Proposition& oProposition);
		virtual ~Proposition() override;

		// Implémentation de l'interface ILogicElement
		virtual ILogicElement*	Duplicate() const override;

		// Implémentation de l'interface IProposition
		virtual bool	Evaluate(const VariablesManager& pVariablesManager) const override;
		virtual bool	CheckValidity() const override;
		virtual void	GetAtomsId(AtomIdSet* pIdSet) const override;

		void			AddElement(const ILogicElement& oElement);
		void			ReplaceAtom(ILogicElement::AtomId iId, const IProposition& oNew);

	private:

		std::vector< SPtr<ILogicElement> >	m_oElements;

		friend std::ostream& operator << (std::ostream& oOs, const Proposition& oProposition);
	};
}
