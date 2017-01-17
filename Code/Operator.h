#pragma once

#include "Types.h"
#include "IRuleElement.h"

namespace ft
{
	//////////////////////////////
	// Classes abstraites		//
	//////////////////////////////

	class AOperator : public IRuleElement
	{
	public:

		const uint32	iOperandCount;

		AOperator() : iOperandCount(0)	{}
		virtual ~AOperator() = 0;

		virtual EType	GetType() const						{ return E_OPERATOR; }
	};

	// Template opérateur
	template <uint32 N>
	class OperatorTemplate : public AOperator
	{
	public:

		OperatorTemplate() : iOperandCount(N)	{}
		virtual ~OperatorTemplate() = 0;
	};

	// Opérateur unaire
	class AUnaryOperator : public OperatorTemplate<1>
	{
	public:

		AUnaryOperator();
		virtual ~AUnaryOperator() = 0;

		virtual bool	Evaluate(bool b) const = 0;
	};

	// Opérateur binaire
	class ABinaryOperator : public OperatorTemplate<2>
	{
	public:

		ABinaryOperator();
		virtual ~ABinaryOperator() = 0;

		virtual bool	Evaluate(bool b1, bool b2) const = 0;
	};

	//////////////////////////////
	// Classes instanciables	//
	//////////////////////////////

	class OperatorNOT : public AUnaryOperator
	{
		OperatorNOT()			{}
		virtual ~OperatorNOT()	{}

		virtual bool	Evaluate(bool b) const				{ return !b; }
	};

	class OperatorAND : public ABinaryOperator
	{
		OperatorAND()			{}
		virtual ~OperatorAND()	{}

		virtual bool	Evaluate(bool b1, bool b2) const	{ return b1 && b2; }
	};

	class OperatorOR : public ABinaryOperator
	{
		OperatorOR()			{}
		virtual ~OperatorOR()	{}

		virtual bool	Evaluate(bool b1, bool b2) const	{ return b1 || b2; }
	};

	class OperatorXOR : public ABinaryOperator
	{
		OperatorXOR()			{}
		virtual ~OperatorXOR()	{}

		virtual bool	Evaluate(bool b1, bool b2) const	{ return b1 ^ b2; }
	};
}
