#pragma once

// Classe qui permet d'écrire dans la console de VisualStudio si on travaille dessus
// Utiliser les macros FT_COUT et FT_CERR en lieu et place de std::cout et std::cerr

#include <iostream>

#if defined (_MSC_VER)

#	include <windows.h>	// ::OutputDebugString()

namespace ft
{
	class VisualConsoleOutputBuf : public std::streambuf
	{
	protected:

		virtual int overflow(int c) override
		{
			static char sz[2] = {0};
			sz[0] = (char)c;
			::OutputDebugString(sz);
			return c;
		}

		virtual std::streamsize sputn(const char *s, std::streamsize n)
		{
			::OutputDebugString(s);
			return n;
		}
	};

	class Output : public std::ostream
	{
	public:

		VisualConsoleOutputBuf m_oBuffer;

		Output() : std::ostream(&m_oBuffer) {}
	};

	extern Output Out;
}

#	define FT_COUT	ft::Out
#	define FT_CERR	ft::Out

#else

#	define FT_COUT	std::cout
#	define FT_CERR	std::cerr

#endif