
#include "StringUtils.h"

namespace ft
{
	std::string&	TrimWhiteSpaces(std::string& s)
	{
		if (s.size() > 0)
		{
			std::string::iterator itBegin = s.begin();
			std::string::iterator itEnd = s.end();
			std::string::iterator it = s.begin();
			while (IsWhiteSpace(*it) && it != itEnd)
				++it;
			if (it != itBegin)
				s.erase(itBegin, it);
			if (s.size() > 0)
			{
				itBegin = s.begin();
				itEnd = s.end();
				it = itEnd - 1;
				while (IsWhiteSpace(*it) && it != itBegin)
					--it;
				if (it != itEnd - 1)
					s.erase(it + 1, itEnd);
			}
		}
		
		return s;
	}
}
