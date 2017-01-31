
#include "Lexer.h"

#include "Core.h"
#include "Types.h"
#include "StringUtils.h"
#include "Output.h"

namespace ft
{
	namespace Lexer
	{
		static const std::string s_sCommentEntry = "#";

		static const std::string s_sCommandsKeyWord[Token::E_CMD_COUNT - (Token::E_CMD_OFFSET+1)] =
		{
			"quit"
		};

		static const std::string s_sSymbolsKeyWord[Token::E_SYM_COUNT - (Token::E_SYM_OFFSET+1)] =
		{
			"(",
			")",
			"=",
			"?"
		};

		static const std::string s_sOperatorsKeyWord[Token::E_OP_COUNT - (Token::E_OP_OFFSET+1)] =
		{
			"<=>",
			"=>",
			"!",
			"+",
			"|",
			"^"
		};

		static bool	ReadCommand(Token* pToken, const std::string& sLine)
		{
			FT_ASSERT(pToken != nullptr);

			for (uint32 i = 0, iCount = sizeof(s_sCommandsKeyWord) / sizeof(std::string); i < iCount; ++i)
			{
				if (sLine == s_sCommandsKeyWord[i])
				{
					pToken->SetupToken((Token::EType)(Token::E_CMD_OFFSET+1 + i), sLine);
					return true;
				}
			}
			return false;
		}

		static bool	IsKeyWord(const char* csInput, const std::string& sKeyWord)
		{
			bool bIsKeyWord = true;

			for (uint32 i = 0; bIsKeyWord && (i < sKeyWord.size()); ++i)
				bIsKeyWord &= csInput[i] == sKeyWord[i];

			return bIsKeyWord;
		}

		static EErrorCode	ReadElement(Token* pToken, uint32* pOffset, const char* csInput)
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
				pToken->SetupToken(Token::E_COMMENT, std::string(csInput, c - csInput));
				goto KeywordFound;
			}
			
			// Opérateurs logiques
			for (uint32 i = 0, iCount = sizeof(s_sOperatorsKeyWord) / sizeof(std::string); i < iCount; ++i)
			{
				const std::string& sKeyWord = s_sOperatorsKeyWord[i];
				if (IsKeyWord(c, sKeyWord))
				{
					pToken->SetupToken((Token::EType)(Token::E_OP_OFFSET+1 + i), sKeyWord);
					c += sKeyWord.size();
					goto KeywordFound;
				}
			}

			// Symboles syntaxiques
			for (uint32 i = 0, iCount = sizeof(s_sSymbolsKeyWord) / sizeof(std::string); i < iCount; ++i)
			{
				const std::string& sKeyWord = s_sSymbolsKeyWord[i];
				if (IsKeyWord(c, sKeyWord))
				{
					pToken->SetupToken((Token::EType)(Token::E_SYM_OFFSET+1 + i), sKeyWord);
					c += sKeyWord.size();
					goto KeywordFound;
				}
			}

			// Si l'entrée n'est pas un mot clé, créer une variable (uniquement alphabétique à un caractère)
			if (IsLetter(*c))
				++c;
			if (c != csInput)
				pToken->SetupToken(Token::E_VARIABLE, std::string(csInput, c - csInput));

KeywordFound:

			*pOffset = c - csInput;

			return c == csInput ? FT_FAIL : FT_OK;
		}

		EErrorCode		ReadLine(OutData* pLexingData, const std::string& sLine)
		{
			FT_ASSERT(pLexingData != nullptr);

			OutData&	oOutData = *pLexingData;
			const char*	c = sLine.c_str();
			Token		oToken;
			uint32		iOffset;
			EErrorCode	eRet = FT_OK;

			if (ReadCommand(&oToken, sLine))
				oOutData.oTokens.push_back(oToken);
			else
			{
				while (*c != '\0')
				{
					while (IsWhiteSpace(*c))
						++c;

					if (ReadElement(&oToken, &iOffset, c) == FT_OK)
					{
						oOutData.oTokens.push_back(oToken);
						c += iOffset;
					}
					else
					{
						iOffset = 0;
						while (!IsWhiteSpace(c[iOffset]) && c[iOffset] != '\0')
							++iOffset;
						FT_CERR << "Erreur ligne: " << sLine << std::endl;
						FT_CERR << "Entrée inconnue ignorée: " << std::string(c, iOffset) << std::endl;
						eRet = FT_FAIL;
						c += iOffset;
					}
				}
			}

			oToken.SetupToken(Token::E_EOL);
			oOutData.oTokens.push_back(oToken);

			return eRet;
		}
	}
}
