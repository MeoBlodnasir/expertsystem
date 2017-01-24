#pragma once

#include "Types.h"

namespace ft
{
	//fw
	class Token;

	class Parser
	{
	public:

		enum EStateFlag
		{
			E_NONE					= 0,
			E_WAITFOR_ENTRY			= (1 << 0),
			E_WAITFOR_SYMBOL		= (1 << 1),
			E_WAITFOR_VARIABLE		= (1 << 2),
			E_WAITFOR_OPERATOR		= (1 << 3),
			E_WAITFOR_IMPLY			= (1 << 4),
			E_WAITFOR_EOL_EOF		= (1 << 5),
			E_DONTWAIT_OPEN_PAR		= (1 << 6),
			E_DONTWAIT_CLOSE_PAR	= (1 << 7),
			E_WAITONLY_VARIABLE		= (1 << 8)
		};

		Parser();
		~Parser();

		void	Reset();
		bool	CheckToken(const Token& oToken);

	private:

		int32	m_iStateFlags;
		int32	m_iParenthesisLevel;
	};
}
