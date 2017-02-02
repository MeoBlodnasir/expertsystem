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
			E_CMD_NONE = 0,
			E_CMD_VERBOSE,
			E_CMD_PRINT,
			E_CMD_RESET_VARIABLES,
			E_CMD_FLUSH,
			E_CMD_HELP,
			E_CMD_QUIT
		};

		enum EOption
		{
			E_OPT_NONE		= 0,
			E_OPT_VERBOSE	= (1 << 0)
		};

		bool					m_bQuitApplication;
		ECommand				m_ePendingCommand;
		uint32					m_iOptions;
		SPtr<VariablesManager>	m_xVariablesManager;
		SPtr<RulesManager>		m_xRulesManager;
		SPtr<InferenceEngine>	m_xInferenceEngine;
		AtomIdSet				m_oPendingQueries;

		EErrorCode	ReadInputFiles(const int32 ac, const char* const* av);
		EErrorCode	AskUserInput();
		EErrorCode	ProcessInputLine(const std::string& sLine);
		void		ProcessCommand();
		void		Flush();

		EErrorCode	EvaluatePendingQueries();

		void		PrintCurrentState() const;
	};
}
