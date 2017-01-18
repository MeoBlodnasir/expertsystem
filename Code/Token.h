#pragma once

#include "ErrorCode.h"

#include <string>

namespace ft
{
	class Token
	{
	public:

		enum EType
		{
			// Type invalide
			E_NONE = -1,

			// Opérateurs
			// L'ordre est lié à celui de la définition des opérateurs dans Lexer.cpp => pas une bonne idée, voir pour faire mieux
			E_OP_IMPLIES_IFANDONLYIF = 0,
			E_OP_IMPLIES,
			E_OP_LOGIC_AND,
			E_OP_LOGIC_OR,
			E_OP_LOGIC_XOR,
			E_OP_LOGIC_NOT,
			E_OP_PAR_OPEN,
			E_OP_PAR_CLOSE,

			E_OP_COUNT,

			// Variable
			E_VARIABLE = 0x00010000
		};

		Token();
		Token(const Token& oToken);
		~Token();
		EType	GetType() const
		{ return m_eType; }
		std::string GetDesc() const
		{ return m_sDesc;}

		//Token& operator = (const Token& oToken);

		EErrorCode	SetupToken(EType eTokenType, const std::string& sDesc);

	private:

		EType		m_eType;
		std::string	m_sDesc;
	};
}
