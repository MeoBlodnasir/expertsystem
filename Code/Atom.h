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
		inline	virtual EType			GetType() const override		{ return E_ATOM; }
				virtual	std::string		GetDesc() const override		{ return std::string(1, m_iId); }
				virtual	ILogicElement*	Duplicate() const override;
				virtual bool			operator == (const ILogicElement& oElem) const override;

		// implémentation interface IProposition
				virtual bool	Evaluate(const VariablesManager& oVariablesManager) const override;
				virtual bool	CheckValidity() const override;
				virtual void	GetAtomsId(AtomIdSet* pIdSet) const override;

		inline	Id				GetId() const							{ return m_iId; }
		inline	virtual void	SetId(Id iId)							{ m_iId = iId; }

	protected:

		Id	m_iId;
	};

	class ConstantAtom : public Atom
	{
	public:

		ConstantAtom(bool bState = false) : Atom('*'), m_bState(bState)					{}
		ConstantAtom(Id /*iId*/, bool bState = false) : Atom('*'), m_bState(bState)		{}
		ConstantAtom(const ConstantAtom& oAtom) : Atom('*'), m_bState(oAtom.m_bState)	{}
		virtual ~ConstantAtom()															{}

		inline virtual	ILogicElement*	Duplicate() const override							{ return new ConstantAtom(*this); }
		inline virtual	bool			Evaluate(const VariablesManager&) const override	{ return GetState(); }
		inline			bool			GetState() const									{ return m_bState; }

	private:

		bool	m_bState;
	};
}
