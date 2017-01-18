
#include "VariableStorage.h"


namespace ft
{
	VariableStorage::VariableStorage()
	{
	}

	VariableStorage::~VariableStorage()
	{
	}

	const Variable&	VariableStorage::GetVar(const char Id)
	{
		for (std::vector<Variable>::const_iterator it = m_oVars.begin(), itEnd = m_oVars.end(); it != itEnd; it++)
		{
			if (it->GetId() == Id)
			{
				return (*it);
			}

		}

		m_oVars.push_back(Variable(false, Id));
		return m_oVars.back();
	}

	void	VariableStorage::SetVar(const char Id, bool value)
	{
		for (std::vector<Variable>::iterator it = m_oVars.begin(), itEnd = m_oVars.end(); it != itEnd; it++)
		{
			if (it->GetId() == Id)
			{
				it->SetState(value);
				it->SetId(Id);
				return ;
			}

		}

		// If var isnt found, create a new one, with correct attributes
		m_oVars.push_back(Variable(value, Id));
	}



}
