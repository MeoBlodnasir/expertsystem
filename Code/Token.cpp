
#include "Token.h"

namespace ft
{
	Token::Token()
		: m_eType(E_NONE)
		, m_eSubType(-1)
		, m_sDesc("")
	{
	}

	Token::Token(const Token& oToken)
		: m_eType(oToken.m_eType)
		, m_eSubType(oToken.m_eSubType)
		, m_sDesc(oToken.m_sDesc)
	{
	}

	Token::~Token()
	{
	}

	EErrorCode	Token::SetupToken(EType eTokenType, int32 eSubType /*= -1*/, const std::string& sDesc /*= std::string()*/)
	{
		m_eType = eTokenType;
		m_eSubType = eSubType;
		m_sDesc = sDesc;

		return FT_OK;
	}
}
