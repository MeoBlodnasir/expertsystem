#pragma once

namespace ft
{
	inline bool	IsWhiteSpace(char c)
	{
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}

	inline bool	IsDigit(char c)
	{
		return '0' <= c && c <= '9';
	}

	inline bool	IsLetter(char c)
	{
		return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
	}

	inline bool	IsNumberDescriptor(char c)
	{
		return IsDigit(c) || c == '.' || c == '+' || c == '-';
	}
}
