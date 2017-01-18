
#include "Output.h"
#include "FileReader.h"
#include "VariableStorage.h"
#include "Rule.h"
#include "Variable.h"
#include "Operator.h"

#include <vector>

int main()
{
	ft::FileReader	oFileReader;
	ft::VariableStorage oVariableStorage;
	ft::Rule		oRule;
// ok to put oRule in Filereader? ok to put VarStorage too maybe?	
	FT_COUT << (oFileReader.Read("./test.txt", oRule) == ft::FT_OK ? "FT_OK" : "FT_FAIL") << std::endl;


	ft::Variable	oT(true, 'T');
	ft::Variable	oF(false, 'F');

	ft::OperatorNOT	oNot;
	ft::OperatorAND	oAnd;
	ft::OperatorOR	oOr;
	ft::OperatorXOR	oXor;

	oRule.AddConditionElement(&oT);
	oRule.AddConditionElement(&oT);
	oRule.AddConditionElement(&oAnd);
	oRule.Evaluate();	// true

	oRule.AddConditionElement(&oF);
	oRule.AddConditionElement(&oAnd);
	oRule.Evaluate();	// false

	oRule.AddConditionElement(&oT);
	oRule.AddConditionElement(&oOr);
	oRule.Evaluate();	// true

	oRule.AddConditionElement(&oT);
	oRule.AddConditionElement(&oXor);
	oRule.Evaluate();	// false

	oRule.AddConditionElement(&oT);
	oRule.AddConditionElement(&oNot);
	oRule.AddConditionElement(&oXor);
	oRule.Evaluate();	// false

	oRule.AddConditionElement(&oF);
	oRule.AddConditionElement(&oNot);
	oRule.AddConditionElement(&oOr);
	oRule.Evaluate();	// true

	return 0;
}
