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

		// implémentation interface ILogicElement
		inline virtual EType			GetType() const override	{ return E_OPERATOR; }

		template <typename T>
		inline bool	operator == (const T&)							{ return false; }
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

		// implémentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override				{ return "!"; }
		inline virtual	ILogicElement*	Duplicate() const override				{ return new OperatorNOT(); }
		// implémentation interface AUnaryOperator
		inline virtual	bool				Evaluate(bool b) const override		{ return !b; }

		inline bool	operator == (const OperatorNOT&)							{ return true; }
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

		// implémentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "+"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorAND(); }
		// implémentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 && b2; }

		inline bool	operator == (const OperatorAND&)								{ return true; }
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

		// implémentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "|"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorOR(); }
		// implémentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 || b2; }

		inline bool	operator == (const OperatorOR&)									{ return true; }
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

		// implémentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "^"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorXOR(); }
		// implémentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 ^ b2; }

		inline bool	operator == (const OperatorXOR&)								{ return true; }
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

		// implémentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "=>"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorIMP(); }
		// implémentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return (!b1) || b2; }

		inline bool	operator == (const OperatorIMP&)								{ return true; }
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

		// implémentation interface ILogicElement
		inline virtual	std::string		GetDesc() const override					{ return "<=>"; }
		inline virtual	ILogicElement*	Duplicate() const override					{ return new OperatorBIMP(); }
		// implémentation interface ABinaryOperator
		inline virtual	bool			Evaluate(bool b1, bool b2) const override	{ return b1 == b2; }

		inline bool	operator == (const OperatorBIMP&)								{ return true; }
	};
}
