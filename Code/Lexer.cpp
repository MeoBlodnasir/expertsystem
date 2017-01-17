
#include "Lexer.h"

#include "Core.h"
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
			FT_ASSERT(pToken != nullptr);
			FT_ASSERT(pOffset != nullptr);
			FT_ASSERT(csInput != nullptr);

			const char*	c = NULL;
			bool		bIsKeyWord = false;
			uint32		iOffset = 0;

			*pOffset = 0;

			// Vérifie les mots clés
			for (uint32 i = 0; i < sizeof(s_sKeyWords) / sizeof(std::string); ++i)
			{
				const std::string& sKeyWord = s_sKeyWords[i];
				c = csInput;
				bIsKeyWord = true;
				for (uint32 j = 0; bIsKeyWord & (j < sKeyWord.size()); ++j)
					bIsKeyWord &= c[j] == sKeyWord[j];
				if (bIsKeyWord)
				{
					pToken->SetupToken((Token::EType)i, sKeyWord);
					iOffset = sKeyWord.size();
					break;
				}
			}

			// Si l'entrée n'est pas un mot clé, crée une variable (uniquement alphanumérique)
			if (!bIsKeyWord)
			{
				while (IsLetter(c[iOffset]) || IsDigit(c[iOffset]))
					++iOffset;
				if (iOffset > 0)
					pToken->SetupToken(Token::E_VARIABLE, std::string(c, iOffset));
			}

			*pOffset = iOffset;

			// Si l'offset n'a pas bougé, c'est qu'il y a un problème (csInput pointant sur un caractère inutilisé)
			return iOffset == 0 ? FT_FAIL : FT_OK;
		}

		EErrorCode	ReadInput(std::vector<Token>* pTokens, const char* csInput)
		{
			FT_ASSERT(pTokens != nullptr);
			FT_ASSERT(csInput != nullptr);

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
