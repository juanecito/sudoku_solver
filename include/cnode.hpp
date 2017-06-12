/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cnode.hpp
 * Copyright (C) 2008, 2013 Juan Maria Gomez Lopez <juanecitorr@gmail.com>
 *
 * sudoku_solver is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sudoku_solver is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file cnode.hpp
 * @author Juan Maria Gomez Lopez <juanecitorr@gmail.com>
 * @date 16 Nov 2008
 */

#ifndef _CNODE_HPP_
#define _CNODE_HPP_

#include <iostream>
#include <vector>

#include <memory>

////////////////////////////////////////////////////////////////////////////////
/**
 * @class CNode
 */
template <class InfoType> class CNode
{
	public:
		/**
		 * @brief Default constructor CNode
		 */
		CNode(void);

		/**
		 * @brief CNode constructor, with his parent, data info, and his index in the children
		 * @param parent
		 * @param info
		 * @param index_of_children
		 */
		CNode( CNode<InfoType> *parent, const InfoType& info , int index_of_children);
		
		/**
		 * Copy constructor
		 * @param original
		 */
		CNode(const CNode<InfoType> &original)
		{
			*this = original;
		}

		/**
		 * @brief CNode copy operator
		 * @param original
		 * @return
		 */
		CNode<InfoType> &operator=(const CNode<InfoType> &original);

		
		virtual ~CNode( ){};


		/* Metodo para devolver el get_children_by_index izquierdo de un nodo */
		/* Devuelve el hijo izquierdo del nodo */
		CNode<InfoType> *get_children_by_index( int children_index )
		{
			if( children_index > (m_childrens.size() + 1) )
			{
				return nullptr;
			}
			return m_childrens.at(children_index);
		}

		/**
		 * @return children count of node
		 */
		int get_children_count() const{ return m_childrens.size(); }
		
		/**
		 * @brief Check if this node is leaf (terminal node)
		 * @return true: is a terminal node
		 */
		inline bool isLeaf(void) const
		{
		  if ( this->m_childrens.empty() )
		    return true;
		  else
		    return false;
		}

		/**
		 * @brief Check if this node is root (initial node)
		 * @return true: is root node
		 */
		inline bool isRoot(void) const
		{
		  if ( !_parent )
		    return true;
		  else
		    return false;
		}

		/**
		 * @return node parent
		 */
		inline const CNode<InfoType> *get_parent( void ) const
		{
		  return _parent;
		}

		/**
		 * @brief Set information data in CNode object
		 * @param x
		 */
		/* Metodo para asignar el campo informativo a un nodo */
		/* Recibe como parametro el campo informativo a asignar */
		inline void set_information(const InfoType& x )
		{
			_information = x;
		}

		/**
		 * @brief Get information data in CNode object
		 * @return
		 */
		/* Metodo para devolver el campo informativo de un nodo */
		/* Devuelve el campo informativo del nodo */
		inline const InfoType& get_informacion() const
		{
		  return _information;
		}

		/**
		 * Start solution search
		 * @param visitados
		 * @return
		 */
		bool search(std::vector<InfoType>** visitados);

		/**
		 *
		 * @param visitados
		 * @return
		 */
		int generateChildrenInNode(std::vector<InfoType> **visitados);

	private:

		void set_parent( CNode<InfoType> *parent ) /* Funcion para asignar el padre de un nodo */
		{
			_parent = parent;
		}

		/**
		 * @brief Insert children in node
		 * @param h
		 */
		void insert_children( CNode<InfoType> *h )
		{
			this->m_childrens.push(h);
		}

		// TODO:
		bool _nodosIguales( bool (*compara)(InfoType,InfoType), const CNode<InfoType>, const CNode<InfoType>) const;

		/* Variables */
		InfoType _information;
		
		// indica si el arbol puede seguir creciendo por aqui
		bool m_is_expansible;

		bool m_is_solved;

		std::vector< std::shared_ptr<CNode<InfoType> > > m_childrens; /* Vector de punteros a hijos del nodo */

		CNode<InfoType> *_parent;

		int m_out_children_index;

};

////////////////////////////////////////////////////////////////////////////////
template <class InfoType>
CNode<InfoType>::CNode():_information(), m_is_expansible(true),
	m_is_solved(false), _parent(nullptr), m_out_children_index(0)
{
	m_childrens.clear();
}

////////////////////////////////////////////////////////////////////////////////
// Metodo constructor de nodo
// Recibe como parametros el nodo padre del nodo y el campo informativo del nodo
template <class InfoType>
CNode<InfoType>::CNode( CNode<InfoType> *parent, const InfoType& info,
						int index_of_children)
{
	set_information(info);
	m_childrens.clear();
	set_parent( parent );
	m_is_expansible = true;
	m_is_solved = false;
	m_out_children_index = index_of_children;
}

////////////////////////////////////////////////////////////////////////////////
template <class InfoType>
CNode<InfoType> &CNode<InfoType>::operator=(const CNode<InfoType> &original)
{
	if (this == &original) return *this;

	this->_parent = original._parent; // Asigna el padre
	this->m_out_children_index = original.m_out_children_index;

	// I need a correct copy constructor in InfoType class
	this->_information = original._information;
		
	m_childrens=original.m_childrens;
	m_is_expansible = original.m_is_expansible;
	m_is_solved = original.m_is_solved;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
template <class InfoType>
bool CNode<InfoType>::search(std::vector<InfoType> **visitados)
{
	int i = 0;
	int children_count = 0;

	if(this->get_informacion().isFinalCondition())
	{
		this->m_is_solved = true;
		std::cout << this->get_informacion() << std::endl;
		return true;
	}

	//--------------------------------------------------------------------------
	// save initial node information
	InfoType InformacionOriginal(this->get_informacion());

	//--------------------------------------------------------------------------
	// Node treatment

	// If node hasn't children it needs to generate, it's not possible to generate
	// return false;
	// If node can generate just one children, node information becomes its children
	// and continues generation children process
	while(children_count < 2)
	{
		children_count = this->generateChildrenInNode(visitados);
		if( children_count == 0)
		{
			// Maybe it's the solution
			if(this->get_informacion().isFinalCondition())
			{
				this->m_is_solved = true;
				return true;
			}
			else
			{
				// This fail node must be inserted in visited nodes with its correspond size
				this->set_information(InformacionOriginal);
	   			(*visitados)[this->get_informacion().get_occupiedBoxCount()].
	   											push_back(this->get_informacion());
				return false;
			}
		}
		
		if(children_count == 1)
		{
			if(m_childrens[0]->get_informacion().isFinalCondition())
			{
				this->set_information( this->m_childrens[0]->get_informacion());
				std::cout << this->get_informacion() << std::endl;
				m_childrens.clear();
				m_is_solved = true;
				return true;
			}

			this->set_information( this->m_childrens[0]->get_informacion());
			std::cout << this->get_informacion() << std::endl;
			m_childrens.clear();			
		}
	}

	// -------------------------------------------------------------------------
	// Node has generated several children [2, x]. It starts recursive search in
	// childrens
	if(children_count > 1)
	{
		std::cout <<  std::endl << std::endl << " CHILDREN COUNT: "
				<< this->m_childrens.size() << std::endl;
		std::cout << this->get_informacion() << std::endl << std::endl;
	}

	auto itChildren = m_childrens.begin();
	while (itChildren != m_childrens.end())
	{
		// When a children return false in its search, this children will be deleted
		if (!((*itChildren)->search(visitados)))
		{
			itChildren = m_childrens.erase(itChildren);
		}
		else
		{
			//itChildren++;
			this->set_information((*itChildren)->get_informacion());
			m_childrens.clear();
			m_is_solved = true;
			return true;
		}
	}

	// At this point, probably I have a unsolvable sudoku
	for(i = 0; i < 80; i++)
	{
		std::cout << " Visited nodes (" << i << ") without success: "
					<< (*visitados)[i].size() << std::endl;
	}

	//--------------------------------------------------------------------------
	// If all children return false, all of them will be deleted, and parent node
	// return false;
	if(m_childrens.empty())
	{
	    this->set_information(InformacionOriginal);
	    
		std::cout << " DELETED "<<  std::endl;
		std::cout << this->get_informacion() << std::endl;
	    
		(*visitados)[this->get_informacion().get_occupiedBoxCount()].
											push_back(this->get_informacion());
		return false;			 
	}
	else
	{
		return true;
	}
}

////////////////////////////////////////////////////////////////////////////////
template <class InfoType>
int CNode<InfoType>::generateChildrenInNode(std::vector<InfoType> **visitados)
{
	std::vector<InfoType> solutions;
	solutions.clear();

	if (!this->get_informacion().generateChildrens(&solutions, visitados))
	{
		// std::cout << "Solution" << std::endl;
		return 0;
	}

	this->m_childrens.clear();

	unsigned int i = 0;
	for (const auto& itBoardolutions : solutions)
	{
		std::shared_ptr<CNode<InfoType> >
					shr_ptr(new CNode<InfoType>(this, itBoardolutions, i));
		this->m_childrens.push_back(shr_ptr);
		i++;
	}

	return m_childrens.size();
}

#endif // _CNODE_HPP_
