
#include "Lexer.h"

#include "Core.h"
#include "Types.h"
#include "StringUtils.h"

namespace ft
{
	namespace Lexer
	{
		static const std::string s_sSymbolsKeyWord[Token::E_SYM_COUNT] =
		{
			"(",
			")",
			"=",
			"?"
		};

		static const std::string s_sOperatorsKeyWord[Token::E_OP_COUNT] =
		{
			"<=>",
			"=>",
			"+",
			"|",
			"^",
			"!"
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
			
			// Opérateurs logiques
			for (uint32 i = 0; i < sizeof(s_sOperatorsKeyWord) / sizeof(std::string); ++i)
			{
				const std::string& sKeyWord = s_sOperatorsKeyWord[i];
				c = csInput;
				bIsKeyWord = true;
				for (uint32 j = 0; bIsKeyWord & (j < sKeyWord.size()); ++j)
					bIsKeyWord &= c[j] == sKeyWord[j];
				if (bIsKeyWord)
				{
					pToken->SetupToken(Token::E_LOGIC_OPERATOR, i, sKeyWord);
					iOffset = sKeyWord.size();
					break;
				}
			}

			// Symboles syntaxiques
			if (!bIsKeyWord)
			{
				for (uint32 i = 0; i < sizeof(s_sSymbolsKeyWord) / sizeof(std::string); ++i)
				{
					const std::string& sKeyWord = s_sSymbolsKeyWord[i];
					c = csInput;
					bIsKeyWord = true;
					for (uint32 j = 0; bIsKeyWord & (j < sKeyWord.size()); ++j)
						bIsKeyWord &= c[j] == sKeyWord[j];
					if (bIsKeyWord)
					{
						pToken->SetupToken(Token::E_SYNTAX_SYMBOL, i, sKeyWord);
						iOffset = sKeyWord.size();
						break;
					}
				}
			}

			// Si l'entrée n'est pas un mot clé, crée une variable (uniquement alphanumérique)
			if (!bIsKeyWord)
			{
				while (IsLetter(c[iOffset]) || IsDigit(c[iOffset]))
					++iOffset;
				if (iOffset > 0)
					pToken->SetupToken(Token::E_VARIABLE, -1, std::string(c, iOffset));
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

				if (*c == '\n')
				{
					oToken.SetupToken(Token::E_EOL);
					pTokens->push_back(oToken);
					++c;
				}

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

			oToken.SetupToken(Token::E_EOF);
			pTokens->push_back(oToken);

			return FT_OK;
		}
	}
}
