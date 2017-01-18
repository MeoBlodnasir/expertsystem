#pragma once

#include "Variable.h"
#include <vector>


namespace ft
{
	class VariableStorage
	{
	public:

		VariableStorage();
		~VariableStorage();

		const Variable&	GetVar(const char Id);
		void			SetVar(const char Id, bool value);

	private:

		std::vector<Variable>	m_oVars;
	};
}
