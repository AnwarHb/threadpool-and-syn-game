# parallel Game of Life
this project implements a parallel Game of Life utilizing the thread-pool patteren as part of HW in operating systems course in Technion.

Life is played on a grid of square cells-like a chess board but extending infinitely in every direction. A cell
can be alive or dead. A live cell is shown by putting a marker on its square. A dead cell is shown by
leaving the square empty. Each cell in the grid has a neighborhood consisting of the eight cells in every
direction including diagonals, each cell belongs to a certain species, and each species is color-coded

The cycle of life in this game is divided into 2 phases, the first phase is where new cells are created and
some cells die, and in the second phase the cells get to know their lovely neighborhood and change its
properties according to the neighborhood it is in.
