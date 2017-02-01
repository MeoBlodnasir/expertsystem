#pragma once

#include "Types.h"
#include "ErrorCode.h"

#include <string>

namespace ft
{
	class Token
	{
	public:

		enum EType
		{
			E_NONE = -1,	// Invalide
			E_EOF = 0,		// EndOfFile
			E_EOL,			// EndOfLine

			// Commentaires
			E_COMMENT,

			// Commandes
			E_CMD_OFFSET,
			E_CMD_VERBOSE,
			E_CMD_PRINT,
			E_CMD_FLUSH,
			E_CMD_QUIT,
			E_CMD_COUNT,

			// Composants de règles
			// L'ordre est lié à celui de la définition dans Lexer.cpp
			// => pas une bonne idée, voir pour faire mieux
			E_SYM_OFFSET,
			E_SYM_OPEN_PAR,
			E_SYM_CLOSE_PAR,
			E_SYM_START_FACTS,
			E_SYM_START_QUERIES,
			E_SYM_COUNT,

			E_OP_OFFSET,
			E_OP_IMPLIES_IFANDONLYIF,
			E_OP_IMPLIES,
			E_OP_LOGIC_NOT,
			E_OP_LOGIC_AND,
			E_OP_LOGIC_OR,
			E_OP_LOGIC_XOR,
			E_OP_COUNT,

			// Variables
			E_VARIABLE,
		};

		Token();
		Token(const Token& oToken);
		~Token();

		inline EType		GetType() const					{ return m_eType; }
		inline std::string	GetDesc() const					{ return m_sDesc; }

		EErrorCode	SetupToken(EType eTokenType, const std::string& sDesc = std::string());

	private:

		EType		m_eType;
		std::string	m_sDesc;
	};
}
