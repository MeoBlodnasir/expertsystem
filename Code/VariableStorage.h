#pragma once

#include "ErrorCode.h"
#include "Variable.h"
#include <string>
#include <vector>


namespace ft
{
	class VariableStorage
	{
		public:

			VariableStorage();
			~VariableStorage();

			Variable	*GetVar(const char Id);
			void		SetVar(const char Id, bool value);


		private:
			std::vector<Variable*> m_oVars;

	};
}
