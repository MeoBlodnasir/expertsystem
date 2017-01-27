#pragma once

#include "Atom.h"

namespace ft
{
	// fw
	class VariablesManager;

	class Variable
	{
	public:

		typedef	Atom::Id	Id;

		// Constructeurs publics pour les besoins de la map dans le VariablesManager
		// mais une variable n'est pas sensée être créée par aucun objet autre que le VariablesManager
		Variable();
		Variable(Id iId, bool bState = false);
		Variable(const Variable& oVar);
		~Variable();

		inline Id	GetId() const					{ return m_iId; }
		inline bool	GetState() const				{ return m_bState; }

	private:
		friend class VariablesManager;

		Id		m_iId;
		bool	m_bState;
	};
}
