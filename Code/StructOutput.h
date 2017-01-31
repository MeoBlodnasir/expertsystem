#pragma once

#include "Output.h"

namespace ft
{
	// fw
	class Rule;

	std::ostream&	operator << (std::ostream& oOs, const Rule& oRule);
}
