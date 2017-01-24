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
			E_EOF,			// EndOfFile
			E_EOL,			// EndOfLine
			E_SYNTAX_SYMBOL,
			E_LOGIC_OPERATOR,
			E_VARIABLE,
		};

		enum ESubTypeSymbol
		{
			// L'ordre est lié à celui de la définition dans Lexer.cpp
			// => pas une bonne idée, voir pour faire mieux
			E_SYM_OPEN_PAR = 0,
			E_SYM_CLOSE_PAR,
			E_SYM_START_FACTS,
			E_SYM_START_QUERIES,

			E_SYM_COUNT
		};

		enum ESubTypeOperator
		{
			// L'ordre est lié à celui de la définition dans Lexer.cpp
			// => pas une bonne idée, voir pour faire mieux
			E_OP_IMPLIES_IFANDONLYIF = 0,
			E_OP_IMPLIES,
			E_OP_LOGIC_AND,
			E_OP_LOGIC_OR,
			E_OP_LOGIC_XOR,
			E_OP_LOGIC_NOT,

			E_OP_COUNT
		};

		Token();
		Token(const Token& oToken);
		~Token();

		inline EType		GetType() const						{ return m_eType; }
		inline int			GetSubType() const					{ return m_eSubType; }
		inline std::string	GetDesc() const						{ return m_sDesc; }

		EErrorCode	SetupToken(EType eTokenType, int32 eSubType = -1, const std::string& sDesc = std::string());

	private:

		EType		m_eType;
		int32		m_eSubType; // par ex: ESubTypeOperator
		std::string	m_sDesc;
	};
}
