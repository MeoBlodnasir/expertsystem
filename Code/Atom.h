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

		// implémentation interface ILogicElement
		inline	virtual EType			GetType() const override	{ return E_ATOM; }
				virtual	std::string		GetDesc() const override	{ return std::string(1, m_iId); }
				virtual	ILogicElement*	Duplicate() const override;

		// implémentation interface IProposition
				virtual bool	Evaluate(const VariablesManager& oVariablesManager) const override;
				virtual bool	CheckValidity() const override;
				virtual void	GetAtomsId(AtomIdSet* pIdSet) const override;

		inline	Id				GetId() const						{ return m_iId; }
		inline	virtual void	SetId(Id iId)						{ m_iId = iId; }

	protected:

		Id	m_iId;
	};

	template <bool bValue, Atom::Id iDesc>
	class ConstantAtom : public Atom
	{
	public:

		ConstantAtom() : Atom(iDesc)			{}
		ConstantAtom(Id) : Atom(iDesc)			{}
		ConstantAtom(const Atom&) : Atom(iDesc)	{}
		virtual ~ConstantAtom();

		inline virtual bool	Evaluate(const VariablesManager&) const override	{ return bValue; }

		inline virtual void	SetId(Id) override	{}
	};

	class AtomTrue : public ConstantAtom<true, '*'>
	{
	};

	class AtomFalse : public ConstantAtom<false, '-'>
	{
	};
}
