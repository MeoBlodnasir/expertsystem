#pragma once

#include "Types.h"
#include "ErrorCode.h"
#include "StrongPointer.h"
#include "Rule.h"

#include <vector>

namespace ft
{
	// fw
	class VariablesManager;
	class RulesManager;
	class InferenceEngine;

	class Application
	{
	public:

		Application();
		~Application();

		EErrorCode	Init(const int32 ac, const char* const* av);
		EErrorCode	Destroy();

		EErrorCode	Run();

	private:

		enum ECommand
		{
			E_NONE = 0,
			E_QUIT
		};

		ECommand				m_ePendingCommand;
		SPtr<VariablesManager>	m_xVariablesManager;
		SPtr<RulesManager>		m_xRulesManager;
		SPtr<InferenceEngine>	m_xInferenceEngine;
		AtomIdSet				m_oPendingQueries;

		EErrorCode	ProcessInputLine(const std::string& sLine);
		EErrorCode	ReadInputFiles(const int32 ac, const char* const* av);
		EErrorCode	AskUserInput();

		EErrorCode	EvaluatePendingQueries();
	};
}
