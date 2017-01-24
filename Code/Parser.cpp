
#include "Parser.h"

#include "Token.h"

namespace ft
{

	Parser::Parser()
		: m_iStateFlags(E_NONE)
		, m_iParenthesisLevel(0)
	{
		Reset();
	}

	Parser::~Parser()
	{
	}

	void	Parser::Reset()
	{
		m_iStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_ENTRY | E_WAITFOR_VARIABLE;
		m_iParenthesisLevel = 0;
	}

	bool	Parser::CheckToken(const Token& oToken)
	{
		// Pseudo State Machine pour vérifier si le Token envoyé est un Token attendu
		switch (oToken.GetType())
		{
		case Token::E_EOF:
			{
				if (!(m_iStateFlags & E_WAITFOR_EOL_EOF) || m_iParenthesisLevel > 0)
					return false;
				m_iStateFlags = E_NONE;
				break;
			}

		case Token::E_EOL:
			{
				if (!(m_iStateFlags & E_WAITFOR_EOL_EOF) || m_iParenthesisLevel > 0)
					return false;
				m_iStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_ENTRY | E_WAITFOR_VARIABLE;
				break;
			}

		case Token::E_SYNTAX_SYMBOL:
			{
				switch ((Token::ESubTypeSymbol)oToken.GetSubType())
				{
				case Token::E_SYM_OPEN_PAR:
					{
						if (m_iStateFlags & E_DONTWAIT_OPEN_PAR)
							return false;
						++m_iParenthesisLevel;
						break;
					}

				case Token::E_SYM_CLOSE_PAR:
					{
						if ((m_iStateFlags & E_DONTWAIT_CLOSE_PAR) || m_iParenthesisLevel == 0)
							return false;
						--m_iParenthesisLevel;
						break;
					}

				case Token::E_SYM_START_FACTS:
				case Token::E_SYM_START_QUERIES:
					{
						if (!(m_iStateFlags & E_WAITFOR_ENTRY))
							return false;
						m_iStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_VARIABLE | E_WAITONLY_VARIABLE | E_DONTWAIT_OPEN_PAR;
						break;
					}

				default:
					return false;
				}
				break;
			}

		case Token::E_LOGIC_OPERATOR:
			{
				switch ((Token::ESubTypeOperator)oToken.GetSubType())
				{
				case Token::E_OP_IMPLIES:
				case Token::E_OP_IMPLIES_IFANDONLYIF:
					{
						if (!(m_iStateFlags & E_WAITFOR_IMPLY) || m_iParenthesisLevel > 0)
							return false;
						m_iStateFlags = E_WAITFOR_VARIABLE | E_DONTWAIT_CLOSE_PAR;
						break;
					}

				case Token::E_OP_LOGIC_NOT:
					{
						if (!(m_iStateFlags & E_WAITFOR_VARIABLE) || (m_iStateFlags & E_WAITONLY_VARIABLE))
							return false;
						m_iStateFlags = E_WAITFOR_VARIABLE | E_DONTWAIT_CLOSE_PAR;
						break;
					}

				case Token::E_OP_LOGIC_AND:
				case Token::E_OP_LOGIC_OR:
				case Token::E_OP_LOGIC_XOR:
					{
						if (!(m_iStateFlags & E_WAITFOR_OPERATOR))
							return false;
						m_iStateFlags = E_WAITFOR_VARIABLE | E_DONTWAIT_CLOSE_PAR;
						break;
					}

				default:
					return false;
				}
				break;
			}

		case Token::E_VARIABLE:
			{
				if (!(m_iStateFlags & E_WAITFOR_VARIABLE))
					return false;
				m_iStateFlags = E_WAITFOR_EOL_EOF | E_WAITFOR_OPERATOR | E_WAITFOR_IMPLY;
				break;
			}

		default:
			return false;
		}

		return true;
	}
}
