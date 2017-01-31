#pragma once

#include "Core.h"
#include "Types.h"
#include "ILogicElement.h"

namespace ft
{
	//////////////////////////////
	// Classes abstraites		//
	//////////////////////////////

	class ALogicOperator : public ILogicElement
	{
	public:

		const uint32	iOperandCount;

		ALogicOperator() : ILogicElement(), iOperandCount(0)												{}
		ALogicOperator(const ALogicOperator& oOp) : ILogicElement(oOp), iOperandCount(oOp.iOperandCount)	{}
		ALogicOperator(uint32 n) : ILogicElement(), iOperandCount(n)										{}
		virtual ~ALogicOperator() override																	{}

		ALogicOperator&	operator = (const ALogicOperator&)		{ return *this; }

		// implémentation interface ILogicElement
		virtual EType			GetType() const override		{ return E_OPERATOR; }
	};

	// Template opérateur
	template <uint32 N>
	class LogicOperatorTemplate : public ALogicOperator
	{
	public:

		LogicOperatorTemplate() : ALogicOperator(N)	{}
		virtual ~LogicOperatorTemplate() override	{}
	};

	// Opérateur unaire
	class AUnaryOperator : public LogicOperatorTemplate<1>
	{
	public:

		AUnaryOperator() : LogicOperatorTemplate<1>()	{}
		virtual ~AUnaryOperator() override				{}

		virtual bool	Evaluate(bool b) const = 0;
	};

	// Opérateur binaire
	class ABinaryOperator : public LogicOperatorTemplate<2>
	{
	public:

		ABinaryOperator() : LogicOperatorTemplate<2>()	{}
		virtual ~ABinaryOperator() override				{}

		virtual bool	Evaluate(bool b1, bool b2) const = 0;
	};

	//////////////////////////////
	// Classes instanciables	//
	//////////////////////////////

	// Non
	class OperatorNOT : public AUnaryOperator
	{
	public:

		OperatorNOT() : AUnaryOperator()	{}
		virtual ~OperatorNOT() override		{}

		// implémentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "!"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorNOT(); }
		// implémentation interface AUnaryOperator
		virtual bool			Evaluate(bool b) const override				{ return !b; }
	};

	// Et
	class OperatorAND : public ABinaryOperator
	{
	public:

		OperatorAND() : ABinaryOperator()	{}
		virtual ~OperatorAND() override		{}

		// implémentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "+"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorAND(); }
		// implémentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 && b2; }
	};

	// Ou
	class OperatorOR : public ABinaryOperator
	{
	public:

		OperatorOR() : ABinaryOperator()	{}
		virtual ~OperatorOR() override		{}

		// implémentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "|"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorOR(); }
		// implémentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 || b2; }
	};

	// Ou exclusif
	class OperatorXOR : public ABinaryOperator
	{
	public:

		OperatorXOR() : ABinaryOperator()	{}
		virtual ~OperatorXOR() override		{}

		// implémentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "^"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorXOR(); }
		// implémentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 ^ b2; }
	};

	// Implique
	class OperatorIMP : public ABinaryOperator
	{
	public:

		OperatorIMP() : ABinaryOperator()	{}
		virtual ~OperatorIMP() override		{}

		// implémentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "=>"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorIMP(); }
		// implémentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return (!b1) || b2; }
	};

	// Equivalence (bidirectionnal implication)
	class OperatorBIMP : public ABinaryOperator
	{
	public:

		OperatorBIMP() : ABinaryOperator()	{}
		virtual ~OperatorBIMP() override	{}

		// implémentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "<=>"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorBIMP(); }
		// implémentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 == b2; }
	};
}
