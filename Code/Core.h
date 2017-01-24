#pragma once

#if defined (_WIN32)
#	include <intrin.h>	// __debugbreak()
#endif

// Outils macro imbriquées
#define FT_STRINGIFY2(s)	#s
#define FT_STRINGIFY(s)		FT_STRINGIFY2(s)

// Spécifications C++
#if defined (_MSC_VER)
#	define FT_VIRTUAL_PURE_DESTRUCTOR	= 0 {}
#else
#	define FT_VIRTUAL_PURE_DESTRUCTOR	= 0;
#endif

// Break au runtime
#if defined (_WIN32)
#	define FT_BREAK_CPU()		__debugbreak()
#else
#	define FT_BREAK_CPU()		asm volatile ("int $3")
#endif

// Todo
// Trouver un moyen de le faire avec g++/clang++
#if defined (_WIN32)
#	define FT_TODO(s)			__pragma(message(__FILE__ "(" FT_STRINGIFY(__LINE__) ") : A FAIRE: " s))
#else
#	define FT_TODO(s)
#endif

namespace ft
{
	namespace Core
	{
		void	PrintAssertInfos(const char* csExpression, const char* csFilePath, const int iLine, const char* csFunction);
	}
}

// Asserts
// FT_ASSERT à l'exécution
// FT_STATIC_ASSERT à la compilation
// en DEBUG uniquement -> à utiliser sans modération !
// ATTENTION -> ne tester dans un FT_ASSERT que des variables ou des fonctions const (qui n'ont pas d'incidence sur le code ou la donnée) vu que ce sera effacé en Release
#if defined(__FT_DEBUG__)

#	define FT_FAILED_ASSERTION(expr)	\
	{									\
		ft::Core::PrintAssertInfos(#expr, __FILE__, __LINE__, __FUNCTION__);	\
		FT_BREAK_CPU();					\
	}

#	define FT_ASSERT(expr)				\
	{									\
		if (!(expr))					\
		{								\
			FT_FAILED_ASSERTION(expr)	\
		}								\
	}

#	define FT_STATIC_ASSERT(expr)		typedef char _FT_S_ASSERT[(expr) ? 1 : -1]

#else
#	define FT_FAILED_ASSERTION(expr)
#	define FT_ASSERT(expr)
#	define FT_STATIC_ASSERT(expr)
#endif

// Macros pratiques autour de FT_ASSERT
#define FT_TEST(expr)				\
{									\
	if (!(expr))					\
	{								\
		FT_FAILED_ASSERTION(expr)	\
	}								\
}

#define FT_TEST_RETURN(expr, ret)	\
{									\
	if (!(expr))					\
	{								\
		FT_FAILED_ASSERTION(expr)	\
		return (ret);				\
	}								\
}

#define FT_TEST_OK(expr)			FT_TEST_RETURN((expr) == ft::FT_OK, ft::FT_FAIL)

#define FT_NOT_IMPLEMENTED(mess)	\
{									\
	FT_TODO(mess);					\
	FT_FAILED_ASSERTION(false)		\
}

// Macros utiles
#define FT_DELETE(ptr)				{ delete ptr; ptr = nullptr; }
#define FT_DELETE_ARRAY(ptr)		{ delete [] ptr; ptr = nullptr; }
#define FT_SAFE_DELETE(ptr)			{ if (ptr) FT_DELETE(ptr); }
#define FT_SAFE_DELETE_ARRAY(ptr)	{ if (ptr) FT_DELETE_ARRAY(ptr); }
