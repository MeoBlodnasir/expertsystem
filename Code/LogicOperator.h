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

		inline ALogicOperator&	operator = (const ALogicOperator&)	{ return *this; }

		// impl�mentation interface ILogicElement
		inline virtual EType	GetType() const override			{ return E_OPERATOR; }
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
	// --------
	//	A	!A
	// --------
	//	T	F
	//	F	T
	class OperatorNOT : public AUnaryOperator
	{
	public:

		OperatorNOT() : AUnaryOperator()	{}
		virtual ~OperatorNOT() override		{}

		// impl�mentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override				{ return "!"; }
		inline virtual	ILogicElement*	Duplicate() const override				{ return new OperatorNOT(); }
		inline virtual	bool			operator == (const ILogicElement& oElem) const override		{ return dynamic_cast<const OperatorNOT*>(&oElem) != nullptr; }
		// impl�mentation interface AUnaryOperator
		inline virtual	bool			Evaluate(bool b) const override			{ return !b; }
	};

	// Et
	// -------------
	//	A	B	A+B
	// -------------
	//	T	T	 T
	//	T	F	 F
	//	F	T	 F
	//	F	F	 F
	class OperatorAND : public ABinaryOperator
	{
	public:

		OperatorAND() : ABinaryOperator()	{}
		virtual ~OperatorAND() override		{}

		// impl�mentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "+"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorAND(); }
		inline virtual	bool			operator == (const ILogicElement& oElem) const override		{ return dynamic_cast<const OperatorAND*>(&oElem) != nullptr; }
		// impl�mentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 && b2; }
	};

	// Ou
	// -------------
	//	A	B	A|B
	// -------------
	//	T	T	 T
	//	T	F	 T
	//	F	T	 T
	//	F	F	 F
	class OperatorOR : public ABinaryOperator
	{
	public:

		OperatorOR() : ABinaryOperator()	{}
		virtual ~OperatorOR() override		{}

		// impl�mentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "|"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorOR(); }
		inline virtual	bool			operator == (const ILogicElement& oElem) const override		{ return dynamic_cast<const OperatorOR*>(&oElem) != nullptr; }
		// impl�mentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 || b2; }
	};

	// Ou exclusif
	// -------------
	//	A	B	A^B
	// -------------
	//	T	T	 F
	//	T	F	 T
	//	F	T	 T
	//	F	F	 F
	class OperatorXOR : public ABinaryOperator
	{
	public:

		OperatorXOR() : ABinaryOperator()	{}
		virtual ~OperatorXOR() override		{}

		// impl�mentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "^"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorXOR(); }
		inline virtual	bool			operator == (const ILogicElement& oElem) const override		{ return dynamic_cast<const OperatorXOR*>(&oElem) != nullptr; }
		// impl�mentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 ^ b2; }
	};

	// Implique
	// --------------
	//	A	B	A=>B
	// --------------
	//	T	T	 T
	//	T	F	 F
	//	F	T	 T
	//	F	F	 T
	class OperatorIMP : public ABinaryOperator
	{
	public:

		OperatorIMP() : ABinaryOperator()	{}
		virtual ~OperatorIMP() override		{}

		// impl�mentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "=>"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorIMP(); }
		inline virtual	bool			operator == (const ILogicElement& oElem) const override		{ return dynamic_cast<const OperatorIMP*>(&oElem) != nullptr; }
		// impl�mentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return (!b1) || b2; }
	};

	// Equivalence (bidirectionnal implication)
	// ---------------
	//	A	B	A<=>B
	// ---------------
	//	T	T	  T
	//	T	F	  F
	//	F	T	  F
	//	F	F	  T
	class OperatorBIMP : public ABinaryOperator
	{
	public:

		OperatorBIMP() : ABinaryOperator()	{}
		virtual ~OperatorBIMP() override	{}

		// impl�mentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "<=>"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorBIMP(); }
		inline virtual	bool			operator == (const ILogicElement& oElem) const override		{ return dynamic_cast<const OperatorBIMP*>(&oElem) != nullptr; }
		// impl�mentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 == b2; }
	};
}
