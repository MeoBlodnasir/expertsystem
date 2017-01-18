
#include "Rule.h"

#include "Core.h"
#include "Output.h" //tmp

namespace ft
{
	Rule::Rule()
		: m_oConditionStack()
	{
	}

	Rule::~Rule()
	{
	}

	void		Rule::AddConditionElement(const IRuleElement* pElement)
	{
		FT_ASSERT(pElement != nullptr);

		m_oConditionStack.AddElement(pElement);
	}

	void		Rule::AddResultElement(const IRuleElement* pElement)
	{
		FT_ASSERT(pElement != nullptr);

		FT_TODO("AddResultElement");
	}

	EErrorCode	Rule::Evaluate() const
	{
		bool	bConditions = false;

		bConditions = m_oConditionStack.Evaluate();

		FT_COUT << (bConditions ? "true" : "false") << std::endl; // temporaire, pour tester

		return FT_OK;
	}

}
