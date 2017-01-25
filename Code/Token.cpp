
#include "Token.h"

namespace ft
{
	Token::Token()
		: m_eType(E_NONE)
		, m_sDesc("")
	{
	}

	Token::Token(const Token& oToken)
		: m_eType(oToken.m_eType)
		, m_sDesc(oToken.m_sDesc)
	{
	}

	Token::~Token()
	{
	}

	EErrorCode	Token::SetupToken(EType eTokenType, const std::string& sDesc /*= std::string()*/)
	{
		m_eType = eTokenType;
		m_sDesc = sDesc;

		return FT_OK;
	}
}
