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

		// impl�mentation interface ILogicElement
		virtual EType			GetType() const override		{ return E_OPERATOR; }
	};

	// Template op�rateur
	template <uint32 N>
	class LogicOperatorTemplate : public ALogicOperator
	{
	public:

		LogicOperatorTemplate() : ALogicOperator(N)	{}
		virtual ~LogicOperatorTemplate() override	{}
	};

	// Op�rateur unaire
	class AUnaryOperator : public LogicOperatorTemplate<1>
	{
	public:

		AUnaryOperator() : LogicOperatorTemplate<1>()	{}
		virtual ~AUnaryOperator() override				{}

		virtual bool	Evaluate(bool b) const = 0;
	};

	// Op�rateur binaire
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

		// impl�mentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "!"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorNOT(); }
		// impl�mentation interface AUnaryOperator
		virtual bool			Evaluate(bool b) const override				{ return !b; }
	};

	// Et
	class OperatorAND : public ABinaryOperator
	{
	public:

		OperatorAND() : ABinaryOperator()	{}
		virtual ~OperatorAND() override		{}

		// impl�mentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "+"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorAND(); }
		// impl�mentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 && b2; }
	};

	// Ou
	class OperatorOR : public ABinaryOperator
	{
	public:

		OperatorOR() : ABinaryOperator()	{}
		virtual ~OperatorOR() override		{}

		// impl�mentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "|"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorOR(); }
		// impl�mentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 || b2; }
	};

	// Ou exclusif
	class OperatorXOR : public ABinaryOperator
	{
	public:

		OperatorXOR() : ABinaryOperator()	{}
		virtual ~OperatorXOR() override		{}

		// impl�mentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "^"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorXOR(); }
		// impl�mentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 ^ b2; }
	};

	// Implique
	class OperatorIMP : public ABinaryOperator
	{
	public:

		OperatorIMP() : ABinaryOperator()	{}
		virtual ~OperatorIMP() override		{}

		// impl�mentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "=>"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorIMP(); }
		// impl�mentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return (!b1) || b2; }
	};

	// Equivalence (bidirectionnal implication)
	class OperatorBIMP : public ABinaryOperator
	{
	public:

		OperatorBIMP() : ABinaryOperator()	{}
		virtual ~OperatorBIMP() override	{}

		// impl�mentation interface ILogicElement
		virtual	std::string		GetDesc() const override					{ return "<=>"; }
		virtual	ILogicElement*	Duplicate() const override					{ return new OperatorBIMP(); }
		// impl�mentation interface ABinaryOperator
		virtual bool			Evaluate(bool b1, bool b2) const override	{ return b1 == b2; }
	};
}
