/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sudoku_solver.hpp
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
 * @file sudoku_solver.hpp
 * @author Juan Maria Gomez Lopez (juanecitorr@gmail.com)
// @date 16//2008 C++11 revision 15 Apr 2013
*/

#ifndef _SUDOKU_HPP_
#define _SUDOKU_HPP_

#include <vector>
#include <iostream>
#include <cstring>

namespace sudoku{

enum E_SUDOKU
{
	E_SUDOKU_DIM = 9,
	E_SUDOKU_SQUARE_DIM = 3,
	E_SUDOKU_BOX_COUNT = 81,
	E_SUDOKU_BOX_STATES_COUNT = 10
};

};

#include "cnode.hpp"

namespace sudoku{

class CSudokuBoard;

////////////////////////////////////////////////////////////////////////////////
/**
 * @class CSudokuBox
 */
class CSudokuBox
{
	friend class CSudokuBoard;

	public:

		CSudokuBox();
		CSudokuBox(short int valor);
		CSudokuBox(const CSudokuBox &original);
		CSudokuBox& operator=(const CSudokuBox &original);
		bool setValor(short int valor);
		inline short int getValor() const { return _valor;}
		virtual ~CSudokuBox(){};

	private:

		bool _posiblesValores[E_SUDOKU_BOX_STATES_COUNT];
		short int _valor;
};

////////////////////////////////////////////////////////////////////////////////
CSudokuBox::CSudokuBox()
{
	_valor=0;
	for(int i = 0; i < E_SUDOKU_BOX_STATES_COUNT; i++) _posiblesValores[i] = true;
}

////////////////////////////////////////////////////////////////////////////////
CSudokuBox::CSudokuBox(short int valor)
{
					 
	if( (valor < 1) || (valor > 9) ){
		_valor=0;
		for(int i = 0; i< E_SUDOKU_BOX_STATES_COUNT; i++)
			_posiblesValores[i] = true;
	}
	else
	{
		_valor = valor;
		for(int i = 0; i < E_SUDOKU_BOX_STATES_COUNT; i++)
			_posiblesValores[i] = true;

		_posiblesValores[0]=false;
		_posiblesValores[valor]=false;
	}
}

////////////////////////////////////////////////////////////////////////////////
CSudokuBox::CSudokuBox(const CSudokuBox &original)
{
	*this = original;
}

////////////////////////////////////////////////////////////////////////////////
CSudokuBox& CSudokuBox::operator=(const CSudokuBox &original)
{
	if (this == &original) return *this;

	this->_valor = original._valor;
	for(int i = 0; i < E_SUDOKU_BOX_STATES_COUNT; i++)
		this->_posiblesValores[i] = original._posiblesValores[i];

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool CSudokuBox::setValor(short int valor)
{
	if( (valor < 0) || (valor > 9) ) return false;

	if(valor == 0)
	{
		_posiblesValores[_valor]=true;
		_valor = valor;
		_posiblesValores[0]=true;
		return true;
	}
	else
	{
		_posiblesValores[_valor]=true;
		_valor = valor;
		_posiblesValores[valor]=false;
		_posiblesValores[0]=false;
		return true;
	}
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @class CSudokuBoard
 */
class CSudokuBoard
{
	friend bool diferentes(CSudokuBoard primero,CSudokuBoard segundo);
	friend bool diferentes2(CSudokuBoard primero,CSudokuBoard segundo);

	public:

		/**
		 * Default constructor
		 */
		CSudokuBoard();

		/**
		 * Set value in board at position posX, poxY
		 * @param value
		 * @param posX
		 * @param posY
		 * @return true all is OK, false error inserting value
		 */
		bool setValorByXY(short int value, short int posX, short int posY);

		/**
		 * Get value by position in board posX, posY
		 * @param posX
		 * @param posY
		 * @return return value [1, 9] is all OK and -1 if there is any error
		 */
		short int getValorByXY(short int posX, short int posY) const;

		/**
		 * Copy constructor
		 * @param original
		 */
		CSudokuBoard(const CSudokuBoard &original);

		/**
		 * Copy operator
		 * @param original
		 * @return
		 */
		CSudokuBoard &operator=(const CSudokuBoard &original);

		/**
		 * Check if a number in board is complete, or si all
		 * number are completed (value = 0)
		 * @param value
		 * @return
		 */
		// Devuelve si un numero o todo el tablero esta completo
		// si se pregunta por 0, es por todo el tablero,
		// y numero de 1 a 9 por su correpondiente
		bool is_complete(short int value) const;

		/**
		 * Check sudoku rules when I want to insert value in position posX, posY
		 * @param valor
		 * @param posX
		 * @param posY
		 * @return true all rules are respected, false any rule isn't respected
		 */
		bool rulesCheck(int value, int posX, int posY);

		/**
		 *
		 * @return
		 */
		inline bool isFinalCondition(void) const
		{
			return m_occupiedBoxCount == E_SUDOKU_BOX_COUNT;
		}

		/**
		 *
		 * @return
		 */
		inline int get_occupiedBoxCount(void) const
		{
			return m_occupiedBoxCount;
		}

		/**
		 *
		 * @param soluciones
		 * @param visitados
		 * @return
		 */
		bool generateChildrens(std::vector<CSudokuBoard> *soluciones,
				  	  	  	  	  	std::vector<CSudokuBoard> **visitados) const
		{
			return this->generateSudokuBoardChildrens(soluciones, visitados, *this);
		}

	private:

		bool generateSudokuBoardChildrens(std::vector<CSudokuBoard> *solutions,
				  	  	  	  	  	std::vector<CSudokuBoard> **visited,
									const CSudokuBoard& primero) const;

		/**
		 *
		 */
		// Completo para cada uno de los numeros, y el indice 0 a true
		// significa completo entero
		void _update_complete();

		int m_occupiedBoxCount;

		CSudokuBox _boardBoxes[E_SUDOKU_DIM][E_SUDOKU_DIM];

		bool _completo[E_SUDOKU_BOX_STATES_COUNT];

};

////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream &input, CSudokuBoard &o)
{
	unsigned int current_row = 0;
	unsigned int file_row = 0;

	short int value[9];
	memset(value, 0, sizeof(short int) * E_SUDOKU_DIM);

	char separator[2] = {'\0', '\0'};
	std::string line_separator;

	while (current_row < E_SUDOKU_DIM)
	{
		if (file_row > 0 &&
			file_row < 11 &&
			(file_row + 1) % 4 == 0)
		{
			input >> line_separator;
		}
		else
		{
			input >> value[0] >> value[1] >> value[2] >> separator[0] >>
					value[3] >> value[4] >> value[5] >> separator[1] >>
					value[6] >> value[7] >> value[8];

			for (unsigned int uiI = 0; uiI < E_SUDOKU_DIM; uiI++)
			{
				if (!o.setValorByXY(value[uiI], current_row, uiI))
				{
					std::cerr << "Error file_row: " << file_row << std::endl;
					std::cerr << "Error current_row: " << current_row << std::endl;
					std::cerr << "Error current_column: " << uiI << std::endl;

					input.setstate(std::ios_base::badbit);
				    return input;
				}
			}

			current_row++;
		}
		file_row++;
	}

    return input;
}

////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &output, const CSudokuBoard &o)
{
	unsigned int current_row = 0;
	unsigned int file_row = 0;

	short int value[E_SUDOKU_DIM];
	memset(value, 0, sizeof(short int) * E_SUDOKU_DIM);

	char separator = '|';
	const std::string line_separator = "---------------------";

	output << "SUDOKU BOARD (" << o.get_occupiedBoxCount() << ")" << std::endl;

	while (current_row < E_SUDOKU_DIM)
	{
		if (file_row > 0 &&
			file_row < 11 &&
			(file_row + 1) % 4 == 0)
		{
			output << line_separator << std::endl;
		}
		else
		{
			for (unsigned int uiI = 0; uiI < E_SUDOKU_DIM; uiI++)
			{
				value[uiI] = o.getValorByXY(current_row, uiI);
			}

			output << value[0] << " " << value[1] << " " << value[2] << " " << separator << " " <<
					value[3] << " " << value[4] << " " << value[5] << " " << separator << " " <<
					value[6] << " " << value[7] << " " << value[8] << std::endl;

			current_row++;
		}
		file_row++;
	}

    return output;
}

////////////////////////////////////////////////////////////////////////////////
CSudokuBoard::CSudokuBoard()
{
	for(int i = 0;i < E_SUDOKU_BOX_STATES_COUNT; i++)
	{
		this->_completo[i] = false;
	}
	m_occupiedBoxCount = 0;
}

////////////////////////////////////////////////////////////////////////////////
CSudokuBoard::CSudokuBoard(const CSudokuBoard &original)
{
	*this = original;
}

////////////////////////////////////////////////////////////////////////////////
CSudokuBoard& CSudokuBoard::operator=(const CSudokuBoard &original)
{
	if (this == &original) return *this;

	int i,j;
	this->m_occupiedBoxCount = original.m_occupiedBoxCount;
	for(i = 0; i < E_SUDOKU_DIM; i++)
	{
		for(j = 0; j < E_SUDOKU_DIM; j++)
			this->_boardBoxes[i][j] = original._boardBoxes[i][j];
	}
	for(i = 0; i < E_SUDOKU_BOX_STATES_COUNT; i++)
	{
		this->_completo[i] = original._completo[i];
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
short int CSudokuBoard::getValorByXY(short int posX, short int posY) const
{
	
	if( (posX < 0) || (posX > 8) || (posY < 0) || (posY > 8) )
	{
		return -1;
	}
	else
	{
		return _boardBoxes[posX][posY].getValor();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Funcion para establecer valores en el tablero
// Se tienen en cuenta 3 posibilidades:
//		1- Si pasa de un valor [1-9] a un valor 0.
//		2- Si pasa de valor 0 a un valor [1-9].
//		3- Si pasa de valor [1-9] a un valor [1-9].
bool CSudokuBoard::setValorByXY(short int valor, short int posX, short int posY)
{
	// -------------------------------------------------------------------------
	// Check limits and values
	if( (valor<0) || (valor>9) || (posX<0) || (posX>8) || (posY<0) || (posY>8) )
		return false;

	// -------------------------------------------------------------------------
	// 0- Pure initialization, nothing to do
		if( valor == 0 && _boardBoxes[posX][posY].getValor() == 0 ) return true;

	// -------------------------------------------------------------------------
	// 1- If value is in [1-9] and it will be set to undetermined (0),
	// I have to remove restrictions
	if( valor == 0 && _boardBoxes[posX][posY].getValor() !=0 )
	{
		int i,j,k,l;
		int valor_antiguo = _boardBoxes[posX][posY].getValor();
		
		if( !_boardBoxes[posX][posY].setValor(valor)) return false;

		m_occupiedBoxCount--;

		//Quita las restricciones en los valores posibles para valor antiguo
    	
    	// Pongo todo el tablero a true para ese valor
    	for(i = 0; i < E_SUDOKU_DIM; i++)
    	{
    		for(j = 0; j< E_SUDOKU_DIM ;j++)
    		{
				_boardBoxes[i][j]._posiblesValores[valor_antiguo] = true;
			}
		}
		// Establezco la restricciones en todo el tablero de nuevo para ese valor_antiguo
    	for(i = 0; i < E_SUDOKU_DIM; i++)
    	{
    		for(j = 0; j < E_SUDOKU_DIM; j++)
    		{
				if( _boardBoxes[i][j].getValor() == valor_antiguo )
				{
					for(k = 0; k < (E_SUDOKU_BOX_STATES_COUNT - 1); k++)
					{
						_boardBoxes[i][k]._posiblesValores[valor_antiguo] = false;
						_boardBoxes[k][j]._posiblesValores[valor_antiguo] = false;
					}

					int cuadvert = j / E_SUDOKU_SQUARE_DIM;
					int cuadhori = i / E_SUDOKU_SQUARE_DIM;

					for(k = 0; k < E_SUDOKU_SQUARE_DIM; k++)
					{
						for(l = 0; l < E_SUDOKU_SQUARE_DIM; l++)
						{
							_boardBoxes[cuadhori * E_SUDOKU_SQUARE_DIM + k]
										[cuadvert * E_SUDOKU_SQUARE_DIM + l].
											_posiblesValores[valor_antiguo] = false;
						
						}
					}
				}
			}
		}

		// Actualiza los numeros completos
    	this->_update_complete();
    	return true;
	}

	// -------------------------------------------------------------------------
	// 2- If value move from undetermined to [1-9]. I have to put restrictions
	if( valor != 0 && _boardBoxes[posX][posY].getValor() == 0)
	{
		if(!rulesCheck(valor,posX,posY) )
		{
			return false;
		}
		else
		{
			int i,j;
			// Set value in board box
			if( !_boardBoxes[posX][posY].setValor(valor)) return false;

			m_occupiedBoxCount++;
			
			//Pone las restricciones en los valores posibles
    		
			// Ver el cuadrante que ocupa:
			// Horizontal
			int cuadhori = posX / E_SUDOKU_SQUARE_DIM;
			// Vertical
			int cuadvert = posY / E_SUDOKU_SQUARE_DIM;

    		// Set restrictions in square
    		for(i = 0; i < E_SUDOKU_SQUARE_DIM; i++)
    		{
    			for(j = 0; j < E_SUDOKU_SQUARE_DIM; j++)
    			{
    				_boardBoxes[cuadhori * E_SUDOKU_SQUARE_DIM + i]
								[cuadvert * E_SUDOKU_SQUARE_DIM + j].
									_posiblesValores[valor] = false;
    			}
    		}

    		// Set restrictions of column and row
    		for(i = 0; i < E_SUDOKU_DIM; i++)
    		{
    			_boardBoxes[posX][i]._posiblesValores[valor] = false;
    			_boardBoxes[i][posY]._posiblesValores[valor] = false;
    		}

			// Actualiza los numeros completos
    		this->_update_complete();

    		return true;
		}
	}

	// -------------------------------------------------------------------------
	// 3- If value move from [1-9] to [1-9], I have to change restrictions
	if( valor != 0 && _boardBoxes[posX][posY].getValor() != 0)
	{
		if(!rulesCheck(valor,posX,posY) )
		{
			return false;
		}
		else
		{
			int i,j,k,l,cuadvert,cuadhori;
			// recoge el valor antiguo
			int valor_antiguo = _boardBoxes[posX][posY].getValor();

			// Pone el valor en el tablero
			if( !_boardBoxes[posX][posY].setValor(valor)) return false;

    	// Pongo todo el tablero a true para ese valor_antiguo
    	for(i = 0; i < E_SUDOKU_DIM; i++)
    	{
    		for(j = 0; j < E_SUDOKU_DIM; j++)
    		{
				_boardBoxes[i][j]._posiblesValores[valor_antiguo] = true;
			}
		}
		
		// Establezco la restricciones en todo el tablero de nuevo
    	for(i = 0; i < E_SUDOKU_DIM; i++)
    	{
    		for(j = 0; j < E_SUDOKU_DIM; j++)
    		{
				if( _boardBoxes[i][j].getValor() == valor_antiguo )
				{
					for(k = 0; k < (E_SUDOKU_BOX_STATES_COUNT - 1); k++)
					{
						_boardBoxes[i][k]._posiblesValores[valor_antiguo] = false;
						_boardBoxes[k][j]._posiblesValores[valor_antiguo] = false;
					}
					cuadhori = i / E_SUDOKU_SQUARE_DIM;
					cuadvert = j / E_SUDOKU_SQUARE_DIM;
					for(k = 0;k < E_SUDOKU_SQUARE_DIM; k++)
					{
						for(l = 0; l < E_SUDOKU_SQUARE_DIM; l++)
						{
							_boardBoxes[cuadhori * E_SUDOKU_SQUARE_DIM + k]
										[cuadvert * E_SUDOKU_SQUARE_DIM + l].
											_posiblesValores[valor_antiguo] = false;
						
						}
					}
				}
			}
		}

		//Pone las restricciones en los valores posibles con el valor nuevo
    	
		// Ver el cuadrante que ocupa:
		// Horizontal
		cuadhori = posX / E_SUDOKU_SQUARE_DIM;
		// Vertical
		cuadvert = posY / E_SUDOKU_SQUARE_DIM;

    	// Restricciones de cuadrante
    	for(i = 0; i < E_SUDOKU_SQUARE_DIM; i++)
    	{
    		for(j = 0; j < E_SUDOKU_SQUARE_DIM; j++)
    		{
    			_boardBoxes[cuadhori * E_SUDOKU_SQUARE_DIM + i]
							[cuadvert * E_SUDOKU_SQUARE_DIM + j].
										_posiblesValores[valor] = false;
    		}
    	}
    	// Restricciones de columna y fila
    	for(i = 0; i < E_SUDOKU_DIM; i++)
    	{
    		_boardBoxes[posX][i]._posiblesValores[valor] = false;
    		_boardBoxes[i][posY]._posiblesValores[valor] = false;
    	}
    		
		// Actualiza los numeros completos
    	this->_update_complete();

    	return true;
		}
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////
bool CSudokuBoard::rulesCheck(int valor, int posX, int posY)
{
	if(valor == 0) return true;
	return (_boardBoxes[posX][posY])._posiblesValores[valor];
}

////////////////////////////////////////////////////////////////////////////////
// Devuelve si un numero o todo el tablero esta completo
// Si se pregunta por 0, es por todo el tablero,
// y numero de 1 a 9 por su correpondiente
// Cualquier valor fuera de [0,9] da false
bool CSudokuBoard::is_complete(short int valor) const
{
	if( (valor<0) || (valor>9) ) return false;
	return _completo[valor];
}

////////////////////////////////////////////////////////////////////////////////
// Comprueba si se ha completado algun numero o todo el tablero, y lo rellena en
// en el vector _completo
void CSudokuBoard::_update_complete(){

	register int i;
	int cuenta;
	int k; // valores posibles de sudoku
	for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
	{
		cuenta=0;
		for(i = 0; i < E_SUDOKU_BOX_COUNT; i++)
		{
			if(_boardBoxes[i/9][i%9].getValor() == k) cuenta++;
		}

		if(cuenta == 9) _completo[k] = true;
	}

	if( _completo[1] && _completo[2] && _completo[3] && _completo[4] && _completo[5] &&
		_completo[6] && _completo[7] && _completo[8] && _completo[9]) _completo[0] = true;
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Check if two sudoku board are equal
 * @param primero first sudoku board to compare
 * @param segundo second sudoku board to compare
 * @return if second board isn't equal from first board return true
 */
bool diferentes(CSudokuBoard primero,CSudokuBoard segundo){

	if( primero.m_occupiedBoxCount != segundo.m_occupiedBoxCount) return true;

	int i,j;	
	bool diferencia = false;

	for(i = 0; (i < E_SUDOKU_DIM && !diferencia); i++)
	{
		for(j = 0; (j < E_SUDOKU_DIM && !diferencia); j++)
		{
			if( (primero._boardBoxes[i][j]).getValor() !=
				(segundo._boardBoxes[i][j]).getValor() )
			{
				diferencia = true;
			}
   		}
	}
	return diferencia;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Check if two sudoku board are equal, or if second of both are derived
 * from first.
 * @param primero first sudoku board to compare
 * @param segundo second sudoku board to compare
 * @return if second board isn't equal and not derived from first board return true
 */
bool diferentes2(CSudokuBoard primero,CSudokuBoard segundo)
{
	register int i;
	bool diferencia=false;
	for(i = 0; (i < E_SUDOKU_BOX_COUNT && !diferencia); i++)
	{
		if( ( (primero._boardBoxes[i/9][i%9]).getValor()
					!= (segundo._boardBoxes[i/9][i%9]).getValor() )
			&& (primero._boardBoxes[i/9][i%9]).getValor() != 0)
		{
			diferencia = true;
		}
	}
	return diferencia;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief This function uses four rules to generate new sudoku boards from
 * original "primero". These new boards are checked with visited
 * before including them into solutions vector. It can generate safe children,
 * with 100% probability, or probable children, if safe children cannot be generated
 * @param solutions
 * @param visited
 * @param primero
 * @return
 */
bool CSudokuBoard::generateSudokuBoardChildrens(std::vector<CSudokuBoard> *solutions,
									std::vector<CSudokuBoard> **visited,
									const CSudokuBoard& primero) const
{
	// TODO: this function is too long

	// If sudoku board primero is completed, I cannot generate children
	if (primero.is_complete(0) ) return false;

	CSudokuBoard aux2 = primero;
	CSudokuBoard aux1 = primero;
	CSudokuBoard aux = primero;

	bool is_there_the_same_one = false;
	bool is_safe_children = false;
	unsigned int k,l,m,cuentatrue,valor;
	int z = 0;

	unsigned int P; // Probability
	unsigned int P_limit; // Probability limit
	int posXencontrado,posYencontrado;
	unsigned int register i,j;

	// If there is a safe children (probability 100%)
	// it will be returned as unique children

	bool nuevaInsercion = false;
	std::vector<int> antepasados;

	//--------------------------------------------------------------------------
	do{
		nuevaInsercion = false;
		//--------------------------------------------------------------------------
		// Rule 1. Check immediate values resolution
		for(i = 0; i < E_SUDOKU_DIM; i++)
		{
			for(j = 0; j < E_SUDOKU_DIM; j++)
			{
				// Pone el valor en la casilla donde solo se pueda poner ese
				if(aux1.getValorByXY(i,j) == 0)
				{ // solo pone en las casillas con 0
					cuentatrue = 0;
					for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
					{
						if(aux1._boardBoxes[i][j]._posiblesValores[k])
						{
							cuentatrue++;
							valor=k;
						}
					}
					if( cuentatrue == 1)
					{
						aux1.setValorByXY(valor,i,j);
						is_safe_children=true;
						nuevaInsercion = true;
					}
				}
			}
		}
		//--------------------------------------------------------------------------
		// Rule 2. Check 3x3 squares resolution
		for(l = 0; l < E_SUDOKU_DIM; l++)
		{
			for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
			{
				cuentatrue = 0;
				for(i = (l / E_SUDOKU_SQUARE_DIM) * E_SUDOKU_SQUARE_DIM;
					i < ((l / E_SUDOKU_SQUARE_DIM) * E_SUDOKU_SQUARE_DIM + E_SUDOKU_SQUARE_DIM);
					i++)
				{
					for(j = ((l % E_SUDOKU_SQUARE_DIM) * E_SUDOKU_SQUARE_DIM);
						j < (((l % E_SUDOKU_SQUARE_DIM) * E_SUDOKU_SQUARE_DIM) + E_SUDOKU_SQUARE_DIM);
						j++)
					{
						if(aux1._boardBoxes[i][j]._posiblesValores[k] &&
								aux1._boardBoxes[i][j]._posiblesValores[0])
						{
							cuentatrue++;
							valor=k;
							posXencontrado = i;
							posYencontrado = j;
						}

					}
				}
				if( (cuentatrue == 1)
						&&
					(aux1.getValorByXY(posXencontrado,posYencontrado) == 0) )
				{
					aux1.setValorByXY(valor,posXencontrado,posYencontrado);
					is_safe_children = true;
					nuevaInsercion = true;
				}
			}
		}

		//--------------------------------------------------------------------------
		// Rules 3 and 4 check column and row values
		for(i = 0; i < E_SUDOKU_DIM; i++)
		{
			// Check column
			for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
			{
				cuentatrue = 0;
				for(l = 0; l < E_SUDOKU_DIM; l++)
				{
					if(aux1._boardBoxes[i][l]._posiblesValores[k] &&
							aux1._boardBoxes[i][l]._posiblesValores[0])
					{
						cuentatrue++;
						valor = k;
						m = l;
					}
				}
				if( cuentatrue == 1)
				{
					if(aux1.getValorByXY(i,m) == 0)
					{
						aux1.setValorByXY(valor,i,m);
						is_safe_children = true;
						nuevaInsercion = true;
					}
					cuentatrue = 0;
				}
			}

			// Check row
			for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
			{
				cuentatrue = 0;
				for(l = 0 ; l < E_SUDOKU_DIM; l++)
				{
					if(aux1._boardBoxes[l][i]._posiblesValores[k] &&
							aux1._boardBoxes[l][i]._posiblesValores[0])
					{
						cuentatrue++;
						valor = k;
						m = l;
					}
				}
				if( cuentatrue == 1)
				{
					if(aux1.getValorByXY(m,i) == 0)
					{
						aux1.setValorByXY(valor,m,i);
						is_safe_children = true;
						nuevaInsercion = true;
					}
				}
			}
		}
	}while(nuevaInsercion);

	//--------------------------------------------------------------------------
	// 100% probability children
	if(is_safe_children)
	{
		is_there_the_same_one = false;
		z=0;
		antepasados.clear();
		while(z < (aux1.m_occupiedBoxCount - 1))
		{
			if( (*visited)[z].size()>0){ antepasados.push_back(z);}
			z++;			
		}
		for(m = 0;!is_there_the_same_one && (m < antepasados.size()) ;m++)
		{
			for(l=0;!is_there_the_same_one && (l<((*visited)[m]).size());l++)
			{
				if(!diferentes2(( (*visited)[m] ).at(l),aux1) )
				{
					//std::cout << " Safe children inclusion =" << std::endl;
					is_there_the_same_one = true;
				}
			}
		}
		if(!is_there_the_same_one)
		{
			solutions->push_back(aux1);
			//std::cout << " Safe children inclusion" << std::endl;
			return true;
		}
	}

	//--------------------------------------------------------------------------
	// Probability inclusion. From here, I'am going to include number in sudoku
	// board with certain probability level but not sure.

	// I have to find the box where it's less probable to make a mistake
	// and order them.
	std::vector<int> ii;
	std::vector<int> jj;

	// VERY IMPORTANT: THE LIMITS OF PROBABILITY MUST BE DEFINE CARESFULLY, THE
	// EXTENSION OF SEARCH TREE CAN DESTROY YOUR RAM MEMORY JA JA JA JA ....
	// LIMITS (2->50%, 3->33%, 4->25%, 5->20%, ...)

	// Here, I have to decide how many children I are going to create, and probability
	// level of them. More children -> more memory and CPU time
	P = 2; // From 50% probability
	P_limit = 8; // To 12.5% probability. my memory is infinite

	for(l = P; l < P_limit; l++)
	{
		for(i = 0; i < E_SUDOKU_DIM; i++)
		{
			for(j = 0; j < E_SUDOKU_DIM; j++)
			{
				if(aux2.getValorByXY(i,j) == 0)
				{
					cuentatrue=0;
					for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
					{
						if(aux2._boardBoxes[i][j]._posiblesValores[k]) cuentatrue++;
					}
					if(cuentatrue == l){
						ii.push_back(i);
						jj.push_back(j);
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------
	// Tomando el orden anterior se realiza una insercion sin reglas

	nuevaInsercion = false;

	do{
		nuevaInsercion = false;

		for(i = 0; i < ii.size() ;i++)
		{
			for(k = 1; k < E_SUDOKU_BOX_STATES_COUNT; k++)
			{
				if( aux.getValorByXY( ii.at(i) , jj.at(i) ) == 0)
				{
					if( aux.setValorByXY(k, ii.at(i) , jj.at(i) ) )
					{
						is_there_the_same_one=false;
						z=0;
						antepasados.clear();
						while(z < (aux.m_occupiedBoxCount - 1))
						{
							if( (*visited)[z].size()>0){ antepasados.push_back(z);}
							z++;
						}
						for(m = 0;!is_there_the_same_one && (m < antepasados.size()) ;m++)
						{
							for(l=0;!is_there_the_same_one && (l<((*visited)[m]).size());l++)
							{
								if(!diferentes2(( (*visited)[m] ).at(l),aux) ) is_there_the_same_one = true;
							}
						}

						if(!is_there_the_same_one)
						{
							solutions->push_back(aux);
							if( !aux.setValorByXY(0, ii.at(i) , jj.at(i) ) ) std::cerr << "error";
							// std::cout << " Probable children inclusion" << std::endl;
							std::cout << ". ";
							nuevaInsercion = true;
						}
					}
				}
			}
		}
	}while( (nuevaInsercion) && (k < E_SUDOKU_BOX_STATES_COUNT) );

	return true;
}

} // namespace sudoku
#endif // _SUDOKU_HPP_

