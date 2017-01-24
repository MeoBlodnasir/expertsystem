
#include "Rule.h"

#include "Core.h"
#include "Output.h" // tmp

namespace ft
{
	Rule::Rule()
		: m_oCondition()
	{
	}

	Rule::~Rule()
	{
	}

	void		Rule::AddConditionElement(const IRuleElement* pElement)
	{
		FT_ASSERT(pElement != nullptr);

		m_oCondition.AddElement(pElement);
	}

	void		Rule::AddResultElement(const IRuleElement* pElement)
	{
		FT_ASSERT(pElement != nullptr);

		m_oResult.AddElement(pElement);
	}

	EErrorCode	Rule::Evaluate() const
	{
		bool	bConditions = false;

		bConditions = m_oCondition.Evaluate();

		FT_COUT << (bConditions ? "true" : "false") << std::endl; // temporaire, pour tester

		return FT_OK;
	}

}
