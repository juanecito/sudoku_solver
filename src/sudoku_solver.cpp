/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sudoku_solver.cpp
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

/** @file sudoku_solver.cpp
 * @author Juan Maria Gomez Lopez <juanecitorr@gmail.com>
 * @date 16 Nov 2008
 */

#include "sudoku_solver.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

#include <getopt.h>

#include "cnode.hpp"

using namespace sudoku;

int main(int argc, char** argv)
{

	CNode<CSudokuBoard> *initialState;
	initialState = new CNode<CSudokuBoard>;
	std::vector<CSudokuBoard> *visitados;
	visitados = new std::vector<CSudokuBoard>[E_SUDOKU_BOX_COUNT - 1];

	CSudokuBoard sudoku;

	//--------------------------------------------------------------------------
	// Get arguments
	int c = 0;
	char* file_name = nullptr;
	opterr = 0;

	while ((c = getopt (argc, argv, "f:")) != -1)
	{
		switch (c)
		{
			case 'f':
				file_name = optarg;
				break;
			case '?':
				if (optopt == 'f')
					fprintf (stderr,
						"Option -%c requires an argument: file name.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);
				return 1;
			default:
				abort ();
		}
	}

	//--------------------------------------------------------------------------
	// Get sudoku board from file
	std::ifstream sudoku_file(file_name);
	sudoku_file >> sudoku;

	if (sudoku_file.fail())
	{
		std::cerr << " Error getting sudoku board from file: " << std::endl;
		sudoku_file.close();
		return -1;
	}
	sudoku_file.close();

	//--------------------------------------------------------------------------
	// Sudoku resolution
	std::cout << " This sodoku will be solved: " <<
								std::endl << std::endl << sudoku << std::endl;

	std::cout << " Press intro key: " << std::endl << std::endl;
	c = getchar();
	initialState->set_information(sudoku);
	for(c = 0; c < E_SUDOKU_BOX_COUNT - 1; c++) visitados[c].clear();

	initialState->search(&visitados);

	delete initialState;
	delete[] visitados;

	return 0;
}
