#pragma once

#include "IProposition.h"

namespace ft
{
	// fw
	class VariablesManager;

	class Atom : public IProposition
	{
	public:

		typedef	ILogicElement::AtomId	Id;

		Atom();
		Atom(Id iId);
		Atom(const Atom& oAtom);
		virtual ~Atom();

		// impl�mentation interface ILogicElement
		inline	virtual EType			GetType() const override	{ return E_ATOM; }
				virtual	ILogicElement*	Duplicate() const override;

		// impl�mentation interface IProposition
		virtual bool	Evaluate(const VariablesManager& oVariablesManager) const override;
		virtual bool	CheckValidity() const override;
		virtual void	GetAtomsId(AtomIdSet* pIdSet) const override;

		inline Id		GetId() const					{ return m_iId; }
		inline void		SetId(Id iId)					{ m_iId = iId; }

	private:

		Id	m_iId;
	};
}
