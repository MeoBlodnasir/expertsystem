#pragma once

#include "ErrorCode.h"
#include "VariableStorage.h"

namespace ft
{
	class Application
	{
	public:

		Application();
		~Application();

		EErrorCode	Init(int ac, char **av);
		EErrorCode	Destroy();

		EErrorCode	Run();

	private:

		VariableStorage		m_oVariableStorage;
		std::vector<Rule>	m_oRules;
		std::vector<char>	m_oPendingQueries;
	};
}
