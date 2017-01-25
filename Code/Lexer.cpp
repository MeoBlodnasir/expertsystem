
#include "Lexer.h"

#include "Core.h"
#include "Types.h"
#include "StringUtils.h"

namespace ft
{
	namespace Lexer
	{
		static const std::string s_sCommentEntry = "#";

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

		static bool	IsKeyWord(const char* csInput, const std::string& sKeyWord)
		{
			bool bIsKeyWord = true;
			for (uint32 i = 0; bIsKeyWord & (i < sKeyWord.size()); ++i)
				bIsKeyWord &= csInput[i] == sKeyWord[i];
			return bIsKeyWord;
		}

		EErrorCode	ReadToken(Token* pToken, uint32* pOffset, const char* csInput)
		{
			FT_ASSERT(pToken != nullptr);
			FT_ASSERT(pOffset != nullptr);
			FT_ASSERT(csInput != nullptr);

			const char*	c = csInput;

			*pOffset = 0;

			// Vérifie les mots clés

			// Commentaire
			if (IsKeyWord(c, s_sCommentEntry))
			{
				while (*c != '\n' && *c != '\0')
					++c;
				pToken->SetupToken(Token::E_COMMENT, -1, std::string(csInput, c - csInput));
				goto KeywordFound;
			}
			
			// Opérateurs logiques
			for (uint32 i = 0; i < sizeof(s_sOperatorsKeyWord) / sizeof(std::string); ++i)
			{
				const std::string& sKeyWord = s_sOperatorsKeyWord[i];
				if (IsKeyWord(c, sKeyWord))
				{
					pToken->SetupToken(Token::E_LOGIC_OPERATOR, i, sKeyWord);
					c += sKeyWord.size();
					goto KeywordFound;
				}
			}

			// Symboles syntaxiques
			for (uint32 i = 0; i < sizeof(s_sSymbolsKeyWord) / sizeof(std::string); ++i)
			{
				const std::string& sKeyWord = s_sSymbolsKeyWord[i];
				if (IsKeyWord(c, sKeyWord))
				{
					pToken->SetupToken(Token::E_SYNTAX_SYMBOL, i, sKeyWord);
					c += sKeyWord.size();
					goto KeywordFound;
				}
			}

			// Si l'entrée n'est pas un mot clé, créer une variable (uniquement alphanumérique)
			while (IsLetter(*c) || IsDigit(*c))
				++c;
			if (c != csInput)
				pToken->SetupToken(Token::E_VARIABLE, -1, std::string(csInput, c - csInput));

KeywordFound:

			*pOffset = c - csInput;

			return c == csInput ? FT_FAIL : FT_OK;
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
