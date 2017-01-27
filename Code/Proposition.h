#pragma once

#include "IProposition.h"

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
		virtual bool	SelfAssert() const override;
		virtual void	GetAtomsId(AtomIdSet* pIdSet) const override;

		void			AddElement(const ILogicElement& oElement);
		void			ReplaceAtom(ILogicElement::AtomId iId, const IProposition& oNew);
		virtual void			Print() const override;

	private:

		std::vector<ILogicElement*>	m_oElements;
	};
}
