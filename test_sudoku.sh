#!/bin/bash
################################################################################
## Test sudokus
## Juan Maria Gomez Lopez <juanecitorr@gmail.com>
################################################################################

BIN_FILE=./sudoku_solver
DATA_DIR=./data

for test_file in `ls ${DATA_DIR}`; do
	echo "----------------------------------------------------------------"
	echo "File Test: "$test_file
	time ${BIN_FILE} -f ${DATA_DIR}/$test_file
	echo "----------------------------------------------------------------"
done


exit 0

