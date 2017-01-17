
#include "Lexer.h"

#include "Types.h"
#include "StringUtils.h"

namespace ft
{
	namespace Lexer
	{
		const std::string s_sKeyWords[Token::E_OP_COUNT] =
		{
			"<=>",
			"=>",
			"+",
			"|",
			"^",
			"!",
			"(",
			")"
		};

		EErrorCode	ReadToken(Token* pToken, uint32* pOffset, const char* csInput)
		{
			const char*	c = NULL;
			bool		bIsKeyWord = false;
			uint32		iOffset = 0;

			*pOffset = 0;

			// V�rifie les mots cl�s
			for (uint32 i = 0; i < sizeof(s_sKeyWords) / sizeof(std::string); ++i)
			{
				const std::string& sKeyWord = s_sKeyWords[i];
				c = csInput;
				bIsKeyWord = true;
				for (uint32 j = 0; bIsKeyWord & (j < sKeyWord.size()); ++j)
					bIsKeyWord &= c[j] == sKeyWord[j];
				if (bIsKeyWord)
				{
					pToken->SetupToken((Token::ETypes)i, sKeyWord);
					iOffset = sKeyWord.size();
					break;
				}
			}

			// Si l'entr�e n'est pas un mot cl�, cr�e une variable (uniquement alphanum�rique)
			if (!bIsKeyWord)
			{
				while (IsLetter(c[iOffset]) || IsDigit(c[iOffset]))
					++iOffset;
				if (iOffset > 0)
					pToken->SetupToken(Token::E_VARIABLE, std::string(c, iOffset));
			}

			*pOffset = iOffset;

			// Si l'offset n'a pas boug�, c'est qu'il y a un probl�me (csInput pointant sur un caract�re inutilis�)
			return iOffset == 0 ? FT_FAIL : FT_OK;
		}

		EErrorCode	ReadInput(std::vector<Token>* pTokens, const char* csInput)
		{
			const char*	c = csInput;
			Token		oToken;
			uint32		iOffset;

			while (*c != '\0')
			{
				while (IsWhiteSpace(*c))
					++c;

				if (ReadToken(&oToken, &iOffset, c) == FT_OK)
				{
					pTokens->push_back(oToken);
					c += iOffset;
				}
				else
				{
					++c;
				}
			}

			return FT_OK;
		}
	}
}
