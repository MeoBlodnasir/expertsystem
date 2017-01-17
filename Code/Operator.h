#pragma once

#include "Core.h"
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

		AOperator() : iOperandCount(0)			{}
		AOperator(uint32 n) : iOperandCount(n)	{}
		virtual ~AOperator()					{}

		AOperator&	operator = (const AOperator&)			{ return *this; }

		// impl�mentation interface IRuleElement
		virtual EType	GetType() const						{ return E_OPERATOR; }
	};

	// Template op�rateur
	template <uint32 N>
	class OperatorTemplate : public AOperator
	{
	public:

		OperatorTemplate() : AOperator(N)	{}
		virtual ~OperatorTemplate()			{}
	};

	// Op�rateur unaire
	class AUnaryOperator : public OperatorTemplate<1>
	{
	public:

		AUnaryOperator()				{}
		virtual ~AUnaryOperator()		{}

		virtual bool	Evaluate(bool b) const = 0;
	};

	// Op�rateur binaire
	class ABinaryOperator : public OperatorTemplate<2>
	{
	public:

		ABinaryOperator()				{}
		virtual ~ABinaryOperator()		{}

		virtual bool	Evaluate(bool b1, bool b2) const = 0;
	};

	//////////////////////////////
	// Classes instanciables	//
	//////////////////////////////

	class OperatorNOT : public AUnaryOperator
	{
	public:

		OperatorNOT()			{}
		virtual ~OperatorNOT()	{}

		virtual bool	Evaluate(bool b) const				{ return !b; }
	};

	class OperatorAND : public ABinaryOperator
	{
	public:

		OperatorAND()			{}
		virtual ~OperatorAND()	{}

		virtual bool	Evaluate(bool b1, bool b2) const	{ return b1 && b2; }
	};

	class OperatorOR : public ABinaryOperator
	{
	public:

		OperatorOR()			{}
		virtual ~OperatorOR()	{}

		virtual bool	Evaluate(bool b1, bool b2) const	{ return b1 || b2; }
	};

	class OperatorXOR : public ABinaryOperator
	{
	public:

		OperatorXOR()			{}
		virtual ~OperatorXOR()	{}

		virtual bool	Evaluate(bool b1, bool b2) const	{ return b1 ^ b2; }
	};
}
